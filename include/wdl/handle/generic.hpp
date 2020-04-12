// generic.hpp
//
// Type Definitions
//  - wdl::handle::null_handle
//  - wdl::handle::invalid_handle

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::handle
{
    // wdl::handle::null_handle_traits
    // 
    // Traits definition appropriate for raw Windows handles
    // for which the return value on initilization failure 
    // is a null value (i.e. NULL or nullptr)

    struct null_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            ::CloseHandle(value);
        }
    };

    using null_handle = unique_handle<null_handle_traits>;

    // wdl::handle::invalid_handle_traits
    //
    // Traits definition appropriate for raw Windows handles
    // for which the return value on initilization failure 
    // is a Windows constant #define INVALID_HANDLE_VALUE

    struct invalid_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return INVALID_HANDLE_VALUE;
        }

        static void close(pointer value) noexcept
        {
            ::CloseHandle(value);
        }
    };   

    using invalid_handle = unique_handle<invalid_handle_traits>;
}