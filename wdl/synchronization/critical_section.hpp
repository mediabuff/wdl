// critical_section.h
// Class Implementation: wdl::synchronization::critical_section
// 
// Windows Critical Section object wrapper

#pragma once

#include <windows.h>

namespace wdl::synchronization
{

	// critical_section
	//
	// Windows Critical Section type wrapper.

	class critical_section
	{

		CRITICAL_SECTION m_lock;

	public:
		critical_section() noexcept
		{
			::InitializeCriticalSection(&m_lock);
		}

		~critical_section() noexcept
		{
			::DeleteCriticalSection(&m_lock);
		}

		critical_section(const critical_section& other)          = delete;
		critical_section(critical_section&& other)               = delete;
		critical_section& operator=(const critical_section& rhs) = delete;
		critical_section& operator=(critical_section&& rhs)      = delete;

		void enter() noexcept
		{
			::EnterCriticalSection(&m_lock);
		}

		void exit() noexcept
		{
			::LeaveCriticalSection(&m_lock);
		}

		LPCRITICAL_SECTION native_handle() const noexcept
		{
			return &m_lock;
		}
	};
}