cmake_minimum_required(VERSION 3.22)
project(cpp_custom_implementation)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
include(ExternalProject)

# Include google test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(
        threadpool_test
        test_threadpool.cpp
        threadpool.h
        threadpool.cpp
)
target_link_libraries(
        threadpool_test
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(threadpool_test)

add_executable(
        blocking_thread_demo
        blocking_thread_demo.cpp
        ThreadPool.h
        ThreadPool.cpp
)
add_executable(
        calculate_factorial
        calculate_factorial.cpp
        biginteger/biginteger.h
        biginteger/biginteger.cpp
        biginteger/helpers.h
        biginteger/helpers.cpp
        biginteger/Vector.h
)
add_executable(
        calculate_factorial_multithreaded
        calculate_factorial_multithreaded.cpp
        threadpool.h
        threadpool.cpp
        biginteger/biginteger.h
        biginteger/biginteger.cpp
        biginteger/helpers.h
        biginteger/helpers.cpp
        biginteger/Vector.h
)
