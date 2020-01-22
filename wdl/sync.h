// sync.h
// Synchronization utilties.

#pragma once

#include <windows.h>
#include "debug.h"

namespace wdl
{
	// wait_one
	// Simple wrapper around object wait for dispatcher objects.

	template <typename T>
	void wait_one(const T& arg)
	{
		VERIFY_(
			WAIT_OBJECT_0,
			::WaitForSingleObject(arg.get(), INFINITE)
		);
	}

	// wait_all
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
}