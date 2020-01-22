// auto_lock.h
// Scoped-based lock wrapper.

#include <windows.h>

namespace wdl
{
	// auto_lock
	// RAII-based scoped lock wrappper.

	template <typename T>
	class auto_lock
	{
		auto_lock(T& lock)
			: m_lock{ lock }
		{
			m_lock.enter();
		}

		~auto_lock()
		{
			m_lock.exit();
		}

		auto_lock(const T& other)          = delete;
		auto_lock(T&& other)               = delete;
		auto_lock& operator=(const T& rhs) = delete;
		auto_lock& operator=(T&& rhs)      = delete;

	private:
		T& m_lock;
	};
}