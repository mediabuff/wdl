// duplex_pipe_old.hpp
// Class Implementation: wdl::ipc::duplex_pipe_old
//
// A named-pipe wrapper that supports
// aysnchronous full-duplex communications.

#pragma once

#include <windows.h>
#include <ppltasks.h>

#include <string>

#include <wdl/utility/unique_handle.hpp>

using wdl::utility::server_pipe;
using wdl::utility::client_pipe;

// namespace alias for Windows concurrency runtime namespace
namespace wincrt = concurrency;

namespace wdl::ipc
{
    // wdl::ipc::pipe_type
    //
    // Distinguishes duplex pipe types
    // for negotiation purposes.

    enum class pipe_type
    {
        client,
        server
    };

    // wdl::ipc::duplex_pipe_old
    //
    // Full duplex named-pipe implementation.

    class duplex_pipe_old
    {   
        pipe_type m_type;

        std::wstring m_local_name;
        std::wstring m_remote_name;

        server_pipe  m_recv_pipe;
        client_pipe  m_send_pipe;

    public:
        duplex_pipe_old(
            pipe_type           type,
            std::wstring const& local, 
            std::wstring const& remote
            )
            : m_type{ type }
            , m_local_name{ local }
            , m_remote_name{ remote }
        {}

        // rely on unique_handle dtors to handle resource release
        ~duplex_pipe_old() = default;

        // non-copyable
        duplex_pipe_old(duplex_pipe_old const&)            = delete;
        duplex_pipe_old& operator=(duplex_pipe_old const&) = delete;

        // synchronous negotiation
        bool negotiate();
        
        // asynchronous negotiation
        auto negotiate_async();

        auto read_message_async(
            std::byte* buffer, 
            std::size_t buffer_size
            );

        auto write_message_async(
            std::byte const* buffer, 
            std::size_t buffer_size
            );

    private:
        bool negotiate_client();
        bool negotiate_server();

        // synchronous accept
        bool accept();
        // synchronous connect
        bool connect();
    };

    bool duplex_pipe_old::negotiate()
    {
        return m_type == pipe_type::client
            ? negotiate_client()
            : negotiate_server();
    }

    auto duplex_pipe_old::negotiate_async()
    {
        return wincrt::create_task([this]()
        {
            return negotiate();
        });
    }

    bool duplex_pipe_old::negotiate_client()
    {
        if (connect())
        {
            return accept();
        }

        return false;
    }

    bool duplex_pipe_old::negotiate_server()
    {
        if (accept())
        {
            return connect();
        }

        return false;
    }

    bool duplex_pipe_old::accept()
    {
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

        if (!m_recv_pipe)
        {
            return false;
        }

        auto res = ::ConnectNamedPipe(m_recv_pipe.get(), nullptr);
        return res;
    }

    bool duplex_pipe_old::connect()
    {
        if (!::WaitNamedPipeW(m_remote_name.c_str(), NMPWAIT_WAIT_FOREVER))
        {
            return false;
        }
        
        m_send_pipe = client_pipe
        {
            ::CreateFileW(
                m_remote_name.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0, nullptr,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                nullptr
            )
        };

        if (!m_send_pipe)
        {
            return false;
        }

        auto properties = unsigned long{PIPE_READMODE_MESSAGE | PIPE_WAIT};
        auto res = ::SetNamedPipeHandleState(
            m_send_pipe.get(),
            &properties,
            0, 0
            );

        return res;
    }

    auto duplex_pipe_old::read_message_async(
        std::byte* buffer, 
        std::size_t buffer_size
        )
    {
        return wincrt::create_task([=]()
        {
            auto bytes_read = 0ul;
            ::ReadFile(
                m_recv_pipe.get(),
                static_cast<void*>(buffer),
                static_cast<unsigned long>(buffer_size),
                &bytes_read,
                nullptr
                );
            
            return static_cast<std::size_t>(bytes_read);
        });
    }

    auto duplex_pipe_old::write_message_async(
        std::byte const* buffer, 
        std::size_t buffer_size
        )
    {
        return wincrt::create_task([=]()
        {
            auto bytes_written = 0ul;
            ::WriteFile(
                m_send_pipe.get(),
                static_cast<void*>(const_cast<std::byte*>(buffer)),
                static_cast<unsigned long>(buffer_size),
                &bytes_written,
                nullptr
                );

            return static_cast<std::size_t>(bytes_written);
        });
    }

    // wdl::ipc::pipe_negotiate
    //
    // Synchronous negotiation.

    bool pipe_negotiate(duplex_pipe_old& pipe)
    {
        return pipe.negotiate();
    }

    // wdl::ipc::pipe_negotiate_async
    //
    // Asynchronous negotiation.

    auto pipe_negotiate_async(duplex_pipe_old& pipe)
    {
        return pipe.negotiate_async();
    }

    // wdl::ipc::pipe_negotiate_async
    //
    // Asynchronous negotiation with callback function.

    template <typename Handler>
    auto pipe_negotiate_async(duplex_pipe_old& pipe, Handler fn)
    {
        return pipe.negotiate_async()
        .then([&](bool success)
        {
            return success ? fn() : false;
        });
    }

    // wdl::ipc::pipe_read_message_async

    auto pipe_read_message_async(
        duplex_pipe_old& pipe,
        std::byte* buffer, 
        std::size_t buffer_size
        )
    {
        return pipe.read_message_async(buffer, buffer_size);
    }

    // wdl::ipc::pipe_write_message_async

    auto pipe_write_message_async(
        duplex_pipe_old& pipe,
        std::byte const* buffer, 
        std::size_t buffer_size
        )
    {
        return pipe.write_message_async(buffer, buffer_size);
    }

}



