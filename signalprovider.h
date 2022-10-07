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
    ~SignalProvider();
    void register_client(ISignalReceiver* listener);

    void run();
    void stop();
private:
    void receive_frame(const Frame& frame);
    void receive_signal(const Signal& signal, ReadStatus status);

    IDataSource& m_source;

    std::unique_ptr<FrameReader> m_reader;

    std::mutex m_processors_lock;
    std::map<std::uint8_t, std::unique_ptr<FrameProcessor> > m_processors;

    std::mutex m_listeners_lock;
    std::vector<ISignalReceiver*> m_listeners;
};

#endif // SIGNALPROVIDER_H
