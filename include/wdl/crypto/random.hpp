// random.hpp
//
// Free Function Implementations
//  - wdl::crypto::random()

#pragma once

#include <wdl/crypto/base.hpp>

namespace wdl::crypto
{
    void random(
        provider_handle const& p,
        void*                  buffer,
        unsigned               size
        )
    {
        auto result = ::BCryptGenRandom(
            p.get(),
            static_cast<unsigned char*>(buffer),
            size,
            0);

        wdl::error::check_nt(result);
    }

    template <typename T, unsigned Count>
    void random(
        provider_handle const& p,
        T(&buffer)[Count]
    )
    {
        random(p, buffer, sizeof(T) * Count);
    }

    template <typename T>
    void random(provider_handle const& p, T& buffer)
    {
        random(p, &buffer, sizeof(T));
    } 
}