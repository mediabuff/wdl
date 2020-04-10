// scoped_process.hpp
//
// Class Implementation: wdl::process::scoped_process

#pragma once

#include <windows.h>
#include <string>

#include <wdl/debug.hpp>
#include <wdl/handle.hpp>

namespace wdl::process
{
    // wdl::process::scoped_process
    //
    // Wrapper around WDL process smart handle that automatically
    // terminates process on scope exit.

    class scoped_process
    {
        using null_handle_t = wdl::handle::null_handle;

        null_handle_t               m_handle;
        constexpr static auto const cmdline_max_length{256};

    public:
        scoped_process(std::string const& cmdline)
        {
            VERIFY(cmdline.length() < cmdline_max_length);

            auto process_info = PROCESS_INFORMATION{};
            auto startup_info = STARTUPINFOA{};

            char buffer[cmdline_max_length];
            strcpy_s(buffer, _countof(buffer), cmdline.c_str());

            if (::CreateProcessA(
                nullptr,
                buffer,
                nullptr,
                nullptr,
                FALSE,
                0,
                nullptr,
                nullptr,
                &startup_info,
                &process_info))
            {
                m_handle.reset(process_info.hProcess);
            }

            ::CloseHandle(process_info.hThread);
        }   

        scoped_process(scoped_process const&) = delete;
        scoped_process& operator=(scoped_process const&) = delete;

        scoped_process(scoped_process&& other)
            : m_handle{ other.m_handle.release() }
        {}

        scoped_process& operator=(scoped_process&& rhs)
        {
            if (this != &rhs)
            {
                terminate();
                m_handle.reset(rhs.m_handle.release());
            }

            return *this;
        }

        ~scoped_process()
        {
            terminate();
        }

        HANDLE get() const noexcept
        {
            return m_handle.get();
        }

        explicit operator bool() const noexcept
        {
            return static_cast<bool>(m_handle);
        }

    private:

        void terminate()
        {
            // TODO: ensure process is still running before termination?
            ::TerminateProcess(m_handle.get(), 0);
            ::WaitForSingleObject(m_handle.get(), INFINITE);
        }
    };
}