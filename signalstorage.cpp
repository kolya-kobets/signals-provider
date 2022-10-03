#include "signalstorage.h"

SignalStorage::SignalStorage()
{
    add_block(1U);
}

void SignalStorage::process(const Signal& signal, ReadStatus /*status*/)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    auto& last_block = m_storage.back();
    if (last_block.capacity() == last_block.size()) {
        add_block(last_block.size()*2U);
    }
    m_storage.back().push_back(signal);
}

void SignalStorage::add_block(size_t size)
{
    std::vector<Signal> block;
    block.reserve(size);
    m_storage.push_back(std::move(block));
}
