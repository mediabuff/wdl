// path.hpp
//
// Free Function Implementation:
//  - wdl::filesystem::path_append()
//  - wdl::filesystem::path_remove_filename()

#pragma once

#include <windows.h>
#include <PathCch.h>

#include <string>

#include <wdl/filesystem/base.hpp>
#include <wdl/error/com_exception.hpp>
#include <wdl/error/win32_exception.hpp>

namespace wdl::filesystem
{
    namespace detail
    {
        void trim(std::wstring& path)
        {
            path.resize(wcslen(path.c_str()));
        }

        void path_append(
            std::wstring&  path,
            wchar_t const* more,
            size_t const   size
            )
        {   
            // resize buffer for worst case size increase
            path.resize(path.size() + 5 + size);

            ::PathCchAppendEx(
                path.data(),
                path.size() + 1,
                more,
                PATHCCH_ALLOW_LONG_PATHS);

            trim(path);
        }
    }

    void path_append(
        std::wstring&  path,
        wchar_t const* more
        )
    {
        detail::path_append(path, more, wcslen(more));
    }

    void path_append(
        std::wstring&       path,
        std::wstring const& more
        )
    {
        detail::path_append(path, more.data(), more.size());
    }

    bool path_remove_filename(std::wstring& path)
    {
        auto const result = ::PathCchRemoveFileSpec(
            path.data(),
            path.size() + 1);

        if (S_OK == result)
        {
            detail::trim(path);
            return true;
        }

        if (S_FALSE == result)
        {
            return false;
        }

        throw wdl::error::com_exception{ result };
    }
}