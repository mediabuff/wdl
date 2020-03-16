// pipe_type.hpp

#pragma once

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
}