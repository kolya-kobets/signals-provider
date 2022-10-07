#include "frameprocessor.h"

#include "converter.h"

FrameProcessor::FrameProcessor(std::uint8_t source_id, const SignalHandler& handler)
    : m_source_id(source_id), m_handler(handler)
{}

FrameProcessor::~FrameProcessor()
{}

void FrameProcessor::process_frame(const Frame& frame)
{
    m_processor.run_task(
        [this, frame](){
            send_missing_signals(frame.frame_counter);
            m_last_counter = frame.frame_counter;

            const auto signal = Converter::convert(frame);
            m_handler(signal, ReadStatus::OK);
        }
    );
}

void FrameProcessor::send_missing_signals(std::uint16_t current)
{
    if(!m_last_counter) {
        return;
    }

    for(std::uint16_t counter = *m_last_counter+1; counter!=current; ++counter) {
        const auto& bad_signal = create_bad_signal(counter, m_source_id);
        m_handler(bad_signal, ReadStatus::BAD_SIGNAL);
    }
}

Signal FrameProcessor::create_bad_signal(std::uint16_t frame_counter, std::uint8_t source_id)
{
    return {
        .frame_counter = frame_counter,
        .source_id = source_id,
        .signal = boost::float32_t(0.f)
    };
}
