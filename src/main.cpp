#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include "include/LruCache.hpp"
#include "include/LfuCache.hpp"

class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    double elapsed() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start_).count();
    }
};

void run_benchmark() {
    const size_t OPS = 2'000'000;
    const size_t CAP = 50'000;
    const size_t RANGE = 100'000;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, RANGE);

    std::cout << "Running Benchmark (" << OPS << " ops)...\n";

    {
        LruCache<int, int> lru(CAP);
        Timer t;
        for (size_t i = 0; i < OPS; ++i) {
            int k = dist(rng);
            if (i % 2 == 0) lru.get(k);
            else lru.put(k, i);
        }
        std::cout << "[LRU] " << std::fixed << std::setprecision(2) 
                  << (OPS / t.elapsed() / 1e6) << " M ops/sec\n";
    }

    {
        LfuCache<int, int> lfu(CAP);
        Timer t;
        for (size_t i = 0; i < OPS; ++i) {
            int k = dist(rng);
            if (i % 2 == 0) lfu.get(k);
            else lfu.put(k, i);
        }
        std::cout << "[LFU] " << std::fixed << std::setprecision(2) 
                  << (OPS / t.elapsed() / 1e6) << " M ops/sec\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    run_benchmark();
    return 0;
}
