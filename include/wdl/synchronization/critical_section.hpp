// critical_section.hpp
//
// Class Implementation: wdl::synchronization::critical_section

#pragma once

#include <windows.h>

namespace wdl::synchronization
{

	// wdl::synchronization::critical_section
	//
	// Windows Critical Section type wrapper.

	class critical_section
	{

		CRITICAL_SECTION m_handle;

	public:
		critical_section() noexcept
		{
			::InitializeCriticalSection(&m_handle);
		}

		critical_section(unsigned long spin_count) noexcept
		{
			::InitializeCriticalSectionAndSpinCount(&m_handle, spin_count);
		}

		~critical_section() noexcept
		{
			::DeleteCriticalSection(&m_handle);
		}

		critical_section(const critical_section& other)          = delete;
		critical_section& operator=(const critical_section& rhs) = delete;
		critical_section(critical_section&& other)               = delete;
		critical_section& operator=(critical_section&& rhs)      = delete;

		void enter() noexcept
		{
			::EnterCriticalSection(&m_handle);
		}

		void exit() noexcept
		{
			::LeaveCriticalSection(&m_handle);
		}

		LPCRITICAL_SECTION native_handle() noexcept
		{
			return &m_handle;
		}
	};
}