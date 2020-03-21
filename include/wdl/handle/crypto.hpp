// crypto.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // wdl::handle::provider_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows CNG cryptography providers.

    struct provider_handle_traits
    {
        using pointer = BCRYPT_ALG_HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY_(ERROR_SUCCESS, ::BCryptCloseAlgorithmProvider(value, 0));
        }
    };

    // wdl::handle::hash_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows CNG cryptographic hash providers.

    struct hash_handle_traits
    {
        using pointer = BCRYPT_ALG_HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY_(ERROR_SUCCESS, ::BCryptDestroyHash(value));
        }
    };

    // wdl::handle::key_handle_traits
    //
    // Traits definition appropriate for use
    // with Windows CNG cryptographic keys.

    struct key_handle_traits
    {
        using pointer = BCRYPT_KEY_HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY_(ERROR_SUCCESS, ::BCryptDestroyKey(value));
        }
    };
}