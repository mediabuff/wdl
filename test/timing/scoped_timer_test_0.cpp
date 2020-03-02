// SimplePerfTest.cpp
// Unit Test: wdl::timing::scoped_timer
//
// Simple performance test with WDL.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL scoped_timer_test_0.cpp

#include <thread>
#include <chrono>

#include "wdl/timing/timing.hpp"

void spin()
{
	using namespace std::chrono_literals;

	wdl::timing::scoped_timer t{__func__};

	std::this_thread::sleep_for(3s);
}

int main()
{
	spin();
}