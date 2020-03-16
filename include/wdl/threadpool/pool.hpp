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

        using callback_f      = void(*)();
		using io_completion_f = void(*)(unsigned long, ULONG_PTR); 

	public:
		pool()
			: m_handle{ ::CreateThreadpool(nullptr) }
		{
            ::SetThreadpoolCallbackPool(m_environment.get(), m_handle.get());
        }

		// rely on unique_handle for release
		~pool() = default;

		template <typename Callback>
		bool submit_callback(Callback work);

		template <typename Callback>
		PTP_IO register_io(HANDLE handle, Callback handler);

		void set_max_threadcount(unsigned long count);
		bool set_min_threadcount(unsigned long count);

		explicit operator bool()
		{
			return static_cast<bool>(m_handle);
		}

    private:
		// proxy for callback completion
        static void __stdcall callback_wrapper(
			PTP_CALLBACK_INSTANCE, 
			void* args
			);
		
		// proxy for io completion
		static void __stdcall io_completion_wrapper(
			PTP_CALLBACK_INSTANCE instance,
			void*                 context, 
			void*                 overlapped,
			unsigned long         io_result,
			ULONG_PTR             bytes_transferred,
			PTP_IO                io
			);
	};

	template <typename Callback>
	bool pool::submit_callback(Callback work)
	{
		return ::TrySubmitThreadpoolCallback(
			callback_wrapper, 
			static_cast<stateless_function>(work), 
			m_environment.get()
			);
	}
	
	template <typename Callback>
	PTP_IO pool::register_io(
		HANDLE   handle, 
		Callback handler
		)
	{
        // create the io object
        auto io_object = ::CreateThreadpoolIo(
            handle, 
            io_completion_wrapper,
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

	void __stdcall pool::callback_wrapper(
		PTP_CALLBACK_INSTANCE, 
		void* args
		)
	{
		auto fn = static_cast<callback_f>(args);
		fn();
	}

	void __stdcall pool::io_completion_wrapper(
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