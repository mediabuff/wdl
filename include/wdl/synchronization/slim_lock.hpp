// slim_lock.hpp
//
// Class Implementations:
//	wdl::synchronization::slim_lock
//	wdl::synchronization::slim_auto_lock
//
// Windows Slim Reader/Writer lock wrapper.

#pragma once

#include <windows.h>

namespace wdl::synchronization
{
	// wdl::synchronization::slim_auto_lock
	//
	// An RAII-based scoped lock with semantics that are
	// approriate for wrapping slim_lock objects.
	//
	// NOTE: there is likely way to unify this class
	// with the vanilla auto_lock, but I have not 
	// yet identified an API that achieves this satisfactorily. 

	template <typename T>
	class slim_auto_lock
	{
		using UnlockAction = void (T::*)();
	public:
		explicit slim_auto_lock(T& lock, UnlockAction action) noexcept
			: m_lock{ lock }, m_action{ action }
		{}

		~slim_auto_lock() noexcept
		{
			(m_lock.*m_action)();
		}

		slim_auto_lock(const T& other) = delete;
		slim_auto_lock(T&& other) = delete;
		T& operator=(const T& rhs) = delete;
		T& operator=(T&& rhs) = delete;

	private:
		T&           m_lock;
		UnlockAction m_action;
	};

	// wdl::synchronization::slim_lock
	//
	// Windows Slim Reader/Writer lock wrapper.

	class slim_lock
	{
		SRWLOCK m_lock{};

	public:
		slim_lock() noexcept
			: m_lock{ SRWLOCK_INIT }
		{}

		~slim_lock() = default;

		slim_lock(const slim_lock& other)          = delete;
		slim_lock(slim_lock&& other)               = delete;
		slim_lock& operator=(const slim_lock& rhs) = delete;
		slim_lock& operator=(slim_lock&& rhs)      = delete;

		void enter() noexcept
		{
			::AcquireSRWLockExclusive(&m_lock);
		}

		void exit() noexcept
		{
			::ReleaseSRWLockExclusive(&m_lock);
		}

		void enter_shared() noexcept
		{
			::AcquireSRWLockShared(&m_lock);
		}

		void exit_shared() noexcept
		{
			::ReleaseSRWLockShared(&m_lock);
		}

		slim_auto_lock<slim_lock> get_exclusive() noexcept
		{
			enter();
			return slim_auto_lock(*this, &slim_lock::exit);
		}

		slim_auto_lock<slim_lock> get_shared() noexcept
		{
			enter_shared();
			return slim_auto_lock(*this, &slim_lock::exit_shared);
		}

		const SRWLOCK* native_handle() const noexcept
		{
			return &m_lock;
		}
	};
}