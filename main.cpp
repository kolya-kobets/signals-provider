#include <iostream>

#include "testdatasource.h"
#include "signalprovider.h"
#include "signallogger.h"
#include "signalstorage.h"

using namespace std;

int main()
{
//    AsyncTaskProcessor processor;
//    processor.run_task([]() {
//        cout << "Hello World!" << endl;
//    });
{
    TestDataSource dataSource;
    SignalProvider provider(dataSource);
    SignalLogger logger;
    SignalStorage storage;
    provider.register_client(&logger);
    provider.register_client(&storage);
    provider.run();
    sleep(3);
    provider.stop();
}
    return 0;
}
