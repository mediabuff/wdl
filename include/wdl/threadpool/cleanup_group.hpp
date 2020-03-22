// cleanup_group.hpp
//
// Class Implementation: wdl::threadpool::cleanup_group

#pragma once

#include <windows.h>

namespace wdl::threadpool
{
    // wdl::threadpool::cleanup_group_cancellation_policy
    //
    // Determines behavior of cleanup group member closure.

    enum class cleanup_group_cancellation_policy
    {
        no_cancel,
        cancel
    };

    // wdl::threadpool::cleanup_group
    //
    // RAII wrapper around Windows threadpool cleanup group.

    class cleanup_group
    {
        PTP_CLEANUP_GROUP                 m_group;
        cleanup_group_cancellation_policy m_policy;

    public:
        cleanup_group(cleanup_group_cancellation_policy policy
            = cleanup_group_cancellation_policy::no_cancel) 
            : m_group{ ::CreateThreadpoolCleanupGroup() }
        {}

        ~cleanup_group()
        {  
            ::CloseThreadpoolCleanupGroupMembers(
                m_group, 
                static_cast<bool>(m_policy), 
                nullptr);

            ::CloseThreadpoolCleanupGroup(m_group);
        }

        // non-copyable, non-movable
        cleanup_group(cleanup_group const&)            = delete;
        cleanup_group& operator=(cleanup_group const&) = delete;
        cleanup_group(cleanup_group&&)                 = delete;
        cleanup_group& operator=(cleanup_group&&)      = delete;

        void set_cancellation_policy(cleanup_group_cancellation_policy policy)
        {
            m_policy = policy;
        }

        PTP_CLEANUP_GROUP get() const noexcept
        {
            return m_group;
        }
    };
}