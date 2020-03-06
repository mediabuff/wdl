// slist.hpp
// Class Implementation: wdl::interlocked::slist
//
// Wrapper class around Windows lock-free singly-linked list.

#pragma once

#include <windows.h>
#include <malloc.h>

namespace wdl::interlocked
{
    // slist

    class alignas(MEMORY_ALLOCATION_ALIGNMENT) slist
    {
        SLIST_HEADER m_head;

    public:
        slist()
        {
            ::InitializeSListHead(&m_head);
        }

        void push_entry(PSLIST_ENTRY entry) noexcept
        {
            ::InterlockedPushEntrySList(&m_head, entry);
        }

        PSLIST_ENTRY pop_entry() noexcept
        {
            return ::InterlockedPopEntrySList(&m_head);
        }

        unsigned short size() noexcept
        {
            return ::QueryDepthSList(&m_head);
        }

        void flush() noexcept
        {
            ::InterlockedFlushSList(&m_head);
        }
    };
}