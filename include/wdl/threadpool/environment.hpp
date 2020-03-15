// environment.hpp
// Class Implementation: wdl::threadpool::environment

#pragma once

#include <windows.h>

namespace wdl::threadpool
{
    // wdl::threadpool::environment
	//
	// Thread pool environment wrapper.

	class environment
	{
		TP_CALLBACK_ENVIRON m_handle;

	public:
		environment() noexcept
		{
			::InitializeThreadpoolEnvironment(&m_handle);
		}

		~environment() noexcept
		{
			::DestroyThreadpoolEnvironment(&m_handle);
		}

		PTP_CALLBACK_ENVIRON get() noexcept
		{
			return &m_handle;
		}

		environment(const environment& other)          = delete;
		environment(environment&& other)               = delete;
		environment& operator=(const environment& rhs) = delete;
		environment& operator=(environment& rhs)       = delete;
	};
}