#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "base_types.h"
#include "asynctaskprocessor.h"
#include <optional>

using SignalHandler = std::function<void(const Signal&, ReadStatus)>;

class FrameProcessor
{
public:
    FrameProcessor(std::uint8_t source_id, const SignalHandler& handler);
    ~FrameProcessor();
    void process_frame(const Frame& frame);
private:
    void send_missing_signals(std::uint16_t current_counter);
    static Signal create_bad_signal(std::uint16_t frame_counter, std::uint8_t source_id);

    std::uint8_t m_source_id;
    SignalHandler m_handler;
    AsyncTaskProcessor m_processor;

    std::optional<std::uint16_t> m_last_counter;
};

#endif // FRAMEPROCESSOR_H
