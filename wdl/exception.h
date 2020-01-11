// exception.h
// Custom WDL exception classes.

#include <stdexcept>

namespace wdl
{
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