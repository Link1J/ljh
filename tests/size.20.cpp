
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/area/size.hpp"

TEST_CASE("size - constructor", "[test_20][size]")
{
    SECTION("default")
    {
        ljh::size v;
        REQUIRE(v.w == 0);
        REQUIRE(v.h == 0);
    }
    SECTION("width, height")
    {
        ljh::size v{3, 4};
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEST_CASE("size - equality", "[test_20][size]")
{
    ljh::size v;
    SECTION("equal")
    {
        ljh::size v1;
        CHECK(v.w == v1.w);
        CHECK(v.h == v1.h);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        ljh::size v1{3, 4};
        CHECK(v.w != v1.w);
        CHECK(v.h != v1.h);
        REQUIRE(v != v1);
    }
}