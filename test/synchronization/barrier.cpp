// barrier.cpp
//
// Unit Test: wdl::synchronization::barrier

#include <catch2/catch.hpp>
#include <wdl/synchronization/barrier.hpp>

#include <thread>
#include <atomic>
#include <vector>

constexpr auto N_THREADS = 10;

void worker(
    wdl::synchronization::barrier& barrier,
    std::atomic_long& count1,
    std::atomic_long& count2
    )
{
    ++count1;
    barrier.enter();

    ++count2;
    barrier.enter();
}

TEST_CASE("wdl::synchronization::barrier synchronizes thread activity")
{   
    auto count1  = std::atomic_long{};
    auto count2  = std::atomic_long{};

    auto barrier = wdl::synchronization::barrier{N_THREADS+1};

    auto threads = std::vector<std::thread>{};
    for (unsigned long id = 0; id < N_THREADS; ++id)
    {
        threads.emplace_back(
            worker, 
            std::ref(barrier), 
            std::ref(count1), 
            std::ref(count2)
            );
    }

    barrier.enter();
    REQUIRE(count1.load() == N_THREADS);

    barrier.enter();
    REQUIRE(count2.load() == N_THREADS);

    for (auto& t : threads)
    {
        t.join();
    }
}