cmake_minimum_required(VERSION 3.5)

project(tech-task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

#
# ===== IEEE library ========
#

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    ieee754
    GIT_REPOSITORY "https://github.com/kkimdev/ieee754-types.git"
    GIT_TAG master
    GIT_PROGRESS TRUE
#    CONFIGURE_COMMAND ""  # tell CMake it's not a cmake project
)

FetchContent_MakeAvailable(ieee754)

add_library(ieee754 INTERFACE)
target_include_directories(ieee754 INTERFACE ${ieee754_SOURCE_DIR})

#
# ===== Boost library ========
#

set(BOOST_REQD_SUBMODULES
    "tools/cmake;"
    "libs/assert;libs/exception;libs/throw_exception;libs/static_assert;"
    "libs/bind;libs/function_types;libs/function;libs/functional;"
    "libs/chrono;libs/date_time;"
    "libs/concept_check;"
    "libs/config;libs/container;libs/container_hash;"
    "libs/iterator;libs/utility;libs/type_traits;libs/algorithm;libs/conversion;libs/numeric/conversion;libs/regex;libs/unordered;libs/tokenizer;libs/move;libs/ratio;libs/lexical_cast;"
    "libs/tuple;libs/variant2;libs/typeof;libs/detail;libs/array;libs/type_index;libs/range;libs/optional;libs/smart_ptr;libs/integer;libs/rational;"
    "libs/intrusive;libs/io;"
    "libs/core;libs/align;libs/predef;libs/preprocessor;libs/system;libs/winapi;libs/atomic;"
    "libs/mpl;libs/fusion;libs/mp11;"
    "libs/thread"
)

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    boost
    GIT_REPOSITORY "https://github.com/boostorg/boost.git"
    GIT_TAG master
    GIT_SUBMODULES ${BOOST_REQD_SUBMODULES}
    GIT_PROGRESS TRUE
    CONFIGURE_COMMAND ""  # tell CMake it's not a cmake project
)

FetchContent_MakeAvailable(boost)

#
# ===== Signals provider library ========
#

add_library(signals_provider STATIC
    print.h
    print.cpp
    asynctaskprocessor.h
    asynctaskprocessor.cpp
    idatasource.h
    compiler_ext.h
    rawframe.h
    framereader.h
    framereader.cpp
    frameprocessor.h
    frameprocessor.cpp
    isignalreceiver.h
    signalprovider.h
    signalprovider.cpp
    converter.h
    converter.cpp
    signalstorage.h
    signalstorage.cpp
    signallogger.h
    signallogger.cpp
)
target_link_libraries(signals_provider PUBLIC Boost::thread ieee754)
target_include_directories(signals_provider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# ===== Simulation ========
#

add_executable(simulation
    main.cpp

    testdatasource.h
    testdatasource.cpp
)
target_link_libraries(simulation signals_provider)

