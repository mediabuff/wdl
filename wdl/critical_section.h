// critical_section.h
// Windows Critical Section object wrapper

#include <windows.h>

namespace wdl
{

	// critical_section
	// Windows Critical Section type wrapper.

	class critical_section
	{
	public:
		critical_section() noexcept
		{
			::InitializeCriticalSection(&m_lock);
		}

		~critical_section() noexcept
		{
			::DeleteCriticalSection(&m_lock);
		}

		critical_section(const critical_section& other)          = delete;
		critical_section(critical_section&& other)               = delete;
		critical_section& operator=(const critical_section& rhs) = delete;
		critical_section& operator=(critical_section&& rhs)      = delete;

		void enter() noexcept
		{
			::EnterCriticalSection(&m_lock);
		}

		void exit() noexcept
		{
			::LeaveCriticalSection(&m_lock);
		}

		CRITICAL_SECTION* native_handle() const noexcept
		{
			return &m_lock;
		}

	private:
		CRITICAL_SECTION m_lock;
	};
}