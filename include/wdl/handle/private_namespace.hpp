// private_namespace.hpp
//
// Type Definitions:
//  

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::handle
{
    // wdl::handle::private_namespace_handle_traits

    struct private_namespace_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return NULL;
        }

        static void close(pointer value) noexcept
        {
            VERIFY(::ClosePrivateNamespace(value, 0));
        }
    };

    using private_namespace_handle = unique_handle<private_namespace_handle_traits>;

    // wdl::handle::boundary_descriptor_handle_traits

    struct boundary_descriptor_handle_traits
    {
        using pointer = HANDLE;

        constexpr static pointer invalid() noexcept
        {
            return NULL;
        }

        static void close(pointer value) noexcept
        {
            ::DeleteBoundaryDescriptor(value);
        }
    };

    using boundary_descriptor_handle = unique_handle<boundary_descriptor_handle_traits>;
}