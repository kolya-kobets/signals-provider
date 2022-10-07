#include "converter.h"

struct PayloadConverter
{
    //std::uint8_t, std::int16_t, std::int32_t,
    template<typename T>
    boost::float32_t operator()(const T& val) {
        constexpr boost::float32_t MIN = std::numeric_limits<T>::min();
        constexpr boost::float32_t MAX = std::numeric_limits<T>::max();
        boost::float32_t res = ((val-MIN) / (MAX-MIN)) * 2.f - 1.f;
        return res;
    }

    boost::float32_t operator()(boost::float32_t val) {
        return val;
    }
};

Signal Converter::convert(const Frame& frame)
{
    const Signal result = {
        .frame_counter = frame.frame_counter,
        .source_id = frame.source_id,
        .signal = std::visit(PayloadConverter(), frame.payload)
    };
    return result;
}
