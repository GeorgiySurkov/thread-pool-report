#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <atomic>

namespace mytp { // mytp stands for my thread pool

    class ThreadPool {
        int _workers_amount;
        bool _should_terminate = false;           // Говорит тредам, что пора завершать работу
        std::atomic<int> _active_threads = 0;     // Количество активных тредов
        std::mutex _queue_mutex;                  // Предотвращает одновременный доступ к очереди
        std::condition_variable _mutex_condition; // Позволяет тредам ждать, пока не появится задача или не придет сигнал о завершении
        std::vector<std::thread> _threads;        // Пул тредов
        std::queue<std::function<void()>> _tasks; // Очередь задач

    public:
        explicit ThreadPool(int workers_amount = -1) : _workers_amount(workers_amount) {
            if (_workers_amount == -1) {
                _workers_amount = (int) std::thread::hardware_concurrency();
            }
        }
        void submit(const std::function<void()>& task);
        void start();
        void shutdown(bool wait = true);
        bool is_busy();

    private:
        void thread_loop();
    };

} // mytp
