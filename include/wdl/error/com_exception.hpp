// com_exception.hpp
//
// Class Implementation: wdl::error::com_exception

#pragma once

#include <windows.h>

namespace wdl::error
{
	// wdl::error::com_exception

	class com_exception
	{
		HRESULT m_code;

	public:
		explicit com_exception(HRESULT const code) 
			: m_code{ code }
		{}

        HRESULT code() const noexcept
        {
            return m_code;
        }
	};

	// wdl::error::check_com()

	void check_com(HRESULT const result)
	{
		if (S_OK != result)
		{
			throw com_exception{ result };
		}
	}
}