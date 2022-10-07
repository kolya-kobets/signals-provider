#include "bufferedreader.h"

BufferedReader::BufferedReader(IDataSource& source)
    : m_source(source)
{ }

char BufferedReader::operator[](size_t ind)
{
    prepare_data(ind+1);
    return m_buffer[m_cursor+ind];
}

void BufferedReader::read(char* buffer, int size)
{
    prepare_data(size);
    const auto start = m_buffer.begin()+m_cursor;
    std::copy(start, start+size, buffer);
}

void BufferedReader::rewind(size_t offset)
{
    prepare_data(offset+1);
    m_cursor += offset;
}

void BufferedReader::prepare_data(size_t requested_size)
{
    assert(requested_size < m_buffer.size());
    if(m_cursor+requested_size < size()) {
        return;
    }

    if(m_cursor != 0) {
        std::copy(m_buffer.begin()+m_cursor, m_end, m_buffer.begin());
        m_end -= m_cursor; // == m_buffer.begin() + size() - m_cursor;
        m_cursor = 0;
    }


    while(size() < requested_size) {
        int chunk_size = m_source.read(&(*m_end), m_buffer.end()-m_end);
        m_end += chunk_size;
    }
}

size_t BufferedReader::size() const
{
    return m_end - m_buffer.begin();
}
