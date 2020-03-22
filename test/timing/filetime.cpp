// filetime.cpp
//
// Unit Test:
//  wdl::timing::to_filetime()
//  wdl::timing::from_filetime()

#include <catch2/catch.hpp>
#include <wdl/timing/filetime.hpp>

#include <chrono>

TEST_CASE("wdl::timing::to_filetime() and wdl::timing::from_filetime()")
{
    using namespace std::chrono_literals;

    using wdl::timing::to_filetime;
    using wdl::timing::from_filetime;

    auto ft = to_filetime<std::chrono::milliseconds>(100ms);
    auto ch = from_filetime<std::chrono::milliseconds>(ft);

    REQUIRE(ch.count() == std::chrono::milliseconds{100}.count());
}