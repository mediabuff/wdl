// mutex.h
// Class Implementation: wdl::synchronization::mutex
//
// Windows Mutex object wrapper.

#pragma once

#include <windows.h>
#include <string>

#include "wdl/utility/exception.hpp"
#include "wdl/utility/unique_handle.hpp"

using wdl::utility::null_handle;
using wdl::utility::windows_exception;

namespace wdl::synchronization
{
	// mutex
	//
	// Windows Mutex object wrapper.

	class mutex
	{
		mutex()
			: m_lock{ ::CreateMutexW(nullptr, false, nullptr) }
		{
			if (!m_lock)
			{
				throw windows_exception{};
			}
		}

		explicit mutex(const std::wstring& name)
			: m_lock{ ::CreateMutexW(nullptr, false, name.c_str()) }
		{
			if (!m_lock)
			{
				throw windows_exception();
			}
		}

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

	private:
		null_handle m_lock;
	};
}