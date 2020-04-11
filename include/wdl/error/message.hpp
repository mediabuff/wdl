// message.hpp
//
// Free Function Implementation:
//  - wdl::error::get_message()

#pragma once

#include <windows.h>
#include <string>

#include <wdl/handle.hpp>

namespace wdl::error
{
    // wdl::error::get_message()
    //
    // HRESULT overload

    std::wstring get_message(HRESULT const code)
    {
        auto local = wdl::handle::local_alloc_handle{};

        auto const flags = 
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM     |
            FORMAT_MESSAGE_IGNORE_INSERTS;

        auto size = ::FormatMessageW(
            flags,
            nullptr,
            code,
            0,
            reinterpret_cast<wchar_t*>(local.get_address_of()),
            0,
            nullptr);

        auto* as_wstr = static_cast<wchar_t*>(local.get());
        while (size && iswspace(*(as_wstr + size - 1)))
        {
            --size;
        }

        if (0 == size) return L"Error Information Unavailable";

        return std::wstring{ as_wstr, as_wstr + size };
    }

    // wdl::error::get_message()
    //
    // Win32 API error overload

    std::wstring get_message(unsigned long const code)
    {
        auto local = wdl::handle::local_alloc_handle{};

        auto const flags = 
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM     |
            FORMAT_MESSAGE_IGNORE_INSERTS;

        auto size = ::FormatMessageW(
            flags,
            nullptr,
            code,
            0,
            reinterpret_cast<wchar_t*>(local.get_address_of()),
            0,
            nullptr);

        auto* as_wstr = static_cast<wchar_t*>(local.get());
        while (size && iswspace(*(as_wstr + size - 1)))
        {
            --size;
        }

        if (0 == size) return L"Error Information Unavailable";

        return std::wstring{ as_wstr, as_wstr + size };
    }
}