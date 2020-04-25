// base.hpp
//
// Type Definitions:
//  - wdl::security::lsa_policy_handle

#pragma once

#include <ntsecapi.h>

#include <wdl/handle/unique_handle.hpp>

namespace wdl::security
{
    // wdl::security::lsa_policy_handle_traits
    //
    // Traits definition appropriate for use
    // with LSA_HANDLE types.

    struct lsa_policy_handle_traits
    {
        using pointer = LSA_HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::LsaClose(value) >= 0);
        }
    };

    using lsa_policy_handle 
        = wdl::handle::unique_handle<lsa_policy_handle_traits>;
}

