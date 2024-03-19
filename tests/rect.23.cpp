
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/area/rect.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

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

TEMPLATE_TEST_CASE("trect - constructor", "[test_23][rect]", float, int)
{
    SECTION("x, y, width, height")
    {
        ljh::trect v{TestType{1}, TestType{2}, TestType{3}, TestType{4}};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
    SECTION("point, size")
    {
        ljh::tpoint<TestType> p{1, 2};
        ljh::tsize<TestType>  s{3, 4};
        ljh::trect            v{p, s};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEMPLATE_TEST_CASE("rect - constructor", "[test_23][rect]", ljh::rect, ljh::irect)
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
    SECTION("point, point")
    {
        typename TestType::point_type p{1, 2};
        typename TestType::point_type s{4, 6};
        TestType                      v{p, s};
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 2);
        REQUIRE(v.w == 3);
        REQUIRE(v.h == 4);
    }
}

TEMPLATE_TEST_CASE("rect - equality", "[test_23][rect]", ljh::rect, ljh::irect)
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

TEMPLATE_TEST_CASE("rect - operators", "[test_23][rect]", float, int)
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
TEMPLATE_TEST_CASE_SIG("rect - function call constructors", "[test_23][rect]", ((auto FN), FN), (value), (lvalue), (rvalue))
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
TEMPLATE_TEST_CASE_SIG("rect - assignment from other rect", "[test_23][rect]", ((typename F, typename T, int V), F, T, V), (ljh::rect, ljh::irect, 0),
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

TEMPLATE_TEST_CASE("rect - structured binding", "[test_23][rect]", float, int)
{
    using rect  = ljh::trect<TestType>;
    using point = ljh::tpoint<TestType>;
    using size  = ljh::tsize<TestType>;

    SECTION("basic")
    {
        rect v{1, 2, 3, 4};
        auto&& [p, s] = v;
        STATIC_CHECK(std::same_as<decltype(p), point>);
        STATIC_CHECK(std::same_as<decltype(s), size>);
        REQUIRE(p == point{1, 2});
        REQUIRE(s == size{3, 4});
        REQUIRE(p == v.position());
        REQUIRE(s == v.size());
    }

    rect item;
    CHECK(item.x == 0);
    CHECK(item.y == 0);
    CHECK(item.w == 0);
    CHECK(item.h == 0);

    SECTION("from constructor")
    {
        auto [p, s] = rect{};
        STATIC_CHECK(std::same_as<decltype(p), point>);
        STATIC_CHECK(std::same_as<decltype(s), size>);
    }
    SECTION("mutable value")
    {
        auto [p, s] = item;
        STATIC_CHECK(std::same_as<decltype(p), point>);
        STATIC_CHECK(std::same_as<decltype(s), size>);

        p.x = 1;
        p.y = 2;
        s.w = 3;
        s.h = 4;

        REQUIRE(item.x == 0);
        REQUIRE(item.y == 0);
        REQUIRE(item.w == 0);
        REQUIRE(item.h == 0);
    }
    SECTION("const value")
    {
        auto const [p, s] = item;
        STATIC_CHECK(std::same_as<decltype(p), point const>);
        STATIC_CHECK(std::same_as<decltype(s), size const>);

        STATIC_REQUIRE_FALSE(requires { p.x = 1; });
        STATIC_REQUIRE_FALSE(requires { p.y = 2; });
        STATIC_REQUIRE_FALSE(requires { s.w = 3; });
        STATIC_REQUIRE_FALSE(requires { s.h = 4; });

        REQUIRE(item.x == 0);
        REQUIRE(item.y == 0);
        REQUIRE(item.w == 0);
        REQUIRE(item.h == 0);
    }
    SECTION("mutable lvalue")
    {
        auto&& [p, s] = item;
        STATIC_CHECK(std::same_as<decltype(p), point>);
        STATIC_CHECK(std::same_as<decltype(s), size>);

        p.x = 1;
        p.y = 2;
        s.w = 3;
        s.h = 4;

        REQUIRE(item.x == 1);
        REQUIRE(item.y == 2);
        REQUIRE(item.w == 3);
        REQUIRE(item.h == 4);
    }
    SECTION("const lvalue")
    {
        auto&& [p, s] = std::as_const(item);
        STATIC_CHECK(std::same_as<decltype(p), point const>);
        STATIC_CHECK(std::same_as<decltype(s), size const>);

        STATIC_REQUIRE_FALSE(requires { p.x = 1; });
        STATIC_REQUIRE_FALSE(requires { p.y = 2; });
        STATIC_REQUIRE_FALSE(requires { s.w = 3; });
        STATIC_REQUIRE_FALSE(requires { s.h = 4; });

        REQUIRE(item.x == 0);
        REQUIRE(item.y == 0);
        REQUIRE(item.w == 0);
        REQUIRE(item.h == 0);
    }
    SECTION("mutable rvalue")
    {
        auto&& [p, s] = std::move(item);
        STATIC_CHECK(std::same_as<decltype(p), point>);
        STATIC_CHECK(std::same_as<decltype(s), size>);
        CHECK((void*)std::addressof(p) == (void*)std::addressof(item));

        p.x = 1;
        p.y = 2;
        s.w = 3;
        s.h = 4;

        REQUIRE(item.x == 1);
        REQUIRE(item.y == 2);
        REQUIRE(item.w == 3);
        REQUIRE(item.h == 4);
    }
    SECTION("const rvalue")
    {
        auto&& [p, s] = std::move(std::as_const(item));
        STATIC_CHECK(std::same_as<decltype(p), point const>);
        STATIC_CHECK(std::same_as<decltype(s), size const>);

        STATIC_REQUIRE_FALSE(requires { p.x = 1; });
        STATIC_REQUIRE_FALSE(requires { p.y = 2; });
        STATIC_REQUIRE_FALSE(requires { s.w = 3; });
        STATIC_REQUIRE_FALSE(requires { s.h = 4; });

        REQUIRE(item.x == 0);
        REQUIRE(item.y == 0);
        REQUIRE(item.w == 0);
        REQUIRE(item.h == 0);
    }
}


// This is just a sanity check.
#include <tuple>
TEST_CASE("Tuple tests", "[.]")
{
    SECTION("1")
    {
        std::tuple<int, float> test(1, 2.f);
        auto&& [p, s] = std::move(test);
        STATIC_CHECK(std::same_as<decltype(p), int>);
        STATIC_CHECK(std::same_as<decltype(s), float>);
        p = 3;
        s = 4.f;
        CHECK(get<0>(test) == 1);
        CHECK(get<1>(test) == 2.f);
    }
    SECTION("2")
    {
        std::tuple<int, float> test(1, 2.f);
        auto&&                 hidden = std::move(test);
        decltype(auto)         p      = get<0>(hidden);
        decltype(auto)         s      = get<1>(hidden);
        STATIC_CHECK(std::same_as<decltype(p), int&>);
        STATIC_CHECK(std::same_as<decltype(s), float&>);
        p = 3;
        s = 4.f;
        CHECK(get<0>(test) == 1);
        CHECK(get<1>(test) == 2.f);
    }
    SECTION("3")
    {
        std::tuple<int, float> test(1, 2.f);
        decltype(auto)         p = get<0>(std::move(test));
        decltype(auto)         s = get<1>(std::move(test));
        STATIC_CHECK(std::same_as<decltype(p), int&&>);
        STATIC_CHECK(std::same_as<decltype(s), float&&>);
        p = 3;
        s = 4.f;
        CHECK(get<0>(test) == 1);
        CHECK(get<1>(test) == 2.f);
    }
}