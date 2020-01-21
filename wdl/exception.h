// exception.h
// Custom WDL exception classes.

#include <windows.h>
#include <stdexcept>

namespace wdl
{
	class windows_exception
	{
	public:
		explicit windows_exception(
			unsigned long error = ::GetLastError()
		) : error{ error };
		{}

	private:
		unsigned long error;
	};

	class wdlexception : public std::runtime_error
	{
	public:
		explicit wdlexception(
			const std::string& what_arg, 
			unsigned long      api_errno
		)
			: std::runtime_error(what_arg),
			api_errno{api_errno}
		{
		}
	private:
		unsigned long api_errno;
	};
}