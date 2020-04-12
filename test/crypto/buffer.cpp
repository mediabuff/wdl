// buffer.cpp
//
// Unit Test: wdl::crypto::buffer()

#include <catch2/catch.hpp>
#include <wdl/crypto/buffer.hpp>

#include <type_traits>

TEST_CASE("wdl::crypto::buffer() constructs mutable buffer from range")
{
    constexpr const auto SIZE = 32;

    unsigned char raw[SIZE];
    auto* ptr = &raw[0];

    auto buffer = wdl::crypto::buffer(ptr, SIZE);

    REQUIRE(buffer.size() == SIZE);
    REQUIRE(std::is_same_v<decltype(buffer), wdl::crypto::mutable_buffer>);
}

TEST_CASE("wdl::crypto::buffer() construct constant buffer from range")
{
    constexpr const auto SIZE = 32;

    unsigned char raw[SIZE];
    auto const* ptr = &raw[0];

    auto buffer = wdl::crypto::buffer(ptr, SIZE);

    REQUIRE(buffer.size() == SIZE);
    REQUIRE(std::is_same_v<decltype(buffer), wdl::crypto::const_buffer>); 
}