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

	using work_callback_f = void(__stdcall*)(
		PTP_CALLBACK_INSTANCE, 
		void*, 
		PTP_WORK);

	// wdl::threadpool::wait_completion_f
	//
	// Defines function signature for wait completion callback.

	using wait_completion_f = void(__stdcall*)(
		PTP_CALLBACK_INSTANCE, 
		void*, 
		PTP_WAIT, 
		TP_WAIT_RESULT);

	// wdl::threadpool::timer_completion_f
	//
	// Defines callback signature for timer completion callback.

	using timer_completion_f = void(*)(
		PTP_CALLBACK_INSTANCE, 
		void*, 
		PTP_TIMER);

	// wdl::threadpool::io_completion_f
	//
	// Defines function signature for IO completion callback.
	
	using io_completion_f = void(*)(
		PTP_CALLBACK_INSTANCE, 
		void*, 
		void*, 
		unsigned long, 
		ULONG_PTR, 
		PTP_IO); 

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

		PTP_WORK create_work(work_callback_f callback, void* ctx)
		{
			auto work_object = ::CreateThreadpoolWork(
				callback,
				ctx,
				m_environment.get());

			return work_object;
		}

		PTP_WAIT create_wait(wait_completion_f callback, void* ctx)
		{
			auto wait_object = ::CreateThreadpoolWait(
				callback,
				ctx,
				m_environment.get());

			return wait_object;	
		}

		PTP_TIMER create_timer(timer_completion_f callback, void* ctx)
		{
			auto timer_object = ::CreateThreadpoolTimer(
				callback,
				ctx,
				m_environment.get());

			return timer_object;
		}

		PTP_IO create_io(HANDLE handle, io_completion_f callback, void* ctx)
		{
			auto io_object = ::CreateThreadpoolIo(
				handle, 
				callback,
				ctx,
				m_environment.get());
			
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
	};
	
	PTP_WORK create_work(pool& p, work_callback_f fn, void* ctx)
	{
		return p.create_work(fn, ctx);
	}

	void submit_work(PTP_WORK work_object)
	{
		::SubmitThreadpoolWork(work_object);
	}

	PTP_WAIT create_wait(pool& p, wait_completion_f fn, void* ctx)
	{
		return p.create_wait(fn, ctx);
	}

	void set_wait(PTP_WAIT wait_object, HANDLE handle)
	{
		::SetThreadpoolWait(wait_object, handle, nullptr);
	}

	template <typename DurationType>
	void set_wait(
		PTP_WAIT     wait_object, 
		HANDLE       handle, 
		DurationType timeout
		)
	{
		auto ft = wdl::timing::to_filetime<DurationType>(timeout);
		::SetThreadpoolWait(wait_object, handle, &ft);
	}

	PTP_TIMER create_timer(pool& p, timer_completion_f fn, void* ctx)
	{
		return p.create_timer(fn, ctx);
	}

	template <typename DueDuration, 
			  typename PeriodDuration, 
			  typename WindowDuration>
	void set_timer(
		PTP_TIMER      timer_object,
		DueDuration    due_time,
		PeriodDuration period,
		WindowDuration window
		)
	{
		auto ft = wdl::timing::to_filetime<DueDuration>(due_time);
		auto p  = std::chrono::duration_cast<std::chrono::milliseconds>(period).count();
		auto w  = std::chrono::duration_cast<std::chrono::milliseconds>(window).count();

		// TODO: explicit narrowing conversions
		::SetThreadpoolTimer(
			timer_object,
			&ft,
			static_cast<unsigned long>(p),
			static_cast<unsigned long>(w)
			);
	}

	PTP_IO create_io(pool& p, HANDLE handle, io_completion_f fn, void* ctx)
	{
		return p.create_io(handle, fn, ctx);
	}

	void start_io(PTP_IO io_object)
	{
		::StartThreadpoolIo(io_object);
	}
}