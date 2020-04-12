// symmetric.hpp
//
// Free Function Implementations
//  - wdl::crypto::create_shared_secret()
//  - wdl::crypto::encrypt()
//  - wdl::crypto::decrypt()
//  - wdl::crypto::encrypt_message()
//  - wdl::crypto::decrypt_message()

#pragma once

#include <wdl/crypto/base.hpp>
#include <wdl/crypto/key.hpp>
#include <wdl/crypto/hash.hpp>

namespace wdl::crypto
{   
    bytes_t create_shared_secret(bytes_view_t secret)
    {
        auto provider = open_provider(BCRYPT_SHA256_ALGORITHM);

        auto hash = create_hash(provider);

        combine_hash(hash, secret.data(), secret.size());

        auto size = unsigned{};

        get_property(hash.get(), BCRYPT_HASH_LENGTH, size);

        auto value = bytes_t(size, 0);

        get_hash_value(hash, value.data(), size);

        return value;
    }

    unsigned long encrypt(
        key_handle const& k,
        void const*       plaintext,
        size_t            plaintext_size,
        void*             ciphertext,
        size_t            ciphertext_size,
        unsigned          flags
        )
    {
        auto bytes_copied = unsigned long{};

        auto result = ::BCryptEncrypt(
            k.get(),
            static_cast<unsigned char*>(
                const_cast<void*>(plaintext)),
            static_cast<unsigned long>(plaintext_size),
            nullptr,
            nullptr,
            0,
            static_cast<unsigned char*>(ciphertext),
            static_cast<unsigned long>(ciphertext_size),
            &bytes_copied,
            flags);

        wdl::error::check_nt(result);

        return bytes_copied;
    }

    unsigned long decrypt(
        key_handle const& k,
        void const*       ciphertext,
        size_t            ciphertext_size,
        void*             plaintext,
        size_t            plaintext_size,
        unsigned          flags
        )
    {
        auto bytes_copied = unsigned long{};

        auto result = ::BCryptDecrypt(
            k.get(),
            static_cast<unsigned char*>(
                const_cast<void*>(ciphertext)),
            static_cast<unsigned long>(ciphertext_size),
            nullptr,
            nullptr,
            0,
            static_cast<unsigned char*>(plaintext),
            static_cast<unsigned long>(plaintext_size),
            &bytes_copied,
            flags);

        wdl::error::check_nt(result);

        return bytes_copied;
    }

    bytes_t encrypt_message(
        bytes_view_t   shared,
        bytes_view_t   plaintext,
        wchar_t const* algorithm = BCRYPT_AES_ALGORITHM
        )
    {
        auto provider = open_provider(algorithm);

        auto key = create_key(
            provider,
            shared.data(),
            shared.size());

        auto size = encrypt(
            key,
            plaintext.data(),
            plaintext.size(),
            nullptr,
            0,
            BCRYPT_BLOCK_PADDING);

        auto ciphertext = bytes_t(size, 0);

        encrypt(
            key,
            plaintext.data(),
            plaintext.size(),
            ciphertext.data(),
            size,
            BCRYPT_BLOCK_PADDING);

        return ciphertext;
    }

    bytes_t decrypt_message(
        bytes_view_t   shared,
        bytes_view_t   ciphertext,
        wchar_t const* algorithm = BCRYPT_AES_ALGORITHM
        )
    {
        auto provider = open_provider(algorithm);

        auto key = create_key(
            provider,
            shared.data(),
            shared.size());

        auto size = decrypt(
            key,
            ciphertext.data(),
            ciphertext.size(),
            nullptr,
            0,
            BCRYPT_BLOCK_PADDING);

        auto plaintext = bytes_t(size, 0);

        decrypt(
            key,
            ciphertext.data(),
            ciphertext.size(),
            plaintext.data(),
            size,
            BCRYPT_BLOCK_PADDING);

        return plaintext;
    }
}