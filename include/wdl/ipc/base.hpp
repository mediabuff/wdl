// base.hpp
//
// Type Definitions
//  - wdl::ipc::np_client_handle
//  - wdl::ipc::np_server_handle

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::ipc
{
    // wdl::ipc::np_client_handle_traits
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

    using np_client_handle = wdl::handle::unique_handle<np_client_handle_traits>;

    // wdl::ipc::np_server_handle_traits
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

    using np_server_handle = wdl::handle::unique_handle<np_server_handle_traits>;
}