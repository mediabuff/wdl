// handle.hpp
//
// Convenience Header

#include "handle/generic.hpp"
#include "handle/heap.hpp"
#include "handle/network.hpp"
#include "handle/pipe.hpp"
#include "handle/unique_handle.hpp"

namespace wdl::handle
{
    using null_handle        = unique_handle<null_handle_traits>;
    using invalid_handle     = unique_handle<invalid_handle_traits>;
    using winhttp_handle     = unique_handle<winhttp_handle_traits>;
    using np_client_handle   = unique_handle<np_client_handle_traits>;
    using np_server_handle   = unique_handle<np_server_handle_traits>;
    using local_alloc_handle = unique_handle<local_alloc_handle_traits>;
}