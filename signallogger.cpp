#include "signallogger.h"
#include "print.h"

#undef CONTEXT
#define CONTEXT "FRAMERECEIVER"

void SignalLogger::process(const Signal& signal, ReadStatus status)
{
    LOGI(signal, " status:", status);
}
