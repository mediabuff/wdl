// waitable_address.cpp
//
// Unit Test: wdl::synchronization::waitable_address

#include <catch2/catch.hpp>
#include <wdl/synchronization/event.hpp>
#include <wdl/synchronization/waitable_address.hpp>

#include <thread>

#pragma comment(lib, "synchronization.lib")

void waiter(
    wdl::synchronization::waitable_address<unsigned long>& addr,
    wdl::synchronization::event& event,
    bool& flag
    )
{
    addr.wait_until_not(0);
    
    flag = true;
    event.set();
}

void setter(
    wdl::synchronization::waitable_address<unsigned long>& addr
    )
{
    addr.write_and_wake_one(1);
}

TEST_CASE("wdl::synchronization::waitable_address synchronizes thread activity")
{
    using wdl::synchronization::event;
    using wdl::synchronization::event_type;
    using wdl::synchronization::waitable_address;

    auto target = unsigned long{0};
    auto flag   = bool{false};

    auto addr  = waitable_address{&target};

    auto e = event{event_type::manual_reset};

    auto t1 = std::thread{waiter, std::ref(addr), std::ref(e), std::ref(flag)};
    auto t2 = std::thread{setter, std::ref(addr)};

    e.wait();

    REQUIRE(flag);

    t1.join();
    t2.join();
}