// thread.h
// Windows threading utilities.

#pragma once

#include <windows.h>
#include "unique_handle.h"

namespace wdl
{
	using thread_callback = void (*)();

	unsigned long thread_wrapper(void* arg)
	{
		auto callback = static_cast<thread_callback>(arg);

		callback();

		return 0;
	}

	template <typename F>
	null_handle make_thread(F callback) noexcept
	{
		return null_handle
		{
			::CreateThread(
				nullptr,
				0,
				thread_wrapper,
				static_cast<thread_callback>(callback),
				0,
				nullptr
				)
		};
	}

	unsigned long thread_id() noexcept
	{
		return ::GetCurrentThreadId();
	}
}