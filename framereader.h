#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include "base_types.h"
#include "asynctaskprocessor.h"
#include "idatasource.h"

using FrameHandler = std::function<void (const Frame&, ReadStatus)>;

class FrameReader
{
public:
    FrameReader(IDataSource& source, const FrameHandler& handler);
    ~FrameReader();
private:
    void run();

    void find_header(FrameHeader& header, int& offset);

    ReadStatus read_header(FrameHeader& header);
    ReadStatus read_payload(std::uint8_t payload_type, Frame::Payload& payload);

    void read(char* buffer, int size);

    IDataSource& m_source;
    FrameHandler m_handler;
    std::atomic_bool m_stop = false;
    AsyncTaskProcessor m_processor;
};

#endif // FRAMEREADER_H
