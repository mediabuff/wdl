// module.hpp
//
// Type Definitions
//  - wdl::handle::module_handle

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::handle
{
    // wdl::handle::module_handle_traits
    //
    // Traits definition appropriate for use 
    // with Windows dynamic libraries.

    struct module_handle_traits
    {
        using pointer = HMODULE;

        constexpr static pointer invalid() noexcept
        {
            return NULL;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::FreeLibrary(value));
        }
    };

    using module_handle = unique_handle<module_handle_traits>;
}