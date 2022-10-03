#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "base_types.h"
#include "asynctaskprocessor.h"

using SignalHandler = std::function<void(const Signal&, ReadStatus)>;

class FrameProcessor
{
public:
    FrameProcessor(const SignalHandler& handler);
    void process_frame(const Frame& frame, ReadStatus status);
private:
    SignalHandler m_handler;
    AsyncTaskProcessor m_processor;
};

#endif // FRAMEPROCESSOR_H
