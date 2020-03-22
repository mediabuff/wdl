// thread.hpp
//
// Class Implementation: wdl::thread::thread

#pragma once

#include <windows.h>
#include <wdl/handle.hpp>

namespace wdl::thread
{
	using thread_callback_f = void (*)();

	namespace detail
	{
		unsigned long callback_trampoline(void* arg)
		{
			auto callback = static_cast<thread_callback_f>(arg);
			callback();

			return 0;
		}
	}

	template <typename F>
	wdl::handle::null_handle make_thread(F callback) noexcept
	{
		return wdl::handle::null_handle
		{
			::CreateThread(
				nullptr,
				0,
				detail::callback_trampoline,
				static_cast<thread_callback_f>(callback),
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