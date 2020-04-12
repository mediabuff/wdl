// base.hpp
//
// Type Definitions

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::threadpool
{
    // wdl::threadpool::pool_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool object

    struct pool_handle_traits
    {
        using pointer = PTP_POOL;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            ::CloseThreadpool(value);
        }
    };

    using pool_handle = wdl::handle::unique_handle<pool_handle_traits>;

    // wdl::threadpool::work_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool work object.

    struct work_handle_traits
    {
        using pointer = PTP_WORK;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            // by default, does not cancel outstanding work
            ::WaitForThreadpoolWorkCallbacks(value, FALSE);
            ::CloseThreadpoolWork(value);
        }
    };

    using work_handle = wdl::handle::unique_handle<work_handle_traits>;

    // wdl::threadpool::io_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows threadpool IO objects.

    struct io_handle_traits
    {
        using pointer = PTP_IO;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            // by default, does not cancel outstanding operations
            ::WaitForThreadpoolIoCallbacks(value, FALSE);
            ::CloseThreadpoolIo(value);
        }
    };

    using io_handle = wdl::handle::unique_handle<io_handle_traits>;

    // wdl::threadpool::wait_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool wait objects.

    struct wait_handle_traits
    {
        using pointer = PTP_WAIT;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            // by default, does not cancel outstanding waits
            ::WaitForThreadpoolWaitCallbacks(value, FALSE);
            ::CloseThreadpoolWait(value);
        }
    };

    using wait_handle = wdl::handle::unique_handle<wait_handle_traits>;

    // wdl::threadpool::timer_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool timer object.

    struct timer_handle_traits
    {
        using pointer = PTP_TIMER;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            // by default, does not cancel outstanding timers
            ::WaitForThreadpoolTimerCallbacks(value, FALSE);
            ::CloseThreadpoolTimer(value);
        }
    };

    using timer_handle = wdl::handle::unique_handle<timer_handle_traits>;

    // wdl::threadpool::cleanup_group_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool cleanup group object.

    struct cleanup_group_handle_traits
    {
        using pointer = PTP_CLEANUP_GROUP;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            ::CloseThreadpoolCleanupGroup(value);
        }
    };

    using cleanup_group_handle = wdl::handle::unique_handle<cleanup_group_handle_traits>;
}