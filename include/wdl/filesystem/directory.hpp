// directory.hpp
//
// Free Function Implementation:
//  - wdl::filesystem::create_directory()
//  - wdl::filesystem::remove_directory()

#pragma once

#include <windows.h>
#include <PathCch.h>

#include <string>

#include <wdl/filesystem/base.hpp>
#include <wdl/filesystem/path.hpp>
#include <wdl/error/com_exception.hpp>
#include <wdl/error/win32_exception.hpp>

namespace wdl::filesystem
{
    namespace detail
    {
        void create_directory(std::wstring const& path)
        {
            if (!::CreateDirectoryW(path.c_str(), nullptr))
            {
                auto const result = ::GetLastError();
                if (ERROR_ALREADY_EXISTS != result)
                {
                    throw wdl::error::win32_exception{ result };
                }
            }
        }
    }

    void create_directory(std::wstring path)
    {
        for (auto i = path.find(L'\\');
            i != std::wstring::npos;
            i = path.find(L'\\'))
        {
            // \\?\C:\path
            if (i == 0)             continue;
            if (path[i-1] == L'\\') continue;
            if (path[i-1] == L'?')  continue;
            if (path[i-1] == L':')  continue;

            path[i] = 0;
            detail::create_directory(path);
            path[i] = L'\\';
        }

        detail::create_directory(path);
    } 

    void remove_directory(wchar_t const* path)
    {
        auto search = std::wstring{ path };

        path_append(search, L"*");

        auto data = WIN32_FIND_DATA{};

        auto find = find_file_handle
        {
            ::FindFirstFileW(search.c_str(), &data)
        };

        if (find)
        {
            do
            {
                if (0 == wcscmp(data.cFileName, L".")) continue;
                if (0 == wcscmp(data.cFileName, L"..")) continue;

                path_remove_filename(search);
                path_append(search, data.cFileName);

                if (FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes)
                {
                    remove_directory(path);
                }
                else
                {
                    VERIFY(::DeleteFileW(search.c_str()));
                }
                

            } while (::FindNextFileW(find.get(), &data));
        }

        // handle the pseudo-asynchronous properties of filesystem operations
        for (auto i = 0; i < 10; ++i)
        {
            if(::RemoveDirectoryW(path)) break;
            if (ERROR_FILE_NOT_FOUND == ::GetLastError()) break;
            ::Sleep(10);
        }
    }

    void remove_directory(std::wstring const& path)
    {
        remove_directory(path.c_str());
    }
}