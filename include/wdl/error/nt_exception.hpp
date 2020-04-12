// nt_exception.hpp
//
// Class Implementation: wdl::error::nt_exception

#pragma once

#include <windows.h>

namespace wdl::error
{
	// wdl::error::nt_exception

	class nt_exception
	{
		NTSTATUS m_code;

	public:
		explicit nt_exception(NTSTATUS const code) 
			: m_code{ code }
		{}

        NTSTATUS code() const noexcept
        {
            return m_code;
        }
	};

	// wdl::error::check_nt()

	void check_nt(NTSTATUS const status)
	{
		if (ERROR_SUCCESS != status)
		{
			throw nt_exception{ status };
		}
	}
}