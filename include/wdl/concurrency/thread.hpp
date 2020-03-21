// thread.hpp
//
// Class Implementation: wdl::concurrency::thread
//
// Windows threading utilities.

#pragma once

#include <windows.h>
#include <wdl/utility/unique_handle.hpp>

namespace wdl::concurrency
{
	using thread_callback = void (*)();

	using null_handle_t = wdl::utility::null_handle;

	unsigned long thread_wrapper(void* arg)
	{
		auto callback = static_cast<thread_callback>(arg);

		callback();

		return 0;
	}

	template <typename F>
	null_handle_t make_thread(F callback) noexcept
	{
		return null_handle_t
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