// hash.hpp
//
// Free Function Implementations
//  - wdl::crypto::create_hash()
//  - wdl::crypto::combine_hash()
//  - wdl::crypto::get_hash_value()

#include <wdl/crypto/base.hpp>

namespace wdl::crypto
{
    hash_handle create_hash(provider_handle const& p)
    {
        auto h = hash_handle{};

        auto result = ::BCryptCreateHash(
            p.get(),
            h.get_address_of(),
            nullptr,
            0,
            nullptr,
            0, 0);

        wdl::error::check_nt(result);

        return h;
    }

    void combine_hash(
        hash_handle const& h,
        void const*        buffer,
        size_t             size
        )
    {
        auto result = ::BCryptHashData(
            h.get(),
            static_cast<unsigned char*>(
                const_cast<void*>(buffer)),
            static_cast<unsigned long>(size),
            0);

        wdl::error::check_nt(result);
    }

    void get_hash_value(
        hash_handle const& h,
        void*              buffer,
        size_t             size
        )
    {
        auto result = ::BCryptFinishHash(
            h.get(),
            static_cast<unsigned char*>(buffer),
            static_cast<unsigned long>(size),
            0);

        wdl::error::check_nt(result);
    }
}