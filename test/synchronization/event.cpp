// event.cpp
// Unit Test: wdl::synchronization::event

#include <catch2/catch.hpp>
#include <wdl/synchronization/event.hpp>

#include <utility>

TEST_CASE("wdl::synchronization::event lifetime management")
{
    using wdl::synchronization::event;
    using wdl::synchronization::event_type;

    SECTION("construction with auto reset")
    {
        auto ev = event{event_type::auto_reset};
        REQUIRE(ev.is_valid());
    }

    SECTION("construction with manual reset")
    {
        auto ev = event{event_type::manual_reset};
        REQUIRE(ev.is_valid());
    }

    SECTION("move construction")
    {
        auto src = event{event_type::auto_reset};
        auto dst = event{std::move(src)};

        REQUIRE(dst.is_valid());
    }

    SECTION("move assignment")
    {
        auto src = event{event_type::auto_reset};
        auto dst = std::move(src);

        REQUIRE(dst.is_valid());
    }
}

TEST_CASE("wdl::synchronization::event provides ability to create named event")
{
    using wdl::synchronization::event;
    using wdl::synchronization::event_type;

    auto e1 = event{event_type::manual_reset, L"test_event"};
    REQUIRE(e1.is_valid());
    REQUIRE(e1.is_new_instance());

    auto e2 = event{event_type::manual_reset, L"test_event"};
    REQUIRE(e2.is_valid());
    REQUIRE_FALSE(e2.is_new_instance());
}

