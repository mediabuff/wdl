// unique_handle.cpp
// Unit Test: wdl::utility::unique_handle

#include <catch2/catch.hpp>
#include <wdl/utility/unique_handle.hpp>

#include <windows.h>

TEST_CASE("wdl::utility::unique_handle lifetime semantics")
{
	using wdl::utility::null_handle;

	auto src = null_handle{ ::CreateEvent(nullptr, false, false, nullptr) };

	REQUIRE(src.is_valid());

	auto dst = null_handle{ std::move(src) };

	REQUIRE(dst.is_valid());
	REQUIRE_FALSE(src.is_valid());

	src = std::move(dst);

	REQUIRE(src.is_valid());
	REQUIRE_FALSE(dst.is_valid());
}

