
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "ljh/area/rect.hpp"
#include "ljh/area/point.hpp"
#include "ljh/area/size.hpp"

TEST_CASE("rect - constructor", "[test_20][rect]")
{
    SECTION("default")
    {
        ljh::rect v;
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
        REQUIRE(v.w == 0);
        REQUIRE(v.h == 0);
    }
    SECTION("x, y, width, height")
    {
        ljh::rect v{1, 2, 3, 4};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEST_CASE("rect - equality", "[test_20][rect]")
{
    ljh::rect v;
    SECTION("equal")
    {
        ljh::rect v1;
        CHECK(v.x == v1.x);
        CHECK(v.y == v1.y);
        CHECK(v.w == v1.w);
        CHECK(v.h == v1.h);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        ljh::rect v1{1, 2, 3, 4};
        CHECK(v.x != v1.x);
        CHECK(v.y != v1.y);
        CHECK(v.w != v1.w);
        CHECK(v.h != v1.h);
        REQUIRE(v != v1);
    }
}

TEST_CASE("rect - structured binding", "[test_20][rect]")
{
    ljh::rect v{1, 2, 3, 4};
    auto&& [point, size] = v;
    STATIC_REQUIRE(std::same_as<decltype(point), ljh::point>);
    STATIC_REQUIRE(std::same_as<decltype(size), ljh::size>);
    REQUIRE(point == ljh::point{1, 2});
    REQUIRE(size == ljh::size{3, 4});
    REQUIRE(point == v.position());
    REQUIRE(size == v.size());
}

TEST_CASE("rect - operators", "[test_20][rect]")
{
    ljh::rect v{1, 2, 3, 4};
    SECTION("operator+")
    {
        SECTION("point")
        {
            auto v1 = v + ljh::point{5, 6};
            REQUIRE(v == ljh::rect{1, 2, 3, 4});
            REQUIRE(v1.position() == ljh::point{6, 8});
            REQUIRE(v1 == ljh::rect{6, 8, 3, 4});
        }
        SECTION("size")
        {
            auto v1 = v + ljh::size{5, 6};
            REQUIRE(v == ljh::rect{1, 2, 3, 4});
            REQUIRE(v1.size() == ljh::size{8, 10});
            REQUIRE(v1 == ljh::rect{1, 2, 8, 10});
        }
    }
    SECTION("operator+=")
    {
        SECTION("point")
        {
            v += ljh::point{5, 6};
            REQUIRE(v != ljh::rect{1, 2, 3, 4});
            REQUIRE(v.position() == ljh::point{6, 8});
            REQUIRE(v == ljh::rect{6, 8, 3, 4});
        }
        SECTION("size")
        {
            v += ljh::size{5, 6};
            REQUIRE(v != ljh::rect{1, 2, 3, 4});
            REQUIRE(v.size() == ljh::size{8, 10});
            REQUIRE(v == ljh::rect{1, 2, 8, 10});
        }
    }
}