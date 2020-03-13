// thread_pool.h
// Class Implementation: wdl::threadpool::tp_environment
//
// Windows Thread Pool utilties.

#pragma once

#include <windows.h>

namespace wdl::threadpool
{
	// tp_environment
	//
	// Thread pool environment wrapper.

	class tp_environment
	{
	public:
		tp_environment() noexcept
		{
			::InitializeThreadpoolEnvironment(&m_handle);
		}

		~tp_environment() noexcept
		{
			::DestroyThreadpoolEnvironment(&m_handle);
		}

		PTP_CALLBACK_ENVIRON get() noexcept
		{
			return &m_handle;
		}

		tp_environment(const tp_environment& other)          = delete;
		tp_environment(tp_environment&& other)               = delete;
		tp_environment& operator=(const tp_environment& rhs) = delete;
		tp_environment& operator=(tp_environment& rhs)       = delete;

	private:
		TP_CALLBACK_ENVIRON m_handle;
	};
}