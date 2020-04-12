// base.hpp
//
// Type Definitions
//  - wdl::crypto::byte_t
//  - wdl::crypto::bytes_t
//  - wdl::crypto::provider_handle
//  - wdl::crypto::hash_handle
//  - wdl::crypto::key_handle
//  - wdl::crypto::to_bytes
//
// Free Function Implementations
//  - wdl::crypto::open_provider()
//  - wdl::crypto::get_property()

#pragma once

#include <windows.h>
#include <bcrypt.h>

#include <string>
#include <string_view>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>
#include <wdl/error/nt_exception.hpp>

namespace wdl::crypto
{
    using byte_t       = BYTE;
    using bytes_t      = std::basic_string<byte_t>;
    using bytes_view_t = std::basic_string_view<byte_t>;

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

    using provider_handle = wdl::handle::unique_handle<provider_handle_traits>;

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

    using hash_handle = wdl::handle::unique_handle<hash_handle_traits>;

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

    using key_handle = wdl::handle::unique_handle<key_handle_traits>;

    // wdl::crypto::open_provider()

    provider_handle open_provider(wchar_t const* algorithm)
    {
        auto p = provider_handle{};

        auto result = ::BCryptOpenAlgorithmProvider(
            p.get_address_of(),
            algorithm,
            nullptr, 0);

        wdl::error::check_nt(result);

        return p;
    }

    // wdl::crypto::get_property()

    template <typename T>
    void get_property(
        BCRYPT_HANDLE  handle,
        wchar_t const* name,
        T&             value
        )
    {
        auto bytes_copied = unsigned long{};

        auto result = ::BCryptGetProperty(
            handle,
            name,
            reinterpret_cast<unsigned char*>(&value),
            sizeof(T),
            &bytes_copied,
            0);

        wdl::error::check_nt(result);
    }

    bytes_t to_bytes(void const* buffer, unsigned long size)
    {
        return bytes_t(static_cast<byte_t const*>(buffer), size);
    }

    bytes_t to_bytes(std::string const& str)
    {
        return to_bytes(str.data(), static_cast<unsigned long>(str.length()));
    }

    std::string from_bytes(bytes_t bytes)
    {
        return std::string(reinterpret_cast<char*>(bytes.data()), bytes.length());
    }
}