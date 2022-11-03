#include <iostream>
#include <vector>
#include <chrono>
#include "ThreadPool.h"

int main() {
    std::vector<std::chrono::milliseconds> tasks = {
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(15000), // Задача, которая заблокирует один из потоков
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(15000), // Задача, которая заблокирует другой поток
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
            std::chrono::milliseconds(500),
    };
    mytp::ThreadPool tp(2);
    for (auto task : tasks) {
        tp.submit([task] {
            std::this_thread::sleep_for(task);
            std::string log = "Task with duration " + std::to_string(task.count()) + "ms is done\n";
            std::cout << log;
        });
    }
    tp.start();
    tp.shutdown();
}
