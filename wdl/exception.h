// exception.h
// Custom WDL exception classes.

#include <windows.h>

namespace wdl
{
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