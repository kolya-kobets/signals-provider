#include "testdatasource.h"

#include "base_types.h"
#include <boost/cstdfloat.hpp>
#include <chrono>
#include <thread>
#include <print.h>

#undef CONTEXT
#define CONTEXT "GENERATOR"

TestDataSource::TestDataSource()
    : m_random_engine(m_random_device())
{

}


int TestDataSource::read(char* dest, int size)
{
    delay();

    if (m_buffer.size() < size) {
        gen_noise();
        gen_frame();
    }
    const auto read_size = std::min<int>(m_buffer.size(), size);
    std::copy(m_buffer.begin(), m_buffer.begin()+read_size, dest);
    m_buffer.erase(m_buffer.begin(), m_buffer.begin()+read_size);
    return read_size;
}

void TestDataSource::delay()
{
    const auto ms = std::chrono::milliseconds(random(100, 400));
    std::this_thread::sleep_for(ms);
}

void TestDataSource::gen_frame()
{
    const auto payloadtype = random(PAYLOAD_TYPE0, PAYLOAD_TYPE3);

    auto header = gen_header(payloadtype);

    std::variant<int32_t, float> payload;
    switch (payloadtype) {
        case PAYLOAD_TYPE0:
            payload = static_cast<int32_t>(gen_payload_0());
            break;
        case PAYLOAD_TYPE1:
            payload = static_cast<int32_t>(gen_payload_1());
            break;
        case PAYLOAD_TYPE2:
            payload = gen_payload_2();
            break;
        case PAYLOAD_TYPE3:
            payload = gen_payload_3();
            break;
    }
    LOGI("Frame[src: ", (int)header.source_id, ", counter: ", header.frame_counter,
         ", payload_type: ", (int)header.payload_type, ", payload: ", payload, "]");
}

FrameHeader TestDataSource::gen_header(std::uint8_t payloadtype)
{
    const auto source_id = gen_source_id();
    skip_some_frames(source_id);

    const auto payload_size = [payloadtype]() {
        if (payloadtype == PAYLOAD_TYPE0) {
            return 1U;
        }
        if (payloadtype == PAYLOAD_TYPE1) {
            return 2U;
        }
        return 4U;
    }();

    const FrameHeader header = {
        .magic_word = WORD,
        .frame_counter = m_frame_counters[source_id],
        .source_id = source_id,
        .payload_type = payloadtype,
        .payload_size = payload_size
    };

    m_frame_counters[source_id]++;

    write_to_buffer(header);

    return header;
}

std::uint8_t TestDataSource::gen_source_id()
{
    constexpr std::uint8_t MIN = 0u;
    constexpr std::uint8_t MAX = 10u;
    using Distribution = std::uniform_int_distribution<std::uint8_t>;

    Distribution distribution(MIN, MAX);
    return distribution(m_random_engine);
}

template<typename PayloadType, class Distribution>
PayloadType TestDataSource::gen_payload(PayloadType min, PayloadType max)
{
    Distribution distribution(min, max);
    const auto payload = static_cast<PayloadType>(distribution(m_random_engine));
    write_to_buffer(payload);
    return payload;
}

std::uint8_t TestDataSource::gen_payload_0()
{
    using Distribution = std::uniform_int_distribution<std::uint8_t>;
    return gen_payload<std::uint8_t, Distribution>();
}

std::int16_t TestDataSource::gen_payload_1()
{
    using Distribution = std::uniform_int_distribution<std::int16_t>;
    return gen_payload<std::int16_t, Distribution>();
}

std::int32_t TestDataSource::gen_payload_2()
{
    using Distribution = std::uniform_int_distribution<std::int32_t>;
    return gen_payload<std::int32_t, Distribution>();
}

boost::float32_t TestDataSource::gen_payload_3()
{
    using Distribution = std::uniform_real_distribution<boost::float32_t>;
    return gen_payload<boost::float32_t, Distribution>(boost::float32_t(-1.), boost::float32_t(1.));
}

void TestDataSource::gen_noise()
{
    const int Probability = 2;
    int val = random(0, 100);
    if (val > Probability) {
        return;
    }

    const int NoiseSizeMin = 3;
    // don't make much noise to reduce the probability of collision with magic word
    const int NoiseSizeMax = 100;
    const int NoiseSize = random(NoiseSizeMin, NoiseSizeMax);

    for(int i=0; i<NoiseSize; ++i) {
        m_buffer.push_back(random<std::uint8_t>());
    }
    LOGI("Generated noise of size:", NoiseSize);
}

void TestDataSource::skip_some_frames(std::uint8_t source_id)
{
    // first frame for the source
    if (m_frame_counters[source_id] == 0) {
        return;
    }
    const int Probability = 20;
    int val = random(0, 100);
    if (val > Probability) {
        return;
    }

    const int FramesCountMin = 1;
    const int FramesCountMax = 8;
    const int SkipNumber = random(FramesCountMin, FramesCountMax);

    const auto skip1 = m_frame_counters[source_id];

    m_frame_counters[source_id] += SkipNumber;
    const auto skip2 = m_frame_counters[source_id];

    LOGI("Skip frames for source_id: ", (int)source_id, ", skip range: [", skip1, ", ", skip2, ")");
}

template<typename T>
void TestDataSource::write_to_buffer(const T& value)
{
    auto ptr = reinterpret_cast<const char*>(&value);
    std::copy(ptr, ptr+sizeof(T), std::back_inserter(m_buffer));
}

template<typename Val>
Val TestDataSource::random(const Val& min, const Val& max)
{
    std::uniform_int_distribution<Val> distribution(min, max);
    return distribution(m_random_engine);
}
