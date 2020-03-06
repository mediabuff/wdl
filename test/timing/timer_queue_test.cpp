// timer_queue_test.cpp
// Unit Test: wdl::timing::timer_queue
//
// Build
//  cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL timer_queue_test.cpp

#include <windows.h>

#include <thread>
#include <chrono>
#include <iostream>

#include "wdl/timing/timer_queue.hpp"
#include "wdl/synchronization/wait.hpp"

HANDLE g_event;

void __stdcall callback(void*, int)
{
    std::cout << "Executing timer queue callback\n";
    ::SetEvent(g_event);
}

int main()
{
    using namespace std::chrono_literals;

    wdl::timing::timer_queue timer_queue{};

    g_event = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

    auto h = timer_queue.submit(
        callback,
        nullptr,
        5s,
        0s
        );

    ::WaitForSingleObject(g_event, INFINITE);
    ::CloseHandle(g_event);

    ::DeleteTimerQueueTimer(timer_queue.get(), h, NULL);

    std::cout << "Exiting\n";
}
