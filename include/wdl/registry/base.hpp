// base.hpp
//
// Type Definitions
//  wdl::registry::registry_handle

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::registry
{
    // wdl::registry::registry_handle_traits
    //
    // Traits definition appropriate for Windows
    // handles returned by Registry API calls.

    struct registry_handle_traits
    {
        using pointer = HKEY;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY_(ERROR_SUCCESS, ::RegCloseKey(value));
        }
    };

    using registry_handle = wdl::handle::unique_handle<registry_handle_traits>;
}