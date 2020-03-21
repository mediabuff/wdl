// mutex.cpp
//
// Unit Test: wdl::synchronization::mutex

#include <catch2/catch.hpp>
#include <wdl/synchronization/mutex.hpp>

#include <thread>
#include <vector>

constexpr auto N_THREADS             = 5;
constexpr auto ITERATIONS_PER_THREAD = 10000;

void work(
    wdl::synchronization::mutex& mutex, 
    unsigned long long& count
    )
{
    for (auto i = 0u; i < ITERATIONS_PER_THREAD; ++i)
    {
        mutex.enter();
        ++count;
        mutex.exit();
    }
}

TEST_CASE("wdl::synchronization::mutex protects shared data access")
{
    auto mutex = wdl::synchronization::mutex{};
    
    // could also use explicit boolean operator
    REQUIRE(mutex.is_valid());

    auto count = unsigned long long{};

    auto threads = std::vector<std::thread>{};
    for (auto i = 0u; i < N_THREADS; ++i)
    {
        threads.emplace_back(work, std::ref(mutex), std::ref(count));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    REQUIRE(count == N_THREADS*ITERATIONS_PER_THREAD);
}