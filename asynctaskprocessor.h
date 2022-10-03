#ifndef ASYNCTASKPROCESSOR_H
#define ASYNCTASKPROCESSOR_H

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

class AsyncTaskProcessor
{
public:
    AsyncTaskProcessor();
    ~AsyncTaskProcessor();

    template< typename Task >
    void run_task( Task task ){
        m_service.post(task);
    }
private:
    boost::asio::io_service m_service;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    boost::thread m_thread;
};

#endif // ASYNCTASKPROCESSOR_H
