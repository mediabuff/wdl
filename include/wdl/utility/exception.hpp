// exception.h
// Class Implementation: wdl::utility::windows_exception
//
// Custom WDL exception class.

#pragma once

#include <windows.h>

namespace wdl::utility
{
	// windows_exception

	class windows_exception
	{
	public:
		explicit windows_exception(
			unsigned long error = ::GetLastError()
		) : error{ error }
		{}

	private:
		unsigned long error;
	};
}