// slim_lock.cpp
//
// Unit Test: wdl::synchronization::slim_lock

#include <catch2/catch.hpp>
#include <wdl/synchronization/slim_lock.hpp>

#include <vector>
#include <thread>

constexpr auto N_THREADS             = 5;
constexpr auto ITERATIONS_PER_THREAD = 10000;

void exclusive_worker(
	wdl::synchronization::slim_lock& lock,
	unsigned long long& count
	)
{
	for (auto i = 0u; i < ITERATIONS_PER_THREAD; ++i)
	{
		lock.enter();
		++count;
		lock.exit();
	}
}

TEST_CASE("wdl::synchronization::slim_lock protects shared data reads in exclusive mode")
{
	auto slim_lock = wdl::synchronization::slim_lock{};

	auto count = unsigned long long{};

	auto threads = std::vector<std::thread>{};
	for (auto i = 0u; i < N_THREADS; ++i)
	{
		threads.emplace_back(exclusive_worker, std::ref(slim_lock), std::ref(count));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	REQUIRE(count == N_THREADS*ITERATIONS_PER_THREAD);
}


void exclusive_worker_auto(
	wdl::synchronization::slim_lock& lock,
	unsigned long long& count
	)
{
	for (auto i = 0u; i < ITERATIONS_PER_THREAD; ++i)
	{
		auto guard = lock.get_exclusive();
		++count;
	}
}

TEST_CASE("wdl::synchronization::slim_auto_lock provides scope-based mutual exclusion")
{
	auto slim_lock = wdl::synchronization::slim_lock{};

	auto count = unsigned long long{};

	auto threads = std::vector<std::thread>{};
	for (auto i = 0u; i < N_THREADS; ++i)
	{
		threads.emplace_back(exclusive_worker, std::ref(slim_lock), std::ref(count));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	REQUIRE(count == N_THREADS*ITERATIONS_PER_THREAD);
}

