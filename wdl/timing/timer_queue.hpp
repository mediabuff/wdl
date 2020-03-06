// timer_queue.hpp
// Class Implementation: wdl::timing::timer_queue.hpp
//
// Wrapper around Windows timer queue.

#pragma once

#include <windows.h>

#include <chrono>
#include <functional>

namespace wdl::timing
{
    // timer_queue

    class timer_queue
    {
        using timer_callback_t = void (__stdcall*)(void*, int);
        
        HANDLE m_timer_queue;

    public:
        timer_queue()
            : m_timer_queue{ ::CreateTimerQueue() }
        {}

        ~timer_queue()
        {
            ::DeleteTimerQueue(m_timer_queue);
        }

        template <typename DueTimeDuration, 
                  typename PeriodDuration>
        HANDLE submit(
            timer_callback_t cb,
            void* arg,
            DueTimeDuration due_time,
            PeriodDuration period,
            unsigned long flags = 0
            )
        {
            HANDLE timer_handle;

            auto due_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(due_time);
            auto period_ms = std::chrono::duration_cast<std::chrono::milliseconds>(period);

            ::CreateTimerQueueTimer(
                &timer_handle, 
                m_timer_queue,
                (WAITORTIMERCALLBACK)cb,
                arg,
                due_time_ms.count(),
                period.count(),
                flags
                );

            return timer_handle;
        }

        HANDLE get() const noexcept
        {
            return m_timer_queue;
        }
    };
}