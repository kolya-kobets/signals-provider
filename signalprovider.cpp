#include "signalprovider.h"

SignalProvider::SignalProvider(IDataSource& source)
    : m_source(source)
{}

void SignalProvider::register_client(ISignalReceiver* listener)
{
    m_listeners.push_back(listener);
}

void SignalProvider::stop()
{
    m_reader.reset();
    m_processors.clear();
}

void SignalProvider::run()
{
    if(m_reader) {
        return;
    }
    const auto receiver = [this](const Frame& frame, ReadStatus status) {
        receive_frame(frame, status);
    };
    m_reader = std::make_unique<FrameReader>(m_source, receiver);
}

void SignalProvider::receive_frame(const Frame& frame, ReadStatus status)
{
    // Called from FrameReader's thread
    if (status != ReadStatus::OK) {
        return;
    }

    if(!m_processors[frame.source_id]) {
        const auto receiver = [this](const Signal& frame, ReadStatus status) {
            receive_signal(frame, status);
        };
        m_processors[frame.source_id] = std::make_unique<FrameProcessor>(receiver);
    }

    m_processors[frame.source_id]->process_frame(frame, status);
}

void SignalProvider::receive_signal(const Signal& signal, ReadStatus status)
{
    for(const auto listener : m_listeners) {
        listener->process(signal, status);
    }
}
