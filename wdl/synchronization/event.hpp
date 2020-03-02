// event.h
// Class Implementation: wdl::synchronization::event
//
// Windows Event object wrapper.

#pragma once

#include <windows.h>

#include "wdl/debug/debug.hpp"
#include "wdl/utility/exception.hpp"
#include "wdl/utility/unique_handle.hpp"

using wdl::utility::null_handle;
using wdl::utility::windows_exception;

namespace wdl::synchronization
{
	// event_type
	//
	// Windows event type enumeration.

	enum class event_type
	{
		auto_reset,
		manual_reset
	};

	// event
	//
	// WDL wrapper around Windows events implementing RAII.

	class event
	{
	public:
		explicit event(event_type type)
			: handle{ ::CreateEvent(nullptr, static_cast<bool>(type), false, nullptr) }
		{
			if (!handle)
			{
				throw windows_exception{};
			}
		}

		// rely on destructor for null_handle to clean up
		~event() = default;

		event(const event&)            = delete;
		event& operator=(const event&) = delete;

		event(event&& other) noexcept
			: handle{ other.handle.release() }
		{}

		event& operator=(event&& rhs) noexcept
		{
			if (this != &rhs)
			{
				handle = std::move(rhs.handle);
			}
			
			return *this;
		}

		void set() noexcept
		{
			VERIFY(::SetEvent(handle.get()));
		}

		void reset() noexcept
		{
			VERIFY(::ResetEvent(handle.get()));
		}

		bool wait(const unsigned long ms = INFINITE) noexcept
		{
			const auto res = ::WaitForSingleObject(handle.get(), ms);

			ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT);

			return res == WAIT_OBJECT_0;
		}

		HANDLE get() const noexcept
		{
			return handle.get();
		}

	private:
		null_handle handle;
	};
}