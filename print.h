#pragma once

#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/thread.hpp>
#include <vector>
#include <ostream>
#include <variant>

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

#define CONTEXT ""

#define LOG(...) sync_print(__PRETTY_FUNCTION__, ":", __LINE__, " Thread:", thread(), " Time:", time(), " ", __VA_ARGS__)

#define LOG_SHORT(...) sync_print("Thread:", thread(), " Time:", time(), " ", __VA_ARGS__)
#define LOGI(...) LOG_SHORT(CONTEXT, " ", __VA_ARGS__)
#define LOGE(...) LOG_SHORT(CONTEXT, " ", __VA_ARGS__)

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

template<class ...T>
std::ostream& operator<<(std::ostream& stream, const std::variant<T...>& variant)
{
    const auto to_string = [](const auto& val)
    {
        return std::to_string(val);
    };
    stream << std::visit(to_string, variant);
    return stream;
}
