
//          Copyright Jared Irwin 2024-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/area/size.hpp"

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

TEMPLATE_TEST_CASE("basic_size - constructor", "[test_23][size]", float, int)
{
    SECTION("w, h")
    {
        ljh::basic_size v{TestType{3}, TestType{4}};
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEMPLATE_TEST_CASE("size - constructor", "[test_23][size]", ljh::size, ljh::isize)
{
    SECTION("default")
    {
        TestType v;
        REQUIRE(v.w == 0);
        REQUIRE(v.h == 0);
    }
    SECTION("width, height")
    {
        TestType v{3, 4};
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEMPLATE_TEST_CASE("size - equality", "[test_23][size]", ljh::size, ljh::isize)
{
    TestType v;
    SECTION("equal")
    {
        TestType v1;
        CHECK(v.w == v1.w);
        CHECK(v.h == v1.h);
        REQUIRE(v == v1);
    }
    SECTION("not equal")
    {
        TestType v1{3, 4};
        CHECK(v.w != v1.w);
        CHECK(v.h != v1.h);
        REQUIRE(v != v1);
    }
}

static void value(ljh::size v, bool _default)
{
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}
static void lvalue(ljh::size const& v, bool _default)
{
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}
static void rvalue(ljh::size&& v, bool _default)
{
    CHECK(v.w == (_default ? 0 : 3));
    CHECK(v.h == (_default ? 0 : 4));
}

// This is really compile test.
TEMPLATE_TEST_CASE_SIG("size - function call constructors", "[test_23][size]", ((auto FN), FN), (value), (lvalue), (rvalue))
{
    SECTION("direct-initialization")
    {
        FN(ljh::size(), true);
        FN(ljh::size(3, 4), false);
        FN(ljh::size({}, {}), true);
        FN(ljh::size({3, 4}), false); // What?
    }
    SECTION("list-initialization with type")
    {
        FN(ljh::size{}, true);
        FN(ljh::size{3, 4}, false);
        FN(ljh::size{{}, {}}, true);
    }
    SECTION("list-initialization without type")
    {
        FN({}, true);
        FN({3, 4}, false);
        FN({{}, {}}, true);
    }
}

// This is really compile test.
TEMPLATE_TEST_CASE_SIG("size - assignment from other size", "[test_23][size]", ((typename F, typename T, int V), F, T, V), (ljh::size, ljh::isize, 0),
                       (ljh::isize, ljh::size, 0))
{
    F v{3, 4};
    SECTION("via converting constructor")
    {
        auto v1 = T{v};
        REQUIRE(v1.w == 3);
        REQUIRE(v1.h == 4);
        REQUIRE(v1.w == v.w);
        REQUIRE(v1.h == v.h);
    }
}

#include <ljh/checked_math.hpp>

TEMPLATE_TEST_CASE("basic_size - add", "[test_23][size]", float, int, ljh::checked_int)
{
    ljh::basic_size v{TestType{3}, TestType{4}};
    CHECK(v.w == 3);
    CHECK(v.h == 4);
    v += v;
    CHECK(v.w == 6);
    CHECK(v.h == 8);
}