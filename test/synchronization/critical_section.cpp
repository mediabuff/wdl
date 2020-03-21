// critical_section.cpp
//
// Unit Test: wdl::synchronization::critical_section

#include <catch2/catch.hpp>
#include <wdl/synchronization/critical_section.hpp>

#include <thread>
#include <vector>

constexpr auto N_THREADS             = 5;
constexpr auto ITERATIONS_PER_THREAD = 10000;

void worker(
    wdl::synchronization::critical_section& critical_section, 
    unsigned long long& count
    )
{
    for (auto i = 0u; i < ITERATIONS_PER_THREAD; ++i)
    {
        critical_section.enter();
        ++count;
        critical_section.exit();
    }
}

TEST_CASE("wdl::synchronization::critical_section protects shared data access")
{
    auto critical_section = wdl::synchronization::critical_section{};

    // no need to check for successful initialization; not a kernel object

    auto count = unsigned long long{};

    auto threads = std::vector<std::thread>{};
    for (auto i = 0u; i < N_THREADS; ++i)
    {
        threads.emplace_back(worker, std::ref(critical_section), std::ref(count));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    REQUIRE(count == N_THREADS*ITERATIONS_PER_THREAD);
}