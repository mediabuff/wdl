// spin_counts.cpp
// Performance Test: wdl::synchronization::critical_section spin count
//
// Simple performance test of critical section with varying 
// spin counts under a low level of contention.
//
// Build
//  cl /EHsc /nologo /W4 /std:c++17 /I C:\Dev\WDL spin_counts.cpp

#include <array>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>
#include <iomanip>
#include <iostream>

#include "wdl/synchronization/critical_section.hpp"

constexpr auto N_THREADS = 5;
constexpr auto N_ITERS   = 10000;

unsigned long long g_data = 0;

void worker(wdl::synchronization::critical_section& cs)
{
    // lazy way to help get more threads prepared before first begins running
    std::this_thread::yield();

    for (auto i = 0; i < N_ITERS; ++i)
    {
        cs.enter();
        g_data++;
        cs.exit();
    }
}

std::chrono::milliseconds 
run_test_with_spin_count(unsigned long spin_count)
{
    std::vector<std::thread> threads{};
    threads.reserve(N_THREADS);

    wdl::synchronization::critical_section cs{spin_count};

    auto start = std::chrono::high_resolution_clock::now();

    for (auto i = 0; i < N_THREADS; ++i)
    {
        threads.emplace_back(worker, std::ref(cs));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    assert(g_data == N_THREADS*N_ITERS);
    g_data = 0;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = stop - start;

    return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
}

int main()
{
    std::array<unsigned long, 4> counts{100, 1000, 10000, 100000};
    for (auto const& c : counts)
    {
        auto ms = run_test_with_spin_count(c);
        
        std::cout << "Spin Count: " << std::setw(6) << c;
        std::cout << " Duration: " << ms.count() << "ms";
        std::cout << std::endl;
    }
}