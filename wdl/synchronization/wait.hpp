// wait.h
// Free Function Implementations:
//	wait_one()
//	wait_all()
//	wait_array()
//
// Synchronization utilties.

#pragma once

#include <windows.h>

#include "wdl/debug/debug.hpp"

namespace wdl::synchronization
{
	// wait_one
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_one(
		const HANDLE handle, 
		const unsigned long timeout = INFINITE
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(handle, timeout)
		);
	}

	template <typename T>
	void wait_one(
		const T& arg, 
		const unsigned long timeout = INFINITE
		)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(arg.get(), timeout)
		);
	}

	// wait_all
	//
	// Variadic template for simplifying dispatcher object waits.

	void pack(HANDLE*) {}

	template <typename T, typename... Args>
	void pack(
		HANDLE* left,
		const T& right,
		const Args&... args
	)
	{
		*left = right.get();
		pack(++left, args...);
	}

	template <typename... Args>
	void wait_all(const Args&... args)
	{
		HANDLE handles[sizeof...(Args)];

		// pack dispatcher object handles into array
		pack(handles, args...);

		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForMultipleObjects(sizeof...(Args), handles, true, INFINITE)
		);
	}

	// wait_array
	//
	// Simple wrapper around object wait for dispatcher objects.

	void wait_array(const HANDLE handles[], const unsigned size)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForMultipleObjects(size, handles, true, INFINITE)
		);
	}
}