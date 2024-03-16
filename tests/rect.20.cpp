
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "ljh/area/rect.hpp"
#include "ljh/area/point.hpp"
#include "ljh/area/size.hpp"

TEMPLATE_TEST_CASE("rect - constructor", "[test_20][rect]", ljh::rect, ljh::irect)
{
    SECTION("default")
    {
        TestType v;
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
        REQUIRE(v.w == 0);
        REQUIRE(v.h == 0);
    }
    SECTION("x, y, width, height")
    {
        TestType v{1, 2, 3, 4};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
    SECTION("point, size")
    {
        typename TestType::point_type p{1, 2};
        typename TestType::size_type  s{3, 4};
        TestType                      v{p, s};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEMPLATE_TEST_CASE("rect - equality", "[test_20][rect]", ljh::rect, ljh::irect)
{
    TestType v;
    SECTION("equal")
    {
        TestType v1;
        CHECK(v.x == v1.x);
        CHECK(v.y == v1.y);
        CHECK(v.w == v1.w);
        CHECK(v.h == v1.h);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        TestType v1{1, 2, 3, 4};
        CHECK(v.x != v1.x);
        CHECK(v.y != v1.y);
        CHECK(v.w != v1.w);
        CHECK(v.h != v1.h);
        REQUIRE(v != v1);
    }
}

TEMPLATE_TEST_CASE("rect - structured binding", "[test_20][rect]", float, int)
{
    using rect  = ljh::trect<TestType>;
    using point = ljh::tpoint<TestType>;
    using size  = ljh::tsize<TestType>;

    rect v{1, 2, 3, 4};
    auto&& [p, s] = v;
    STATIC_REQUIRE(std::same_as<decltype(p), point>);
    STATIC_REQUIRE(std::same_as<decltype(s), size>);
    REQUIRE(p == point{1, 2});
    REQUIRE(s == size{3, 4});
    REQUIRE(p == v.position());
    REQUIRE(s == v.size());
}

TEMPLATE_TEST_CASE("rect - operators", "[test_20][rect]", float, int)
{
    using rect  = ljh::trect<TestType>;
    using point = ljh::tpoint<TestType>;
    using size  = ljh::tsize<TestType>;

    rect v{1, 2, 3, 4};
    SECTION("operator+")
    {
        SECTION("point")
        {
            auto v1 = v + point{5, 6};
            REQUIRE(v == rect{1, 2, 3, 4});
            REQUIRE(v1.position() == point{6, 8});
            REQUIRE(v1 == rect{6, 8, 3, 4});
        }
        SECTION("size")
        {
            auto v1 = v + size{5, 6};
            REQUIRE(v == rect{1, 2, 3, 4});
            REQUIRE(v1.size() == size{8, 10});
            REQUIRE(v1 == rect{1, 2, 8, 10});
        }
    }
    SECTION("operator+=")
    {
        SECTION("point")
        {
            v += point{5, 6};
            REQUIRE(v != rect{1, 2, 3, 4});
            REQUIRE(v.position() == point{6, 8});
            REQUIRE(v == rect{6, 8, 3, 4});
        }
        SECTION("size")
        {
            v += size{5, 6};
            REQUIRE(v != rect{1, 2, 3, 4});
            REQUIRE(v.size() == size{8, 10});
            REQUIRE(v == rect{1, 2, 8, 10});
        }
    }
}

static void value(ljh::rect v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}
static void lvalue(ljh::rect const& v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}
static void rvalue(ljh::rect&& v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}

// This are really compile tests.
TEMPLATE_TEST_CASE_SIG("rect - function call constructors", "[test_20][rect]", ((auto FN), FN), (value), (lvalue), (rvalue))
{
    SECTION("direct-initialization")
    {
        FN(ljh::rect(), true);
        FN(ljh::rect(1, 2, 3, 4), false);
        FN(ljh::rect({}, {}, {}, {}), true);
        FN(ljh::rect({}, {}), true);
        FN(ljh::rect({1, 2}, {3, 4}), false);
    }
    SECTION("list-initialization with type")
    {
        FN(ljh::rect{}, true);
        FN(ljh::rect{1, 2, 3, 4}, false);
        FN(ljh::rect{{}, {}, {}, {}}, true);
        FN(ljh::rect{{}, {}}, true);
        FN(ljh::rect{{1, 2}, {3, 4}}, false);
    }
    SECTION("list-initialization without type")
    {
        FN({}, true);
        FN({1, 2, 3, 4}, false);
        FN({{}, {}, {}, {}}, true);
        FN({{}, {}}, true);
        FN({{1, 2}, {3, 4}}, false);
    }
}

// This is really compile test.
TEMPLATE_TEST_CASE_SIG("rect - assignment from other rect", "[test_20][rect]", ((typename F, typename T, int V), F, T, V), (ljh::rect, ljh::irect, 0),
                       (ljh::irect, ljh::rect, 0))
{
    F v{1, 2, 3, 4};
    SECTION("via converting constructor")
    {
        auto v1 = T{v};
        REQUIRE(v1.x == 1);
        REQUIRE(v1.y == 2);
        REQUIRE(v1.w == 3);
        REQUIRE(v1.h == 4);
        REQUIRE(v1.x == v.x);
        REQUIRE(v1.y == v.y);
        REQUIRE(v1.w == v.w);
        REQUIRE(v1.h == v.h);
    }
}
