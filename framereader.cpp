#include "framereader.h"
#include "print.h"
#include <thread>

#undef CONTEXT
#define CONTEXT "FRAMERECEIVER"

FrameReader::FrameReader(IDataSource& source, const FrameHandler& handler)
    : m_reader(source)
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
    LOGI("FrameReader::thread started");

    FrameHeader header = {};
    Frame::Payload payload;
    Frame frame;

    using std::chrono::milliseconds;
    using std::chrono::steady_clock;
    using std::chrono::duration_cast;

    const auto TIME_LIMIT = milliseconds(100);
    while(!m_stop) {
        auto start = steady_clock::now();

        find_good_frame();

        read_header(header);
        read_payload(header.payload_type, payload);

        frame.frame_counter = header.frame_counter;
        frame.source_id = header.source_id;
        frame.payload = payload;

        m_handler(frame);
        auto end = steady_clock::now();

        auto diff = duration_cast<milliseconds>(end - start);
        if(diff < TIME_LIMIT) {
            std::this_thread::sleep_for(TIME_LIMIT- diff);
        }
    }
    LOGI("FrameReader::thread ended");
}

ReadStatus FrameReader::read_header(FrameHeader& header)
{
    read(reinterpret_cast<char*>(&header), sizeof(FrameHeader));
    return ReadStatus::OK;
}

ReadStatus FrameReader::read_payload(std::uint8_t payload_type, Frame::Payload& result)
{
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
    m_reader.read(buffer, size);
    m_reader.rewind(size);
}

void FrameReader::find_good_frame()
{
    static constexpr char MAGIC_WORD[] = WORD;
    static constexpr size_t SIZE = sizeof(MAGIC_WORD);
    char word[SIZE] = "BAD";

    int skipped = 0;
    while(true)
    {
        m_reader.read(word, SIZE);
        if(std::equal(word, word+SIZE, MAGIC_WORD)){
            break;
        }
        m_reader.rewind(1);
        skipped += 1;
    }
    if(skipped > 0) {
        LOGI("Bad frames detected, skipped ", skipped, " bytes");
    }
}
