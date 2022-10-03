#ifndef SIGNALSTORAGE_H
#define SIGNALSTORAGE_H

#include "isignalreceiver.h"

#include <list>
#include <vector>
#include <mutex>

class SignalStorage : public ISignalReceiver
{
public:
    SignalStorage();
    void process(const Signal& signal, ReadStatus status) override;
private:
    void add_block(size_t size);

    std::list<std::vector<Signal>> m_storage;
    std::mutex m_mutex;
};

#endif // SIGNALSTORAGE_H
