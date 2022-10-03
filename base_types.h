#ifndef RAWFRAME_H
#define RAWFRAME_H

#include "compiler_ext.h"

#include <cstdint>
#include <variant>
#include <boost/cstdfloat.hpp>

constexpr std::uint8_t PAYLOAD_TYPE0 = 0;
constexpr std::uint8_t PAYLOAD_TYPE1 = 1;
constexpr std::uint8_t PAYLOAD_TYPE2 = 2;
constexpr std::uint8_t PAYLOAD_TYPE3 = 3;

#define WORD {'H', 'E', 'A', 'D'}
PACKED_STRUCT(FrameHeader)
{
    char magic_word[4] = WORD;
    std::uint16_t frame_counter;
    std::uint8_t source_id;
    std::uint8_t payload_type;
    std::uint32_t payload_size;
};

struct Frame
{
    std::uint16_t frame_counter;
    std::uint8_t source_id;
    using Payload = std::variant<std::uint8_t, std::int16_t, std::int32_t, boost::float32_t>;
    Payload payload;
};

enum class ReadStatus
{
    BAD_FRAME, OK
};

struct Signal
{
    std::uint16_t frame_counter;
    std::uint8_t source_id;
    boost::float32_t signal;
};


// Output operators

inline std::ostream& operator<<(std::ostream& stream, const ReadStatus& status)
{
    switch(status) {
    case ReadStatus::BAD_FRAME:
        stream << "BAD_FRAME";
        break;
    case ReadStatus::OK:
        stream << "OK";
        break;
    }
    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Signal& signal)
{
    stream << "S[src:" << (int)signal.source_id
           << ", cnt: " << signal.frame_counter
           << ", val: " << std::fixed << std::setprecision(5) << signal.signal << "]";
    return stream;
}

#endif // RAWFRAME_H
