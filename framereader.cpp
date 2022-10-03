#include "framereader.h"
#include "print.h"

FrameReader::FrameReader(IDataSource& source, const FrameHandler& handler)
    : m_source(source)
    , m_handler(handler)
{
    m_processor.run_task([this](){run();});
}

FrameReader::~FrameReader()
{
    m_stop = true;
}

void FrameReader::run()
{
    FrameHeader header = {};
    Frame::Payload payload;
    Frame frame;
    while(!m_stop) {
        //TODO: error handling
        //TODO: limit time
        read_header(header);
        read_payload(header.payload_type, payload);

        frame.frame_counter = header.frame_counter;
        frame.source_id = header.source_id;
        frame.payload = payload;

        m_handler(frame, ReadStatus::OK);
    }
    LOGI("FrameReader::run ended");
}

ReadStatus FrameReader::read_header(FrameHeader& header)
{
    read(reinterpret_cast<char*>(&header), sizeof(FrameHeader));
    return ReadStatus::OK;
}

ReadStatus FrameReader::read_payload(std::uint8_t payload_type, Frame::Payload& result)
{
    //std::uint8_t, std::int16_t, std::int32_t, boost::float32_t
    switch(payload_type) {
        case PAYLOAD_TYPE0: {
            std::uint8_t payload = 0;
            read(reinterpret_cast<char*>(&payload), sizeof(payload));
            result = payload;
            break;
        }
        case PAYLOAD_TYPE1: {
            std::int16_t payload = 0;
            read(reinterpret_cast<char*>(&payload), sizeof(payload));
            result = payload;
            break;
        }
        case PAYLOAD_TYPE2: {
            std::int32_t payload = 0;
            read(reinterpret_cast<char*>(&payload), sizeof(payload));
            result = payload;
            break;
        }
        case PAYLOAD_TYPE3: {
            boost::float32_t payload = 0;
            read(reinterpret_cast<char*>(&payload), sizeof(payload));
            result = payload;
            break;
        }
    }
    return ReadStatus::OK;
}

void FrameReader::read(char* buffer, int size)
{
    char* ptr = buffer;
    int remaining = size;
    while(remaining > 0) {
        int read_size = m_source.read(ptr, remaining);
        ptr += read_size;
        remaining -= read_size;
    }
}
