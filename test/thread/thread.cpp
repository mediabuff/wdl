// thread.cpp
//
// Unit Test: 
//  - wdl::thread::make_thread()
//  - wdl::thread::thread_id()

#include <catch2/catch.hpp>
#include <wdl/thread/thread.hpp>
#include <wdl/synchronization/event.hpp>

#include <windows.h>

TEST_CASE("wdl::thread::make_thread() provides a simpler thread creation interface")
{
    using wdl::synchronization::event;
    using wdl::synchronization::event_type;

    auto e = event{event_type::manual_reset, L"test_event"};
    REQUIRE(e.is_new_instance());

    auto thread = wdl::thread::make_thread(
        []()
        { 
            auto e = event{event_type::manual_reset, L"test_event"};
            REQUIRE_FALSE(e.is_new_instance());
            e.set();
        });

    ::WaitForSingleObject(thread.get(), INFINITE);
    ::WaitForSingleObject(e.get(), INFINITE);

    // if we get here, thread has successfully executed
    REQUIRE(true);
}