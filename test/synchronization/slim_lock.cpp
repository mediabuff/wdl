// slim_lock.cpp
// Unit Test: wdl::synchronization::slim_lock
//
// Demonstration of WDL library use with Windows SRW locks.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL slim_lock_test.cpp

#include <windows.h>
#include <cstdio>
#include <thread>
#include <chrono>

#include "wdl/concurrency/thread.hpp"
#include "wdl/synchronization/wait.hpp"
#include "wdl/synchronization/slim_lock.hpp"

auto g_lock   = wdl::synchronization::slim_lock{};
auto g_shared = int{};

void reader() noexcept
{
	using namespace std::chrono_literals;

	for (;;)
	{
		std::this_thread::sleep_for(10ms);

		auto guard = g_lock.get_shared();

		std::this_thread::sleep_for(500ms);

		if (g_shared < 5)
		{
			printf("shared=%d; id=%u\n", g_shared, wdl::concurrency::thread_id());
		}
		else
		{
			break;
		}
	}
}

int main()
{
	using namespace std::chrono_literals;

	auto r1 = wdl::concurrency::make_thread(reader);
	auto r2 = wdl::concurrency::make_thread(reader);

	auto w = wdl::concurrency::make_thread([]
	{
		for (unsigned i = 0; i < 5; ++i)
		{
			std::this_thread::sleep_for(1s);
			auto gaurd = g_lock.get_exclusive();

			++g_shared;
		}
	});

	wdl::synchronization::wait_all(r1, r2, w);
}

