#ifndef ISIGNALRECEIVER_H
#define ISIGNALRECEIVER_H

#include "base_types.h"

class ISignalReceiver // == IClient in the uml
{
public:
    virtual void process(const Signal& signal, ReadStatus status) = 0;
    virtual ~ISignalReceiver() = default;
};

#endif // ISIGNALRECEIVER_H

