// win32_exception.hpp
//
// Class Implementation: wdl::error::win32_exception

#pragma once

#include <windows.h>

namespace wdl::error
{
	// wdl::error::win32_exception

	class win32_exception
	{
		unsigned long m_code;

	public:
		explicit win32_exception(
			unsigned long const code = ::GetLastError()) 
			: m_code{ code }
		{}

		unsigned long code() const noexcept
		{
			return m_code;
		}
	};
}