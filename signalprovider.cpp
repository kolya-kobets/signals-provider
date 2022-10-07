#include "signalprovider.h"

SignalProvider::SignalProvider(IDataSource& source)
    : m_source(source)
{}

SignalProvider::~SignalProvider()
{
    stop();
}

void SignalProvider::register_client(ISignalReceiver* listener)
{
    std::lock_guard guard(m_listeners_lock);
    m_listeners.push_back(listener);
}

void SignalProvider::stop()
{
    //std::lock_guard guard(m_lock);
    m_reader.reset();

    {
    std::lock_guard guard(m_listeners_lock);
    m_listeners.clear();
    }
    {
    std::lock_guard guard(m_processors_lock);
    m_processors.clear();
    }
}

void SignalProvider::run()
{
    if(m_reader) {
        return;
    }
    const auto receiver = [this](const Frame& frame) {
        receive_frame(frame);
    };
    m_reader = std::make_unique<FrameReader>(m_source, receiver);
}

void SignalProvider::receive_frame(const Frame& frame)
{
    std::lock_guard guard(m_processors_lock);

    // Called from FrameReader's thread: creating new processor is done
    // rarely(once per source), so it doesnt make sense to move it to a separate thread.
    if(!m_processors[frame.source_id]) {
        const auto receiver = [this](const Signal& signal, ReadStatus status) {
            receive_signal(signal, status);
        };
        m_processors[frame.source_id] = std::make_unique<FrameProcessor>(frame.source_id, receiver);
    }

    m_processors[frame.source_id]->process_frame(frame);
}

void SignalProvider::receive_signal(const Signal& signal, ReadStatus status)
{
    std::lock_guard guard(m_listeners_lock);

    for(const auto listener : m_listeners) {
        listener->process(signal, status);
    }
}
