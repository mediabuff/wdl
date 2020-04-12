// filesystem.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // find_file_handle_traits
    // 
    // Traits definition appropriate for 
    // filesystem file search operations

    struct find_file_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return INVALID_HANDLE_VALUE;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::FindClose(value));
        }
    };
}