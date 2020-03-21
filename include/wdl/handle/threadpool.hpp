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

    // wdl::handle::work_handle_traits
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
            ::CloseThreadpoolWork(value);
        }
    };

    // wdl::handle::wait_handle_traits
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
            ::CloseThreadpoolWait(value);
        }
    };

    // wdl::handle::timer_handle_traits
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
            ::CloseThreadpoolTimer(value);
        }
    };

    // wdl::handle::cleanup_group_traits
    //
    // Traits definition appropriate for use
    // with Windows thread pool cleanup group object.

    struct cleanup_group_traits
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