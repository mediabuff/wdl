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
        REQUIRE(ev.valid());
    }

    SECTION("construction with manual reset")
    {
        auto ev = event{event_type::manual_reset};
        REQUIRE(ev.valid());
    }

    SECTION("move construction")
    {
        auto src = event{event_type::auto_reset};
        auto dst = event{std::move(src)};

        REQUIRE(dst.valid());
    }

    SECTION("move assignment")
    {
        auto src = event{event_type::auto_reset};
        auto dst = std::move(src);

        REQUIRE(dst.valid());
    }
}

