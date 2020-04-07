// region.cpp
// Unit Test: wdl::memory::region

#include <catch2/catch.hpp>
#include <wdl/memory/util.hpp>
#include <wdl/memory/virtual.hpp>

TEST_CASE("wdl::memory::region support manual construction")
{
    using wdl::memory::region;
    using wdl::memory::region_type;

    auto size = wdl::memory::page_size();

    auto r = region{region_type::reserve, size, PAGE_READWRITE};

    REQUIRE(r.is_valid());
}

TEST_CASE("wdl::memory::region provides convenience factory functions")
{
    using wdl::memory::region;

    SECTION("reserved memory")
    {
        auto size = wdl::memory::page_size();
        auto r = region::reserve(size, PAGE_READWRITE);

        REQUIRE(r.is_valid());
    }   

    SECTION("committed memory")
    {
        auto size = wdl::memory::page_size();
        auto r = region::commit(size, PAGE_READWRITE);

        REQUIRE(r.is_valid());
    }
}