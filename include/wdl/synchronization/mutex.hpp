// mutex.hpp
//
// Class Implementation: wdl::synchronization::mutex
//
// Windows Mutex object wrapper.

#pragma once

#include <windows.h>
#include <string>

#include <wdl/handle.hpp>

namespace wdl::synchronization
{
	// wdl::synchronization::mutex
	//
	// Windows Mutex object wrapper.

	class mutex
	{
		using null_handle_t = wdl::handle::null_handle;

		null_handle_t m_handle;
		bool          m_new_instance;

	public:
		mutex()
			: m_handle{ ::CreateMutexW(nullptr, false, nullptr) }
		{}

		explicit mutex(
			std::wstring const& name, 
			bool const initial_owner = false)
			: m_handle{ ::CreateMutexW(nullptr, initial_owner, name.c_str()) }
			, m_new_instance{ ::GetLastError() != ERROR_ALREADY_EXISTS }
		{}

		// rely on unique_handle semantics for release
		~mutex() = default;

		mutex(mutex const& other)          = delete;
		mutex& operator=(mutex const& rhs) = delete;
		mutex(mutex&& other)               = delete;
		mutex& operator=(mutex&& rhs)      = delete;

		void enter(unsigned long ms = INFINITE)
		{
			::WaitForSingleObject(m_handle.get(), ms);
		}

		void exit()
		{
			::ReleaseMutex(m_handle.get());
		}

		HANDLE get() const noexcept
		{
			return m_handle.get();
		}

		bool is_new_instance() const noexcept
		{
			return m_new_instance;
		}

		bool is_valid() const noexcept
		{
			return static_cast<bool>(m_handle);
		}

		explicit operator bool() const noexcept
		{
			return static_cast<bool>(m_handle);
		}
	};
}