#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include "base_types.h"
#include "asynctaskprocessor.h"
#include "bufferedreader.h"

using FrameHandler = std::function<void (const Frame&)>;

class FrameReader
{
public:
    FrameReader(IDataSource& source, const FrameHandler& handler);
    ~FrameReader();
private:
    void run();

    ReadStatus read_header(FrameHeader& header);
    ReadStatus read_payload(std::uint8_t payload_type, Frame::Payload& payload);

    void read(char* buffer, int size);
    void find_good_frame();

    BufferedReader m_reader;
    FrameHandler m_handler;
    std::atomic_bool m_stop = false;
    AsyncTaskProcessor m_processor;
};

#endif // FRAMEREADER_H
