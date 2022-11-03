
#include "ThreadPool.h"

namespace mytp {

    void ThreadPool::submit(const std::function<void()> &task) {
        {
            std::unique_lock<std::mutex> lock(_queue_mutex);
            if (_should_terminate) {
                return;
            }
            _tasks.push(task);
        }
        _mutex_condition.notify_one();
    }

    void ThreadPool::start() {
        _threads.reserve(_workers_amount);
        for (uint32_t i = 0; i < _workers_amount; i++) {
            _threads.emplace_back(&ThreadPool::thread_loop, this);
        }
    }

    void ThreadPool::shutdown(bool wait) {
        {
            std::unique_lock<std::mutex> lock(_queue_mutex);
            _should_terminate = true;
            if (!wait) {
                std::queue<std::function<void()>> empty;
                std::swap(_tasks, empty);
            }
        }
        _mutex_condition.notify_all();
        for (std::thread& active_thread : _threads) {
            active_thread.join();
        }
        _threads.clear();
    }

    bool ThreadPool::is_busy() {
        return _active_threads > 0;
    }

    void ThreadPool::thread_loop() {
        while (true) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(_queue_mutex);
                _mutex_condition.wait(lock, [this] {
                    return !_tasks.empty() || _should_terminate;
                });
                if (_should_terminate && _tasks.empty()) {
                    return;
                }
                job = _tasks.front();
                _tasks.pop();
            }
            ++_active_threads;
            job();
            --_active_threads;
        }
    }
} // mytp
