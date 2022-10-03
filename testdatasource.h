#ifndef TESTDATASOURCE_H
#define TESTDATASOURCE_H

#include "idatasource.h"
#include <deque>
#include <cstdint>
#include <random>
#include <limits>
#include <map>

class TestDataSource : public IDataSource
{
public:
    TestDataSource();
    int read(char* dest, int size) override;
private:
    void delay();

    void gen_frame();

    void gen_header(std::uint8_t payloadtype);

    std::uint8_t gen_source_id();

    template<typename PayloadType, class Distribution>
    void gen_payload(PayloadType Min=std::numeric_limits<PayloadType>::min(),
                     PayloadType Max=std::numeric_limits<PayloadType>::max());

    void gen_payload_0();
    void gen_payload_1();
    void gen_payload_2();
    void gen_payload_3();

    template<typename T>
    void write_to_buffer(const T& value);

    template<typename Val>
    Val random(const Val& min, const Val& max);

    std::deque<char> m_buffer;
    std::random_device m_random_device;
    std::mt19937 m_random_engine;
    std::map<std::uint8_t, std::uint16_t> m_frame_counters; // frame counter for each source_id
};

#endif // TESTDATASOURCE_H
