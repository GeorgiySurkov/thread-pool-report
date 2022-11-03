#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <mutex>
#include "biginteger/biginteger.h"
#include "ThreadPool.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }
    int limit = std::stoi(argv[1]);
    std::queue<BigInteger> numbers_to_multiply;
    std::mutex queue_mutex;
    for (int i = 1; i <= limit; ++i) {
        numbers_to_multiply.push(i);
    }
    mytp::ThreadPool tp;
    tp.start();
    for (int i = 0; i < limit - 1; ++i) {
        tp.submit([&numbers_to_multiply, &queue_mutex] {
            BigInteger first, second;
            while (true) {
                queue_mutex.lock();
                if (numbers_to_multiply.size() < 2) {
                    queue_mutex.unlock();
                    continue;
                }
                first = std::move(numbers_to_multiply.front());
                numbers_to_multiply.pop();
                second = std::move(numbers_to_multiply.front());
                numbers_to_multiply.pop();
                queue_mutex.unlock();
                break;
            }
            BigInteger result = first * second;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                numbers_to_multiply.push(std::move(result));
            }
        });
    }
    tp.shutdown();
}