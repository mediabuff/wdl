// threadpool.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // wdl::handle::pool_handle_traits
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

    // wdl::handle::tp_work_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool work object.

    struct tp_work_handle_traits
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

    // wdl::threadpool::tp_io_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows threadpool IO objects.

    struct tp_io_handle_traits
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

    // wdl::handle::tp_wait_handle_traits
    // Traits definition appropriate for use
    // with Windows thread pool wait objects.

    struct tp_wait_handle_traits
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

    // wdl::handle::tp_timer_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool timer object.

    struct tp_timer_handle_traits
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

    // wdl::handle::tp_cleanup_group_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool cleanup group object.

    struct tp_cleanup_group_handle_traits
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
}