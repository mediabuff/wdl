// wait.cpp
//
// Unit Test:
//	- wdl::synchronization::wait() 
//	- wdl::synchronization::wait_all()
// 	- wdl::synchronization::wait_handles()
//
// Demonstration of dispatcher object synchronization with WDL.

#include <catch2/catch.hpp>

#include <array>
#include <vector>
#include <thread>
#include <algorithm>

#include <wdl/synchronization/wait.hpp>
#include <wdl/synchronization/event.hpp>

constexpr auto N_HANDLES = 5;

void async_work_1(HANDLE h, bool& flag)
{
	flag = true;
	::SetEvent(h);
}

void async_work_2(
	wdl::synchronization::event& e, 
	bool& flag
	)
{
	flag = true;
	e.set();
}

TEST_CASE("wdl::synchronization::wait_one() supports wait on a single Windows handle")
{
	auto flag = bool{false};

	auto h = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
	REQUIRE(h != NULL);

	auto t = std::thread{async_work_1, h, std::ref(flag)};

	wdl::synchronization::wait_one(h);

	REQUIRE(flag);

	t.join();
}


TEST_CASE("wdl::synchronization::wait() supports wait on single WDL smart class")
{
	using wdl::synchronization::event;
	using wdl::synchronization::event_type;

	auto flag = bool{false};

	auto e = event{event_type::manual_reset};
	REQUIRE(e.is_valid());

	auto t = std::thread{async_work_2, std::ref(e), std::ref(flag)};

	// wait on the event
	wdl::synchronization::wait_one(e);

	REQUIRE(flag);

	t.join();
}

TEST_CASE("wdl::synchronization::wait_handles() supports waiting on array of Windows handles")
{
	auto handles = std::array<HANDLE, N_HANDLES>{};
	auto flags   = std::array<bool, N_HANDLES>{};
	auto threads = std::vector<std::thread>{};

	for (auto i = 0u; i < N_HANDLES; ++i)
	{
		handles[i] = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
		REQUIRE(handles[i] != NULL);

		flags[i] = false;  // not necessary

		threads.emplace_back(async_work_1, handles[i], std::ref(flags[i]));
	}

	wdl::synchronization::wait_handles(handles.data(), N_HANDLES);

	REQUIRE(
		std::all_of(flags.begin(), flags.end(), [](bool f){ return f; })
		);

	for (auto& t : threads)
	{
		t.join();
	}
}

TEST_CASE("wdl::synchronization::wait_all() supports waiting on variadic number of WDL smart class")
{
	using wdl::synchronization::event;
	using wdl::synchronization::event_type;

	auto flag1 = bool{false};
	auto flag2 = bool{false};

	auto e1 = event{event_type::manual_reset};
	auto e2 = event{event_type::manual_reset};
	REQUIRE(e1.is_valid());
	REQUIRE(e1.is_valid());

	auto t1 = std::thread{async_work_2, std::ref(e1), std::ref(flag1)};
	auto t2 = std::thread{async_work_2, std::ref(e2), std::ref(flag2)};

	wdl::synchronization::wait_all(e1, e2);

	REQUIRE((flag1 && flag2));

	t1.join();
	t2.join();
}