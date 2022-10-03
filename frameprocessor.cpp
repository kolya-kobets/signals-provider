#include "frameprocessor.h"

#include "converter.h"

FrameProcessor::FrameProcessor(const SignalHandler& handler)
    : m_handler(handler)
{}

void FrameProcessor::process_frame(const Frame& frame, ReadStatus status)
{
    m_processor.run_task(
        [this, frame, status](){
            const auto signal = Converter::convert(frame, status);
            m_handler(signal, status);
        }
    );
}
