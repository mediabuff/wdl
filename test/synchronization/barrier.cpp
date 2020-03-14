// barrier.cpp
// Unit Test: wdl::synchronization::barrier
//
// Build
//  cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL barrier_test.cpp

#include <thread>
#include <vector>
#include <chrono>
#include <cstdio>

#include "wdl/synchronization/barrier.hpp"

constexpr auto N_THREADS = 10;

wdl::synchronization::barrier g_barrier{N_THREADS};

void worker(unsigned long id)
{
    using namespace std::chrono_literals;

    printf("[%u] Begin Phase 1\n", id);

    std::this_thread::sleep_for(3s);

    printf("[%u] End Phase 1\n", id);

    g_barrier.enter();

    printf("[%u] Begin Phase 2\n", id);

    std::this_thread::sleep_for(2s);

    printf("[%u] End Phase 2\n", id);

    g_barrier.enter();

    printf("[%u] Exit\n", id);
}

int main()
{
    std::vector<std::thread> threads{};

    for (unsigned long id = 0; id < N_THREADS; ++id)
    {
        threads.emplace_back(worker, id);
    }

    for (auto& t : threads)
    {
        t.join();
    }
}