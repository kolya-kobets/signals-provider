#include "asynctaskprocessor.h"

#include "print.h"
#include <boost/bind/bind.hpp>

AsyncTaskProcessor::AsyncTaskProcessor()
    : m_work(std::make_unique<boost::asio::io_service::work>(m_service)),
      m_thread(boost::bind(&boost::asio::io_service::run, &m_service))
{

}

AsyncTaskProcessor::~AsyncTaskProcessor()
{
    LOGI("~AsyncTaskProcessor");
    //m_service.stop();
    m_work.reset();

    try {
        m_thread.join();
    }
    catch(...) {
        LOGE("ERROR: ~AsyncTaskProcessor");
    }
}

