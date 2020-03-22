// pool.cpp
//
// Unit Test: wdl::threadpool::pool

#include <catch2/catch.hpp>

#include <wdl/handle.hpp>
#include <wdl/threadpool/pool.hpp>
#include <wdl/synchronization/event.hpp>

TEST_CASE("wdl::threadpool::pool supports work submission")
{
    using wdl::handle::work_handle;
    using wdl::synchronization::event;
    using wdl::synchronization::event_type;
    using wdl::threadpool::pool_cancellation_policy;

    auto pool = wdl::threadpool::pool{pool_cancellation_policy::no_cancel};
    REQUIRE(pool.is_valid());

    auto e = event{event_type::manual_reset, L"test_event"};
    REQUIRE(e.is_valid());
    REQUIRE(e.is_new_instance());

    {
        auto work = work_handle
        {
            pool.submit_work(
                [](PTP_WORK)
                {  
                    auto e = event{event_type::manual_reset, L"test_event"};
                    REQUIRE(e.is_valid());
                    REQUIRE_FALSE(e.is_new_instance());
                    e.set();
                })
        };

        // work handle goes out of scope here, work is waited on
    }

    ::WaitForSingleObject(e.get(), INFINITE);

    // if we get here, callback executed successfully
    REQUIRE(true);
}