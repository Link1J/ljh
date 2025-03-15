
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/area/point.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

namespace Catch
{
    template<typename T>
    concept formattable = requires(T const& value) {
        { std::format("{}", value) } -> std::same_as<std::string>;
    };

    template<formattable T>
    struct StringMaker<T>
    {
        static std::string convert(T const& value)
        {
            return std::format("{}", value);
        } // namespace Catch
    };
} // namespace Catch

TEMPLATE_TEST_CASE("basic_point - constructor", "[test_23][point]", float, int)
{
    SECTION("x, y")
    {
        ljh::basic_point v{TestType{1}, TestType{2}};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
    }
}

TEMPLATE_TEST_CASE("point  - constructor", "[test_23][point]", ljh::point, ljh::ipoint)
{
    SECTION("default")
    {
        TestType v;
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
    }
    SECTION("x, y")
    {
        TestType v{1, 2};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
    }
}

TEMPLATE_TEST_CASE("point - equality", "[test_23][point]", ljh::point, ljh::ipoint)
{
    TestType v;
    SECTION("equal")
    {
        TestType v1;
        CHECK(v.x == v1.x);
        CHECK(v.y == v1.y);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        TestType v1{1, 2};
        CHECK(v.x != v1.x);
        CHECK(v.y != v1.y);
        REQUIRE(v != v1);
    }
}

static void value(ljh::point v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
}
static void lvalue(ljh::point const& v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
}
static void rvalue(ljh::point&& v, bool _default)
{
    CHECK(v.x == (_default ? 0 : 1));
    CHECK(v.y == (_default ? 0 : 2));
}

// This are really compile tests.
TEMPLATE_TEST_CASE_SIG("point - function call constructors", "[test_23][point]", ((auto FN), FN), (value), (lvalue), (rvalue))
{
    SECTION("direct-initialization")
    {
        FN(ljh::point(), true);
        FN(ljh::point(1, 2), false);
        FN(ljh::point({}, {}), true);
        FN(ljh::point({1, 2}), false); // What?
    }
    SECTION("list-initialization with type")
    {
        FN(ljh::point{}, true);
        FN(ljh::point{1, 2}, false);
        FN(ljh::point{{}, {}}, true);
    }
    SECTION("list-initialization without type")
    {
        FN({}, true);
        FN({1, 2}, false);
        FN({{}, {}}, true);
    }
}

// This is really compile test.
TEMPLATE_TEST_CASE_SIG("point - assignment from other point", "[test_23][point]", ((typename F, typename T, int V), F, T, V), (ljh::point, ljh::ipoint, 0),
                       (ljh::ipoint, ljh::point, 0))
{
    F v{1, 2};
    SECTION("via converting constructor")
    {
        auto v1 = T{v};
        REQUIRE(v1.x == 1);
        REQUIRE(v1.y == 2);
        REQUIRE(v1.x == v.x);
        REQUIRE(v1.y == v.y);
    }
}
