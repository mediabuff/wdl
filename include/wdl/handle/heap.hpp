// heap.hpp
//
// Type Definitions
//  - wdl::handle::local_alloc_handle

#pragma once

#include <windows.h>

#include <wdl/debug.hpp>
#include <wdl/handle/unique_handle.hpp>

namespace wdl::handle
{
    // wdl::handle::local_alloc_handle_traits
    // 
    // Traits definition appropriate for memory
    // allocations made with LocalAlloc()

    struct local_alloc_handle_traits
    {
        using pointer = void*;

        constexpr static pointer invalid() noexcept
        {
            return nullptr;
        }

        static void close(pointer value) noexcept
        {
            VERIFY_(nullptr, ::LocalFree(value));
        }
    };

    using local_alloc_handle = unique_handle<local_alloc_handle_traits>;
}