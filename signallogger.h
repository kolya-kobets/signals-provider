#ifndef SIGNALLOGGER_H
#define SIGNALLOGGER_H

#include "print.h"
#include "isignalreceiver.h"

class SignalLogger : public ISignalReceiver
{
public:
    void process(const Signal& signal, ReadStatus status) override;
};

#endif // SIGNALLOGGER_H
