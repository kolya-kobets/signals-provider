#ifndef TESTDATASOURCE_H
#define TESTDATASOURCE_H

#include "idatasource.h"
#include <deque>
#include <cstdint>
#include <random>
#include <limits>
#include <map>
#include "base_types.h"

class TestDataSource : public IDataSource
{
public:
    TestDataSource();
    int read(char* dest, int size) override;
private:
    void delay();

    void gen_frame();
    FrameHeader gen_header(std::uint8_t payloadtype);

    std::uint8_t gen_source_id();

    template<typename PayloadType, class Distribution>
    PayloadType gen_payload(PayloadType min=std::numeric_limits<PayloadType>::min(),
                            PayloadType max=std::numeric_limits<PayloadType>::max());

    std::uint8_t gen_payload_0();
    std::int16_t gen_payload_1();
    std::int32_t gen_payload_2();
    boost::float32_t gen_payload_3();

    void gen_noise();

    void skip_some_frames(std::uint8_t source_id);

    template<typename T>
    void write_to_buffer(const T& value);

    template<typename Val>
    Val random(const Val& min=std::numeric_limits<Val>::min(),
               const Val& max=std::numeric_limits<Val>::max());

    std::deque<char> m_buffer;
    std::random_device m_random_device;
    std::mt19937 m_random_engine;
    std::map<std::uint8_t, std::uint16_t> m_frame_counters; // frame counter for each source_id
};

#endif // TESTDATASOURCE_H
