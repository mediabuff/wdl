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


namespace wdl::crypto
{   
    bytes_t create_shared_secret(bytes_t const& secret)
    {
        auto p = open_provider(BCRYPT_SHA256_ALGORITHM);

        auto h = create_hash(p);

        combine_hash(h, secret.data(), secret.size());

        auto size = unsigned{};

        get_property(h.get(), BCRYPT_HASH_LENGTH, size);

        auto value = bytes_t(size);

        get_hash_value(h, value.data(), size);

        return value;
    }

    unsigned long encrypt(
        key_handle const& k,
        void const*       plaintext,
        unsigned          plaintext_size,
        void*             ciphertext,
        unsigned          ciphertext_size,
        unsigned          flags
        )
    {
        auto bytes_copied = unsigned long{};

        auto result = ::BCryptEncrypt(
            k.get(),
            static_cast<unsigned char*>(
                const_cast<void*>(plaintext)),
            plaintext_size,
            nullptr,
            nullptr,
            0,
            static_cast<unsigned char*>(ciphertext),
            ciphertext_size,
            &bytes_copied,
            flags);

        wdl::error::check_nt(result);

        return bytes_copied;
    }

    unsigned long decrypt(
        key_handle const& k,
        void const*       ciphertext,
        unsigned          ciphertext_size,
        void*             plaintext,
        unsigned          plaintext_size,
        unsigned          flags
        )
    {
        auto bytes_copied = unsigned long{};

        auto result = ::BCryptDecrypt(
            k.get(),
            static_cast<unsigned char*>(
                const_cast<void*>(ciphertext)),
            ciphertext_size,
            nullptr,
            nullptr,
            0,
            static_cast<unsigned char*>(plaintext),
            plaintext_size,
            &bytes_copied,
            flags);

        wdl::error::check_nt(result);

        return bytes_copied;
    }

    bytes_t encrypt_message(
        bytes_t const& shared,
        bytes_t const& plaintext,
        wchar_t const* algorithm = BCRYPT_AES_ALGORITHM
        )
    {
        auto p = open_provider(algorithm);

        auto k = create_key(
            p,
            &shared[0],
            shared.size());

        auto size = encrypt(
            k,
            plaintext.data(),
            plaintext.size(),
            nullptr,
            0,
            BCRYPT_BLOCK_PADDING);

        bytes_t ciphertext(size);

        encrypt(
            k,
            plaintext.data(),
            plaintext.size(),
            &ciphertext[0],
            size,
            BCRYPT_BLOCK_PADDING);

        return ciphertext;
    }

    bytes_t decrypt_message(
        bytes_t const& shared,
        bytes_t const& ciphertext,
        wchar_t const* algorithm = BCRYPT_AES_ALGORITHM
        )
    {
        auto p = open_provider(algorithm);

        auto k = create_key(
            p,
            &shared[0],
            shared.size());

        auto size = decrypt(
            k,
            ciphertext.data(),
            ciphertext.size(),
            nullptr,
            0,
            BCRYPT_BLOCK_PADDING);

        auto plaintext = bytes_t(size, 0);

        decrypt(
            k,
            ciphertext.data(),
            ciphertext.size(),
            plaintext.data(),
            size,
            BCRYPT_BLOCK_PADDING);

        return plaintext;
    }
}