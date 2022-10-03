#pragma once

#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/thread.hpp>
#include <vector>
#include <ostream>

inline auto time()
{
    //TODO: print milliseconds
    const auto date_time = boost::posix_time::second_clock::local_time();
    return date_time.time_of_day();
}

inline boost::thread::id thread()
{
  return boost::this_thread::get_id();
}


extern boost::mutex mtx;

template<class... Args>
void sync_print(Args... args) {
  boost::lock_guard<boost::mutex> guard(mtx);
//  std::cout << thread() << " : " << time();
  ((std::cout << args), ...);
  std::cout << std::endl;
}

#define LOG(...) sync_print(__PRETTY_FUNCTION__, ":", __LINE__, " Thread:", thread(), " Time:", time(), " ", __VA_ARGS__)
#define LOG_SHORT(...) sync_print("Thread:", thread(), " Time:", time(), " ", __VA_ARGS__)
#define LOGI(...) LOG_SHORT(__VA_ARGS__)
#define LOGE(...) LOG_SHORT(__VA_ARGS__)

// Print operators
template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& array)
{
    stream << "[ size:" << array.size() << ", items: ";
    for(const auto& val : array) {
        stream << val << " ";
    }
    stream << "]";
    return stream;
}
