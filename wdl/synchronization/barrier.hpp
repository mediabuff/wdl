// barrier.hpp
// Class Implementation: wdl::synchronization::barrier
//
// Wrapper around Windows SYNCHRONIZATION_BARRIER type.

#pragma once

#include <windows.h>

namespace wdl::synchronization
{
    // barrier
    //
    // Simple RAII wrapper around Windows built-in type.

    class barrier
    {
        SYNCHRONIZATION_BARRIER m_barrier;

    public:
        barrier(long n_threads, long spin_count = -1)
        {
            ::InitializeSynchronizationBarrier(
                &m_barrier, 
                n_threads, 
                spin_count
                );
        }

        void enter(unsigned long flags = 0)
        {
            ::EnterSynchronizationBarrier(&m_barrier, flags);
        }   

        ~barrier()
        {
            ::DeleteSynchronizationBarrier(&m_barrier);
        }
    };
}