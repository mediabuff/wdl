// key.hpp
//
// Free Function Implementation
//  - wdl::crypto::create_key()

#include <crypto/base.hpp>

namespace wdl::crypto
{
    key_handle create_key(
        provider_handle const& p,
        void const*            secret,
        unsigned               size
        )
    {
        auto k = key_handle{};

        auto result = ::BCryptGenerateSymmetricKey(
            p.get(),
            k.get_address_of(),
            nullptr,
            0,
            static_cast<unsigned char*>(
                const_cast<void*>(secret)),
            size,
            0);

        wdl::error::check_nt(result);

        return k;
    }
}