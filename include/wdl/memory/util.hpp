// util.hpp
//
// Virtual memory utilities.

#pragma once

#include <windows.h>

namespace wdl::memory
{
    unsigned long page_size()
    {
        auto info = SYSTEM_INFO{};
        ::GetSystemInfo(&info);
        return info.dwPageSize;
    }
}