#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include "ThreadPool.h"

TEST(ThreadPool, BasicExample) {
    bool worked = false;
    mytp::ThreadPool tp;
    tp.start();
    tp.submit([&worked] { worked = true; });
    tp.shutdown();
    ASSERT_TRUE(worked);
}

TEST(ThreadPool, DoesntWorkBeforeStart) {
    bool worked = false;
    mytp::ThreadPool tp;
    tp.submit([&worked] { worked = true; });
    tp.shutdown();
    ASSERT_FALSE(worked);
}

TEST(ThreadPool, MultipleJobs) {
    std::atomic<int> counter = 0;
    mytp::ThreadPool tp;
    tp.start();
    for (int i = 0; i < 100; ++i) {
        tp.submit([&counter] { counter++; });
    }
    tp.shutdown();
    ASSERT_EQ(counter, 100);
}

TEST(ThreadPool, WorksWithMoreTasksThanThreads) {
    std::atomic<int> counter = 0;
    mytp::ThreadPool tp(1);
    tp.start();
    for (int i = 0; i < 100; ++i) {
        tp.submit([&counter] { counter++; });
    }
    tp.shutdown();
    ASSERT_EQ(counter, 100);
}

TEST(ThreadPool, WorksWithMoreThreadsThanTasks) {
    std::atomic<int> counter = 0;
    mytp::ThreadPool tp(100);
    tp.start();
    for (int i = 0; i < 1; ++i) {
        tp.submit([&counter] { counter++; });
    }
    tp.shutdown();
    ASSERT_EQ(counter, 1);
}

TEST(ThreadPool, BusyReturnsTrueWhenAtLeastOneThreadIsWorking) {
    mytp::ThreadPool tp(8);
    tp.start();
    tp.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds(50)); });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_TRUE(tp.is_busy());
    tp.shutdown();
    ASSERT_FALSE(tp.is_busy());
}

TEST(ThreadPool, BusyReturnsFalseWhenNoThreadsAreWorking) {
    mytp::ThreadPool tp(8);
    tp.start();
    tp.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_FALSE(tp.is_busy());
    tp.shutdown();
}

TEST(ThreadPool, BusyReturnsTrueWhenAllThreadsAreWorking) {
    mytp::ThreadPool tp(8);
    tp.start();
    for (int i = 0; i < 16; ++i) {
        tp.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_TRUE(tp.is_busy());
    tp.shutdown();
}
