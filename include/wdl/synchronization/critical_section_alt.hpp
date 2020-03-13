// critical_section_alt.hpp
// Class Implementation: wdl::synchronization::critical_section_alt
//
// Alternative implementation of critical section wrapper that 
// utilizes WaitOnAddress() as underlying synchronziation primitive.
//
// NOTE: usage requires linkage against synchronization.lib.

#pragma once

#include <windows.h>

namespace wdl::synchronization
{
    class critical_section_alt
    {
        unsigned long m_owner_thread;
        unsigned long m_claim_count;

    public:
        critical_section_alt() noexcept
            : m_owner_thread{ 0 }
            , m_claim_count{ 0 }
        {}

        critical_section_alt(critical_section_alt& other) = delete;
        critical_section_alt& operator=(critical_section_alt& rhs) = delete;
        critical_section_alt(critical_section_alt&& other) = delete;
        critical_section_alt& operator=(critical_section_alt&& rhs) = delete;

        void enter() noexcept
        {
            auto this_thread = ::GetCurrentThreadId();
            for (;;)
            {
                // swap the owner thread ID with our own if it is currently 0
                auto previous_owner = 
                    InterlockedCompareExchangeAcquire(
                        &m_owner_thread,
                        this_thread,
                        0
                        );

                // if we acquired a previously non-owned critical section,
                // or we just recursively acquired it, drop out
                if (0 == previous_owner || this_thread == previous_owner)
                {
                    m_claim_count++;
                    return;
                }

                ::WaitOnAddress(
                    &m_owner_thread, 
                    &previous_owner, 
                    sizeof(previous_owner), 
                    INFINITE
                    );
            }
        }

        void exit() noexcept
        {   
            // decrement the claim count
            // if claim count (for us) is still nonzero, retain ownership
            if (--m_claim_count != 0)
            {
                return;
            }

            // relinquish ownership
            ::InterlockedExchange(&m_owner_thread, 0);

            // wake one of the waiting threads
            ::WakeByAddressSingle(&m_owner_thread);
        }
    };
}

