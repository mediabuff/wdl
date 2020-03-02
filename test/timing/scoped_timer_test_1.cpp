// scoped_timer_test_1.cpp
// Unit Test: wdl::timing::scoped_timer
//
// Using the WDL to profile STL smart pointer performance.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL scoped_timer_test_1.cpp

#include <windows.h>

#include <array>
#include <memory>
#include <utility>
#include <iostream>

#include "wdl/timing/timing.hpp"

constexpr auto n_iters = 1000;

struct Vector2
{
	float x;
	float y;
};

int main()
{
	std::cout << "STL Smart Pointer Performance Test\n\n";

	{
		std::array<std::unique_ptr<Vector2>, n_iters> unique{};

		wdl::timing::scoped_timer t{ "std::unique_ptr - constructor" };
		for (int i = 0; i < n_iters; ++i)
		{
			unique[i] = std::unique_ptr<Vector2>{ new Vector2 };
		}
	}

	{
		std::array<std::unique_ptr<Vector2>, n_iters> unique{};

		wdl::timing::scoped_timer t{ "std::unique_ptr - make_unique" };
		for (int i = 0; i < n_iters; ++i)
		{
			unique[i] = std::make_unique<Vector2>();
		}
	}

	{
		std::array<std::shared_ptr<Vector2>, n_iters> shared{};

		wdl::timing::scoped_timer t{ "std::shared_ptr - constructor" };
		for (int i = 0; i < n_iters; ++i)
		{
			shared[i] = std::shared_ptr<Vector2>{ new Vector2 };
		}
	}

	{
		std::array<std::shared_ptr<Vector2>, n_iters> shared{};

		wdl::timing::scoped_timer t{ "std::shared_ptr - make_shared" };
		for (int i = 0; i < n_iters; ++i)
		{
			shared[i] = std::make_shared<Vector2>();
		}
	}
}