#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H

#include "idatasource.h"
#include <vector>
#include "base_types.h"

class BufferedReader
{
public:
    BufferedReader(IDataSource& source);
public:
    char operator[](size_t ind);
    void read(char* buffer, int size);
    void rewind(size_t offset = 1U);
private:
    IDataSource& m_source;

    void prepare_data(size_t requested_size);
    size_t size() const;

    static constexpr size_t CAPACITY = 128;
    static_assert(CAPACITY > sizeof(FrameHeader)+sizeof(boost::float32_t),
        "CAPACITY is too small comparing to input frame"
    );
    size_t m_cursor = 0;
    std::array<char, CAPACITY> m_buffer;
    decltype(m_buffer)::iterator m_end = m_buffer.begin();
};

#endif // BUFFEREDREADER_H
