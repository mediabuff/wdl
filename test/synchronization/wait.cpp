// wait.cpp
//
// Unit Test:
//	- wdl::synchronization::wait() 
//	- wdl::synchronization::wait_all()
// 	- wdl::synchronization::wait_handles()
//
// Demonstration of dispatcher object synchronization with WDL.

#include <catch2/catch.hpp>

#include <thread>

#include <wdl/debug/debug.hpp>
#include <wdl/synchronization/wait.hpp>
#include <wdl/utility/unique_handle.hpp>

void async_work(HANDLE h, bool& flag)
{
	flag = true;
	VERIFY(::SetEvent(h));
}

TEST_CASE("wdl::synchronization::wait() supports wait on single Windows handle")
{
	auto handle = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
	REQUIRE(handle != NULL);

	auto flag = bool{false};

	auto thread = std::thread{async_work, handle, std::ref(flag)};

	wdl::synchronization::wait_one(handle);

	REQUIRE(flag);
}