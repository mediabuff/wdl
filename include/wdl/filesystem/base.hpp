// base.hpp
//
// Type Definitions
//  - wdl::filesystem::find_file_handle

#pragma once

#include <windows.h>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::filesystem
{
    // wdl::filesystem::find_file_handle_traits
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

    using find_file_handle = wdl::handle::unique_handle<find_file_handle_traits>;
}