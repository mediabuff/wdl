// iocp.hpp
//
// Class Implementation: wdl::io::iocp

#pragma once

#include <windows.h>
#include <wdl/utility/unique_handle.hpp>

#include <thread>

namespace wdl::io
{
    // wdl::io::iocp
    //
    // Wrapper around Windows IO Completion Port

    class iocp
    {
        using null_handle_t = wdl::utility::null_handle;

        null_handle_t m_port;

    public:
        iocp(unsigned long n_threads = std::thread::hardware_concurrency()) 
            : m_port{ ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, n_threads) }
        {}

        ~iocp() = default;

        iocp(iocp const&)            = delete;
        iocp& operator=(iocp const&) = delete;

        iocp(iocp&&)            = delete;
        iocp& operator=(iocp&&) = delete;

        bool register_io(HANDLE handle, ULONG_PTR key);

        bool get_completion(
            unsigned long* bytes_xfer, 
            PULONG_PTR key, 
            LPOVERLAPPED* ppov,
            unsigned long timeout = INFINITE
            );

        bool post_completion(
            unsigned long bytes_xfer, 
            ULONG_PTR     key, 
            LPOVERLAPPED  pov
            );

        explicit operator bool() const noexcept
        {
            return static_cast<bool>(m_port);
        }
    };

    bool iocp::register_io(HANDLE handle, ULONG_PTR key)
    {
        auto ret = ::CreateIoCompletionPort(handle, m_port.get(), key, 0);
        return ret != NULL;
    }

    bool iocp::get_completion(
        unsigned long* bytes_xfer, 
        PULONG_PTR     key, 
        LPOVERLAPPED*  ppov,
        unsigned long  timeout
        )
    {
        return ::GetQueuedCompletionStatus(
            m_port.get(),
            bytes_xfer,
            key,
            ppov,
            timeout 
            );
    }

    bool iocp::post_completion(
        unsigned long bytes_xfer, 
        ULONG_PTR     key, 
        LPOVERLAPPED  pov
        )
    {
        return ::PostQueuedCompletionStatus(m_port.get(), bytes_xfer, key, pov);
    }

    // wdl::io::register_io()

    bool register_io(iocp& port, HANDLE handle, UINT_PTR key)
    {
        return port.register_io(handle, key);
    }

    // wdl::io::get_completion()

    bool get_completion(
        iocp&          port, 
        unsigned long* bytes_xfer, 
        PULONG_PTR     key, 
        LPOVERLAPPED*  ppov, 
        unsigned long  timeout = INFINITE)
    {
        return port.get_completion(bytes_xfer, key, ppov, timeout);
    }

    // wdl::io::post_completion()

    bool post_completion(
        iocp&         port,
        unsigned long bytes_xfer,
        ULONG_PTR     key,
        LPOVERLAPPED  pov
        )
    {
        return port.post_completion(bytes_xfer, key, pov);
    }  
}