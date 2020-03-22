// pool.hpp
//
// Class Implementation: wdl::threadpool::pool

#pragma once

#include <windows.h>
#include <chrono>

#include <wdl/handle.hpp>
#include <wdl/timing/filetime.hpp>

#include "environment.hpp"
#include "cleanup_group.hpp"

namespace wdl::threadpool
{
	// wdl::threadpool::work_callback_f
	//
	// Defines function signature for work callback.

	using work_callback_f = void(*)(PTP_WORK);

	// wdl::threadpool::wait_completion_f
	//
	// Defines function signature for wait completion callback.

	using wait_completion_f = void(*)(PTP_WAIT, TP_WAIT_RESULT);

	// wdl::threadpool::timer_completion_f
	//
	// Defines callback signature for timer completion callback.

	using timer_completion_f = void(*)(PTP_TIMER);

	// wdl::threadpool::io_completion_f
	//
	// Defines function signature for IO completion callback.

	using io_completion_f = void(*)(PTP_IO, unsigned long, ULONG_PTR); 

	// wdl::threadpool::pool_cancellation_policy
	//
	// Determines if the created pool cancels 
	// outstanding callbacks on destruction.

	enum class pool_cancellation_policy
	{
		no_cancel,
		cancel
	};

	// wdl::threadpool::pool
    //
    // Wrapper around user-controlled threadpool.

	class pool
	{
		wdl::handle::pool_handle  m_handle;

        environment    m_environment;
		cleanup_group  m_cleanup_group;

		pool_cancellation_policy m_policy;

	public:
		pool(pool_cancellation_policy policy = pool_cancellation_policy::no_cancel)
			: m_handle{ ::CreateThreadpool(nullptr) }
			, m_cleanup_group{}
			, m_policy{ policy }
		{
			m_cleanup_group.set_cancellation_policy(
				static_cast<bool>(policy) 
				? cleanup_group_cancellation_policy::cancel
				: cleanup_group_cancellation_policy::no_cancel
				);

			// associate the private pool with the internal environment
            ::SetThreadpoolCallbackPool(
				m_environment.get(), 
				m_handle.get());
			
			// and set a cleanup group for the environment
			::SetThreadpoolCallbackCleanupGroup(
				m_environment.get(), 
				m_cleanup_group.get(), 
				nullptr);
        }

		// rely on unique_handle for release
		~pool() = default;

		// non-copyable, non-movable
		pool(pool const&)            = delete;
		pool& operator=(pool const&) = delete;
		pool(pool&&)                 = delete;
		pool& operator=(pool&&)      = delete;

		void set_max_threadcount(unsigned long count)
		{
			::SetThreadpoolThreadMaximum(m_handle.get(), count);
		}

		bool set_min_threadcount(unsigned long count)
		{
			return ::SetThreadpoolThreadMinimum(m_handle.get(), count);
		}

		PTP_WORK submit_work(work_callback_f fn)
		{
			auto work_object = ::CreateThreadpoolWork(
				work_callback_trampoline,
				static_cast<void*>(fn),
				m_environment.get()
				);

			if (work_object != NULL)
			{
				::SubmitThreadpoolWork(work_object);
			}

			return work_object;
		}

		PTP_WAIT submit_wait(
			HANDLE            handle, 
			wait_completion_f fn
			)
		{
			auto wait_object = ::CreateThreadpoolWait(
				wait_completion_trampoline,
				static_cast<void*>(fn),
				m_environment.get()
				);

			if (wait_object != NULL)
			{
				// TODO: support non-infinite timeout
				::SetThreadpoolWait(wait_object, handle, NULL);
			}

			return wait_object;	
		}

		template <typename DurationType>
		PTP_WAIT submit_wait(
			HANDLE            handle,
			DurationType      timeout, 
			wait_completion_f fn
			)
		{
			auto wait_object = ::CreateThreadpoolWait(
				wait_completion_trampoline,
				static_cast<void*>(fn),
				m_environment.get()
				);

			if (wait_object != NULL)
			{
				auto ft = wdl::timing::to_filetime<DurationType>(duration);
				::SetThreadpoolWait(wait_object, handle, &ft);
			}

			return wait_object;	
		}

		template <typename DueDuration, 
				  typename PeriodDuration, 
				  typename WindowDuration>
		PTP_TIMER submit_timer(
			DueDuration        due_time,
			PeriodDuration     period,
			WindowDuration     window,
			timer_completion_f fn
		)
		{
			auto timer_object = ::CreateThreadpoolTimer(
				timer_completion_trampoline,
				static_cast<void*>(fn),
				m_environment.get()
				);

			if (timer_object != NULL)
			{
				auto ft = wdl::timing::to_filetime<DueDuration>(due_time);
				auto p = std::chrono::duration_cast<std::chrono::milliseconds>(period).count();
				auto w = std::chrono::duration_cast<std::chrono::milliseconds>(window).count();

				// TODO: explicit narrowing conversions
				::SetThreadpoolTimer(
					timer_object,
					&ft,
					static_cast<unsigned long>(p),
					static_cast<unsigned long>(w)
					);
			}

			return timer_object;
		}

		PTP_IO submit_io(
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
			
			if (io_object != NULL)
			{
				::StartThreadpoolIo(io_object);
			}

			return io_object;
		}

		bool is_valid() const noexcept
		{
			return static_cast<bool>(m_handle);
		}

		explicit operator bool()
		{
			return static_cast<bool>(m_handle);
		}

    private:
		// proxy for work callbacks
		static void __stdcall work_callback_trampoline(
        	PTP_CALLBACK_INSTANCE instance,
    		void*                 context,
    	    PTP_WORK              work_object
			)
		{
			auto fn = static_cast<work_callback_f>(context);
			fn(work_object);
		}

		// proxy for wait completions
		static void __stdcall wait_completion_trampoline(
        	PTP_CALLBACK_INSTANCE instance,
     		void*                 context,
         	PTP_WAIT              wait_object,
            TP_WAIT_RESULT        wait_result
			)
		{
			auto fn = static_cast<wait_completion_f>(context);
			fn(wait_object, wait_result);
		}

		// proxy for timer completions
		static void __stdcall timer_completion_trampoline(
			PTP_CALLBACK_INSTANCE instance,
    		void*                 context,
    		PTP_TIMER             timer_object
			)
		{
			auto fn = static_cast<timer_completion_f>(context);
			fn(timer_object);
		}

		// proxy for io completion
		static void __stdcall io_completion_trampoline(
			PTP_CALLBACK_INSTANCE instance,
			void*                 context, 
			void*                 overlapped,
			unsigned long         io_result,
			ULONG_PTR             bytes_transferred,
			PTP_IO                io_object
			)
		{
			auto fn = static_cast<io_completion_f>(context);
			fn(io_object, io_result, bytes_transferred); 
		}
	};
	

}