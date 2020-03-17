// iocp.cpp
//
// Unit Test: wdl::io::iocp

#include <catch2/catch.hpp>
#include <wdl/io/iocp.hpp>

#include <atomic>
#include <thread>
#include <vector>

constexpr auto N_THREADS      = 10;
constexpr auto OPS_PER_THREAD = 10000;

void producer(wdl::io::iocp& port, std::atomic_long& count)
{
    auto ov = OVERLAPPED{};

    for (auto i = 0ul; i < OPS_PER_THREAD; ++i)
    {
        wdl::io::post_completion(port, i, i, &ov);
        ++count;
    }
}

void consumer(wdl::io::iocp& port, std::atomic_long& count)
{
    auto dummy1      = unsigned long{};
    auto dummy2      = ULONG_PTR{};
    LPOVERLAPPED pov = NULL;

    for (auto i = 0ul; i < OPS_PER_THREAD; ++i)
    {
        wdl::io::get_completion(port, &dummy1, &dummy2, &pov);
        ++count;
    }
}

TEST_CASE("wdl::io::iocp supports concurrent IO operations")
{
    auto port = wdl::io::iocp{};

    REQUIRE(static_cast<bool>(port));

    auto threads = std::vector<std::thread>{};

    auto producer_count = std::atomic_long{};
    auto consumer_count = std::atomic_long{};

    for (auto i = 0u; i < N_THREADS; ++i)
    {
        threads.emplace_back(producer, std::ref(port), std::ref(producer_count));
        threads.emplace_back(consumer, std::ref(port), std::ref(consumer_count));
    }

    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    REQUIRE(producer_count == N_THREADS*OPS_PER_THREAD);
    REQUIRE(consumer_count == N_THREADS*OPS_PER_THREAD);
}