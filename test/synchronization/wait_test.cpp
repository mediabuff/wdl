// wait_test.cpp
// Unit Test: wdl::synchronization::wait_all
//
// Demonstration of dispatcher object synchronization with WDL.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL wait_test.cpp

#include <windows.h>
#include <process.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <sstream>

#include "wdl/synchronization/wait.hpp"
#include "wdl/utility/unique_handle.hpp"

constexpr auto STATUS_SUCCESS_I = 0x0;
constexpr auto STATUS_FAILURE_I = 0x1;

unsigned int __stdcall work()
{
	using namespace std::chrono_literals;

	const auto tid = ::GetCurrentThreadId();

	printf("[%u] Enter\n", tid);

	std::this_thread::sleep_for(2s);

	printf("[%u] Exit\n", tid);

	return STATUS_SUCCESS_I;
}

int main()
{
	auto t1 = wdl::utility::null_handle
	{
		reinterpret_cast<HANDLE>(
			_beginthreadex(nullptr, 0, work, nullptr, 0, nullptr)
		)
	};

	auto t2 = wdl::utility::null_handle
	{
		reinterpret_cast<HANDLE>(
			_beginthreadex(nullptr, 0, work, nullptr, 0, nullptr)
		)
	};

	wdl::synchronization::wait_all(t1, t2);

	return STATUS_SUCCESS_I;
}