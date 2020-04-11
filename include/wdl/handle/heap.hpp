// heap.hpp
//
// Traits definitions for wdl::handle::unique_handle.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // local_handle_traits
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
}