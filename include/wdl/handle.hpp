// handle.hpp
//
// Convenience Header

#include "handle/crypto.hpp"
#include "handle/filesystem.hpp"
#include "handle/generic.hpp"
#include "handle/heap.hpp"
#include "handle/network.hpp"
#include "handle/pipe.hpp"
#include "handle/registry.hpp"
#include "handle/threadpool.hpp"
#include "handle/unique_handle.hpp"

namespace wdl::handle
{
    using null_handle        = unique_handle<null_handle_traits>;
    using invalid_handle     = unique_handle<invalid_handle_traits>;
    using reg_handle         = unique_handle<registry_handle_traits>;
    using winhttp_handle     = unique_handle<winhttp_handle_traits>;
    using pool_handle        = unique_handle<pool_handle_traits>;
    using work_handle        = unique_handle<tp_work_handle_traits>;
    using wait_handle        = unique_handle<tp_wait_handle_traits>;
    using timer_handle       = unique_handle<tp_timer_handle_traits>;
    using io_handle          = unique_handle<tp_io_handle_traits>;
    using cleanup_group      = unique_handle<tp_cleanup_group_handle_traits>;
    using provider_handle    = unique_handle<provider_handle_traits>;
    using hash_handle        = unique_handle<hash_handle_traits>;
    using key_handle         = unique_handle<key_handle_traits>;
    using np_client_handle   = unique_handle<np_client_handle_traits>;
    using np_server_handle   = unique_handle<np_server_handle_traits>;
    using local_alloc_handle = unique_handle<local_alloc_handle_traits>;
    using find_file_handle   = unique_handle<find_file_handle_traits>;
}