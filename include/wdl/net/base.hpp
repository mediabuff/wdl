// base.hpp
//
// Type Definitions
//  - wdl::network::http_handle

#pragma once

#include <winhttp.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::net
{
    // wdl::net::http_handle_traits
    //
    // Traits definition appropriate for use
    // with the WinHTTP API.

    struct http_handle_traits
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

    using http_handle = wdl::handle::unique_handle<http_handle_traits>;
}