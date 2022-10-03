#include "signallogger.h"
#include "print.h"

void SignalLogger::process(const Signal& signal, ReadStatus status)
{
    LOGI(signal, " status:", status);
}
