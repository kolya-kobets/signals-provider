#ifndef SIGNALPROVIDER_H
#define SIGNALPROVIDER_H

#include "isignalreceiver.h"
#include <vector>

#include "framereader.h"
#include "frameprocessor.h"

class SignalProvider
{
public:
    SignalProvider(IDataSource& source);
    void register_client(ISignalReceiver* listener);

    void run();
    void stop();
private:
    void receive_frame(const Frame& frame, ReadStatus status);
    void receive_signal(const Signal& signal, ReadStatus status);

    IDataSource& m_source;
    std::unique_ptr<FrameReader> m_reader;
    std::map<std::uint8_t, std::unique_ptr<FrameProcessor> > m_processors;

    std::vector<ISignalReceiver*> m_listeners;
};

#endif // SIGNALPROVIDER_H
