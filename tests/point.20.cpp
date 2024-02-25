
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/area/point.hpp"

TEST_CASE("point  - constructor", "[test_20][point]")
{
    SECTION("default")
    {
        ljh::point v;
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
    }
    SECTION("x, y")
    {
        ljh::point v{1, 2};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
    }
}

TEST_CASE("point - equality", "[test_20][point]")
{
    ljh::point v;
    SECTION("equal")
    {
        ljh::point v1;
        CHECK(v.x == v1.x);
        CHECK(v.y == v1.y);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        ljh::point v1{1, 2};
        CHECK(v.x != v1.x);
        CHECK(v.y != v1.y);
        REQUIRE(v != v1);
    }
}