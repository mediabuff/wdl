// filetime.hpp
//
// Free Function Implementations:
//  - wdl::timing::to_filetime()
//  - wdl::timing::from_filetime()

#pragma once

#include <windows.h>
#include <chrono>

namespace wdl::timing
{
    // wdl::timing::to_filetime()
    //
    // Convert stdlib chrono duration to relative filetime.

    template <typename DurationType>
    FILETIME to_filetime(DurationType duration)
    {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        
        auto tmp = LARGE_INTEGER{};
        tmp.QuadPart = -10000 * ms.count();

        auto ft = FILETIME{};
        ft.dwLowDateTime  = tmp.LowPart;
        ft.dwHighDateTime = tmp.HighPart;

        return ft;
    }

    // wdl::timing::from_filetime()
    //
    // Convert relative filetime to stdlib chrono duration

    template <typename DurationType>
    DurationType from_filetime(FILETIME filetime)
    {
        auto tmp = LARGE_INTEGER{};
        tmp.LowPart  = filetime.dwLowDateTime;
        tmp.HighPart = filetime.dwHighDateTime;

        auto ms = std::chrono::milliseconds{-(tmp.QuadPart / 10000)};

        return std::chrono::duration_cast<DurationType>(ms);
    }
}