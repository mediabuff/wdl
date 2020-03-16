// pipe_acceptor.hpp

#pragma once

#include <windows.h>

#include <string>
#include <utility>
#include <stdexcept>

#include <wdl/ipc/duplex_pipe.hpp>
#include <wdl/threadpool/pool.hpp>
#include <wdl/utility/unique_handle.hpp>

namespace wdl::ipc
{
    class pipe_acceptor
    {
        std::wstring            m_name;
        wdl::threadpool::pool&  m_pool;

        PTP_IO     m_io_object;
        OVERLAPPED m_overlapped;

        using acceptor_callback_f = void(*)(unsigned long);

    public:
        pipe_acceptor(wdl::threadpool::pool& pool)
            : m_pool{ pool }, m_overlapped{}
        {}

        void listen(std::wstring const& local_name);

        template <typename Callback>
        void accept_async(
            wdl::ipc::duplex_pipe& handler_pipe,
            Callback&&             handler
            );
    };

    void pipe_acceptor::listen(std::wstring const& local_name)
    {
        m_name = local_name;
    }

    template <typename Callback>
    void pipe_acceptor::accept_async(
        wdl::ipc::duplex_pipe& handler_pipe,
        Callback&&             handler
        )
    {   
        // register the read end of the pipe with the 
        // threadpool for IO completions
        m_pool.register_io(
            handler_pipe.server_native_handle(),
            std::forward<Callback>(handler)
            );

        // setup the pipe to listen on the acceptor's interface
        handler_pipe.listen(m_name);

        // initiate the asynchrnonous accept operation
        handler_pipe.accept_async(&m_overlapped);
    }
};