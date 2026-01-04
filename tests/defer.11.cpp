
//          Copyright Jared Irwin 2024-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/defer.hpp"

#include <catch2/catch_test_macros.hpp>

#if LJH_CPP_VERSION <= LJH_CPP17_VERSION
template<template<typename> class Scope, typename T>
Scope<T> make(T&& f) noexcept(std::is_nothrow_constructible<Scope<T>, T>::value)
{
    return Scope<T>{std::forward<T>(f)};
}
#endif

template<template<typename> class Scope>
int no_exception(int& count)
{
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    Scope _{[&count]() mutable { count++; }};
#else
    auto _ = make<Scope>([&count]() mutable { count++; });
#endif
    return 1;
}

template<template<typename> class Scope>
int exception(int& count)
{
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    Scope _{[&count]() mutable { count++; }};
#else
    auto _ = make<Scope>([&count]() mutable { count++; });
#endif
    throw 1;
}

#if LJH_CPP_VERSION <= LJH_CPP17_VERSION
template<template<typename> class Scope, typename D, typename T>
Scope<T> make_with_default(D default_state, T&& f) noexcept(std::is_nothrow_constructible<Scope<T>, D, T>::value)
{
    return Scope<T>{default_state, std::forward<T>(f)};
}
#endif

template<template<typename> class Scope, typename T, typename F>
int no_exception_action(T default_state, int& count, F&& action)
{
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    Scope _{default_state, [&count]() mutable { count++; }};
#else
    auto _ = make_with_default<Scope>(default_state, [&count]() mutable { count++; });
#endif
    action(_);
    return 1;
}

template<template<typename> class Scope, typename T, typename F>
int exception_action(T default_state, int& count, F&& action)
{
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    Scope _{default_state, [&count]() mutable { count++; }};
#else
    auto _ = make_with_default<Scope>(default_state, [&count]() mutable { count++; });
#endif
    action(_);
    throw 1;
}

TEST_CASE("scope_exit", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("No Exception")
    {
        CHECK(no_exception<ljh::scope_exit>(count) == 1);
        REQUIRE(count == 1);
    }
    SECTION("Exception")
    {
        CHECK_THROWS(exception<ljh::scope_exit>(count));
        REQUIRE(count == 1);
    }
}

TEST_CASE("scope_fail", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("No Exception")
    {
        CHECK(no_exception<ljh::scope_fail>(count) == 1);
        REQUIRE(count == 0);
    }
    SECTION("Exception")
    {
        CHECK_THROWS(exception<ljh::scope_fail>(count));
        REQUIRE(count == 1);
    }
}

TEST_CASE("scope_success", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("No Exception")
    {
        CHECK(no_exception<ljh::scope_success>(count) == 1);
        REQUIRE(count == 1);
    }
    SECTION("Exception")
    {
        CHECK_THROWS(exception<ljh::scope_success>(count));
        REQUIRE(count == 0);
    }
}

TEST_CASE("condition_scope_exit", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("Enabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_exit>(true, count, action) == 1);
                REQUIRE(count == 1);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_exit>(true, count, action));
                REQUIRE(count == 1);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_inactive(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_exit>(true, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_exit>(true, count, action));
                REQUIRE(count == 0);
            }
        }
    }
    SECTION("Disabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_exit>(false, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_exit>(false, count, action));
                REQUIRE(count == 0);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_active(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_exit>(false, count, action) == 1);
                REQUIRE(count == 1);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_exit>(false, count, action));
                REQUIRE(count == 1);
            }
        }
    }
}

TEST_CASE("condition_scope_fail", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("Enabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_fail>(true, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_fail>(true, count, action));
                REQUIRE(count == 1);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_inactive(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_fail>(true, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_fail>(true, count, action));
                REQUIRE(count == 0);
            }
        }
    }
    SECTION("Disabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_fail>(false, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_fail>(false, count, action));
                REQUIRE(count == 0);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_active(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_fail>(false, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_fail>(false, count, action));
                REQUIRE(count == 1);
            }
        }
    }
}


TEST_CASE("condition_scope_success", "[test_11][defer]")
{
    int count = 0;
    CHECK(count == 0);
    SECTION("Enabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_success>(true, count, action) == 1);
                REQUIRE(count == 1);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_success>(true, count, action));
                REQUIRE(count == 0);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_inactive(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_success>(true, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_success>(true, count, action));
                REQUIRE(count == 0);
            }
        }
    }
    SECTION("Disabled By Default")
    {
        SECTION("Default")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) {};
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_success>(false, count, action) == 1);
                REQUIRE(count == 0);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_success>(false, count, action));
                REQUIRE(count == 0);
            }
        }
        SECTION("State Changed")
        {
            LJH_CPP17_CONSTEXPR auto action = [](auto& d) { d.make_active(); };
            SECTION("No Exception")
            {
                CHECK(no_exception_action<ljh::condition_scope_success>(false, count, action) == 1);
                REQUIRE(count == 1);
            }
            SECTION("Exception")
            {
                CHECK_THROWS(exception_action<ljh::condition_scope_success>(false, count, action));
                REQUIRE(count == 0);
            }
        }
    }
}
