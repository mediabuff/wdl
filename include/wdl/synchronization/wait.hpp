// wait.hpp
//
// Free Function Implementations:
//	- wdl::synchronization::wait_one()
//	- wdl::synchronization::wait_all()
//	- wdl::synchronization::wait_array()
//
// Synchronization utilties.

#pragma once

#include <windows.h>
#include <iostream>
#include <wdl/debug.hpp>

namespace wdl::synchronization
{
	namespace detail
	{
		void pack(HANDLE*) {}

		template <typename T, typename... Args>
		void pack(
			HANDLE*        left,
			T const&       right,
			Args const&... args
		)
		{
			*left = right.get();
			pack(++left, args...);
		}
	}

	// wdl::synchronization::wait_one()
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_one(
		HANDLE const handle, 
		unsigned long const timeout = INFINITE
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(handle, timeout)
		);
	}

	// wdl::synchronization::wait_one()
	//
	// Overload that accepts WDL synchronization types.

	template <typename T>
	void wait_one(
		T const& arg, 
		unsigned long const timeout = INFINITE
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(arg.get(), timeout)
		);
	}

	// wdl::synchronization::wait_all()
	//
	// Variadic template for simplifying dispatcher object waits.

	template <typename... Args>
	void wait_all(Args const&... args)
	{
		HANDLE handles[sizeof...(Args)];

		// pack dispatcher object handles into array
		detail::pack(handles, args...);

		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForMultipleObjects(sizeof...(Args), handles, true, INFINITE)
		);
	}

	// wdl::synchronization::wait_handles()
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_handles(
		HANDLE* const handles, 
		unsigned long const size
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForMultipleObjects(size, handles, true, INFINITE)
		);
	}
}