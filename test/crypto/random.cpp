// random.cpp
//
// Unit Test: wdl::crypto::random()

#include <catch2/catch.hpp>
#include <wdl/crypto/random.hpp>

#include <memory>

TEST_CASE("wdl::crypto::random() supports invocation with buffer and size")
{
    constexpr const auto BUFFER_SIZE = 32;

    auto provider = wdl::crypto::open_provider(BCRYPT_RNG_ALGORITHM);

    auto buffer = std::make_unique<unsigned char[]>(BUFFER_SIZE);

    wdl::crypto::random(provider, buffer.get(), BUFFER_SIZE);

    REQUIRE(true);
}

TEST_CASE("wdl::crypto::random() supports invocation with statically-allocated buffer")
{
    constexpr const auto BUFFER_SIZE = 32;

    auto provider = wdl::crypto::open_provider(BCRYPT_RNG_ALGORITHM);

    unsigned char buffer[BUFFER_SIZE];

    wdl::crypto::random(provider, buffer);

    REQUIRE(true);
}

TEST_CASE("wdl::crypto::random() supports invocation with arbitrary structures")
{
    struct Data
    {
        int a;
        int b;
        float c;
        float d;
    };

    auto provider = wdl::crypto::open_provider(BCRYPT_RNG_ALGORITHM);

    auto d = Data{};

    wdl::crypto::random(provider, d);

    REQUIRE(true);
}