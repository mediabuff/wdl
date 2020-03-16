// pool.hpp

#pragma once

#include <windows.h>

#include "environment.hpp"
#include <wdl/utility/unique_handle.hpp>

namespace wdl::threadpool
{
	// wdl::threadpool::io_completion_f
	//
	// Defines function signature for IO completion callback.

	using io_completion_f = void(*)(unsigned long, ULONG_PTR); 

	// wdl::threadpool::pool
    //
    // Wrapper around user-controlled threadpool.

	class pool
	{
        environment               m_environment;
		wdl::utility::pool_handle m_handle;

	public:
		pool()
			: m_handle{ ::CreateThreadpool(nullptr) }
		{
            ::SetThreadpoolCallbackPool(m_environment.get(), m_handle.get());
        }

		// rely on unique_handle for release
		~pool() = default;

		PTP_IO register_io(HANDLE handle, io_completion_f handler);

		void set_max_threadcount(unsigned long count);
		bool set_min_threadcount(unsigned long count);

		explicit operator bool()
		{
			return static_cast<bool>(m_handle);
		}

    private:		
		// proxy for io completion
		static void __stdcall io_completion_trampoline(
			PTP_CALLBACK_INSTANCE instance,
			void*                 context, 
			void*                 overlapped,
			unsigned long         io_result,
			ULONG_PTR             bytes_transferred,
			PTP_IO                io
			);
	};
	
	PTP_IO pool::register_io(
		HANDLE          handle, 
		io_completion_f handler
		)
	{
        // create the io object
        auto io_object = ::CreateThreadpoolIo(
            handle, 
            io_completion_trampoline,
            static_cast<void*>(handler),
            m_environment.get()
            );
        
        // signal to the threadpool to start waiting for completions
        ::StartThreadpoolIo(io_object);

		return io_object;
	}

	void pool::set_max_threadcount(unsigned long count)
	{
		::SetThreadpoolThreadMaximum(m_handle.get(), count);
	}

	bool pool::set_min_threadcount(unsigned long count)
	{
		return ::SetThreadpoolThreadMinimum(m_handle.get(), count);
	}

	void __stdcall pool::io_completion_trampoline(
		PTP_CALLBACK_INSTANCE instance,
		void*                 context, 
		void*                 overlapped,
		unsigned long         io_result,
		ULONG_PTR             bytes_transferred,
		PTP_IO                io
		)
	{
		auto fn = static_cast<io_completion_f>(context);
		fn(io_result, bytes_transferred); 
	}
}