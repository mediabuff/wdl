// event.h
// Windows Event object wrapper.

#pragma once

#include <windows.h>

#include "debug.h"
#include "exception.h"
#include "unique_handle.h"

namespace wdl
{
	// event_type
	// Windows event type enumeration.

	enum class event_type
	{
		auto_reset,
		manual_reset
	};

	// event
	// WDL wrapper around Windows events implementing RAII.

	class event
	{
		explicit event(event::type type)
			: h{ ::CreateEvent(nullptr, static_cast<bool>(type), false, nullptr) }
		{
			if (!h)
			{
				throw windows_exception();
			}
		}

		// rely on destructor for null_handle to clean up
		~event() = default;

		event(const event&)            = delete;
		event& operator=(const event&) = delete;

		event(event&& other) noexcept
			: handle{ other.h.release() }
		{}

		event& operator=(event&& rhs) noexcept
		{
			if (this != rhs)
			{
				h = std::move(rhs.h);
			}
		}

		void set() noexcept
		{
			VERIFY(::SetEvent(h.get()));
		}

		void reset() noexcept
		{
			VERIFY(::ResetEvent(h.get()));
		}

		bool wait(const unsigned long ms = INFINITE) noexcept
		{
			const auto res = ::WaitForSingleObject(h.get(), ms);

			ASSERT(res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT);

			return res == WAIT_OBJECT_0;
		}

		HANDLE get() const noexcept
		{
			return h.get();
		}

	private:
		null_handle h;
	};
}