// mutex.hpp
//
// Class Implementation: wdl::synchronization::mutex
//
// Windows Mutex object wrapper.

#pragma once

#include <windows.h>
#include <string>

#include <wdl/utility/exception.hpp>
#include <wdl/utility/unique_handle.hpp>

using wdl::utility::null_handle;
using wdl::utility::windows_exception;

namespace wdl::synchronization
{
	// wdl::synchronization::mutex
	//
	// Windows Mutex object wrapper.

	class mutex
	{
		using null_handle_t = wdl::utility::null_handle;

		null_handle_t m_handle;

	public:
		mutex()
			: m_lock{ ::CreateMutexW(nullptr, false, nullptr) }
		{}

		explicit mutex(const std::wstring& name)
			: m_lock{ ::CreateMutexW(nullptr, false, name.c_str()) }
		{}

		// rely on unique_handle semantics for release
		~mutex() = default;

		mutex(const mutex& other)          = delete;
		mutex(mutex&& other)               = delete;
		mutex& operator=(const mutex& rhs) = delete;
		mutex& operator=(mutex&& rhs)      = delete;

		void enter(unsigned long ms = INFINITE)
		{
			::WaitForSingleObject(m_lock.get(), ms);
		}

		void exit()
		{
			::ReleaseMutex(m_lock.get());
		}

		HANDLE get() const noexcept
		{
			return m_lock.get();
		}
	};
}