// pipe.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // np_client_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows named pipe client.

    struct np_client_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return INVALID_HANDLE_VALUE;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::CloseHandle(value));
        }
    };

    // np_server_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows named pipe server.

    struct np_server_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return INVALID_HANDLE_VALUE;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::DisconnectNamedPipe(value));
        }
    };
}