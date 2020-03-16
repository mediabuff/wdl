// duplex_pipe.hpp
// Class Implementation: wdl::ipc::duplex_pipe
//
// A named-pipe wrapper that supports
// aysnchronous full-duplex communications.

#pragma once

#include <windows.h>
#include <ppltasks.h>

#include <string>
#include <stdexcept>

#include <wdl/utility/unique_handle.hpp>

using wdl::utility::server_pipe;
using wdl::utility::client_pipe;

namespace wdl::ipc
{
    class duplex_pipe
    {
        std::wstring m_local_name;
        std::wstring m_remote_name;

        server_pipe  m_recv_pipe;
        client_pipe  m_send_pipe;

    public:
        duplex_pipe()  = default;
        ~duplex_pipe() = default;

        // non-copyable
        duplex_pipe(duplex_pipe const&)            = delete;
        duplex_pipe& operator=(duplex_pipe const&) = delete;

        // participates in move semantics
        // TODO

        // listen
        // Setup the local pipe instance for recieving connection request.
        bool listen(std::wstring const& local_name);

        // try_connect
        // Attempt connection to remote pipe instance
        bool try_connect(std::wstring const& remote_name);

        // accept_async
        // Accept an incoming connection request asynchronously.
        bool accept_async(LPOVERLAPPED ov);

        HANDLE server_native_handle() const noexcept;
        HANDLE client_native_handle() const noexcept;
    };

    bool duplex_pipe::listen(std::wstring const& local_name)
    {
        m_local_name = local_name;

        m_recv_pipe = server_pipe
        {
            ::CreateNamedPipeW(
                m_local_name.c_str(),
                PIPE_ACCESS_DUPLEX | 
                FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_MESSAGE     |
                PIPE_READMODE_MESSAGE | 
                PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                0, 0, 0,
                nullptr
            )
        };

        // pipe handle provides explicit boolean operator
        return static_cast<bool>(m_recv_pipe);
    }

    bool duplex_pipe::accept_async(LPOVERLAPPED ov)
    {
        if (!m_recv_pipe)
        {
            throw std::logic_error{"Must listen on local pipe instance prior to calling accept"};
        }

        return ::ConnectNamedPipe(m_recv_pipe.get(), ov);
    }

    bool duplex_pipe::try_connect(
        std::wstring const& remote_name 
        )
    {
        // NOTE: does not mess with WaitNamedPipe here

        auto p = client_pipe
        {
            ::CreateFileW(
                m_remote_name.c_str(),
                GENERIC_READ | 
                GENERIC_WRITE,
                0, nullptr,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                nullptr
            )
        };

        if (!p)
        {
            return false;
        }

        // take ownership of the successfully connected pipe
        m_send_pipe = std::move(p);

        auto properties = unsigned long{PIPE_READMODE_MESSAGE | PIPE_WAIT};
        auto res = ::SetNamedPipeHandleState(
            m_send_pipe.get(),
            &properties,
            0, 0
            );

        return res;
    }

    HANDLE duplex_pipe::server_native_handle() const noexcept
    {
        return m_recv_pipe.get();
    }

    HANDLE duplex_pipe::client_native_handle() const noexcept
    {
        return m_send_pipe.get();
    }
}



