// pool.hpp

#pragma once

#include <windows.h>

#include "environment.hpp"
#include <wdl/utility/unique_handle.hpp>

namespace wdl::threadpool
{
	// wdl::threadpool::pool
    //
    // Wrapper around user-controlled threadpool.

	class pool
	{
        environment               m_environment;
		wdl::utility::pool_handle m_handle;

        using stateless_function = void(*)();

	public:
		pool()
			: m_handle{ ::CreateThreadpool(nullptr) }
		{
            ::SetThreadpoolCallbackPool(m_environment.get(), m_handle.get());
        }

		// rely on unique_handle for release
		~pool() = default;

        template <typename Callback>
        bool submit(Callback work)
        {
            return ::TrySubmitThreadpoolCallback(
                callback_wrapper, 
                static_cast<stateless_function>(work), 
                m_environment.get()
                );
        }

		void set_max_threadcount(unsigned long count)
		{
			::SetThreadpoolThreadMaximum(m_handle.get(), count);
		}

		bool set_min_threadcount(unsigned long count)
		{
			return ::SetThreadpoolThreadMinimum(m_handle.get(), count);
		}

		explicit operator bool()
		{
			return static_cast<bool>(m_handle);
		}

    private:

        static void __stdcall callback_wrapper(PTP_CALLBACK_INSTANCE, void* args)
        {
            auto w = static_cast<stateless_function>(args);

            w();
        }
	};
}