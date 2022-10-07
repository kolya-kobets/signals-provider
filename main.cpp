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
    SignalLogger logger;
    SignalStorage storage;

    SignalProvider provider(dataSource);
    provider.register_client(&logger);
    provider.register_client(&storage);
    provider.run();
    sleep(4);
}
    return 0;
}
