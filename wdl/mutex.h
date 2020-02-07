// mutex.h
// Windows Mutex object wrapper.

#pragma once

#include <windows.h>
#include <string>
#include "exception.h"
#include "unique_handle.h"

namespace wdl
{
	// mutex
	// Windows Mutex object wrapper.

	class mutex
	{
		explicit mutex(const std::wstring& name = nullptr)
			: m_lock{ ::CreateMutexW(nullptr, false, name) }
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
			::WaitForSingleObject(m_lock, ms);
		}

		void exit()
		{
			::ReleaseMutex(m_lock);
		}

		HANDLE get() const noexcept
		{
			return m_lock.get();
		}

	private:
		null_handle m_lock;
	};
}