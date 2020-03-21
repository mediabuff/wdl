// network.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <winhttp.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // wdl::handle::winhttp_handle_traits
    //
    // Traits definition appropriate for use
    // with the WinHTTP API.

    struct winhttp_handle_traits
    {
        using pointer = HINTERNET;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::WinHttpCloseHandle(value));
        }
    };
}