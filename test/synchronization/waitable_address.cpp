// waitable_address.cpp
// Unit Test: wdl::synchronization::waitable_address
//
// Build
//  cl /EHsc /nologo /W4 /std:c++17 /I C:\Dev\WDL waitable_address_test.cpp

#include <chrono>
#include <thread>
#include <iostream>
#include <windows.h>

#include "wdl/synchronization/waitable_address.hpp"

#pragma comment(lib, "synchronization.lib")

using wdl::synchronization::waitable_address;

unsigned long g_target = 0;
waitable_address<unsigned long> g_addr{&g_target};

void waiter()
{
    g_addr.wait_until_not(0);

    std::cout << "waitable_address wait satisfied ";
    std::cout << "g_target = " << g_target << '\n';
}

void setter()
{
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(3s);

    g_addr.write_and_wake_one(1);
}

int main()
{
    auto t1 = std::thread{waiter};
    auto t2 = std::thread{setter};

    t1.join();
    t2.join();
}