
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/function_traits.hpp"

void LJH_CALLING_CONVENTION_vectorcall temp()
{}

void LJH_CALLING_CONVENTION_vectorcall temp_noexcept() noexcept
{}

struct member
{
    int LJH_CALLING_CONVENTION_vectorcall function(int a, int b) noexcept
    {
        return a + b;
    };

    int LJH_CALLING_CONVENTION_vectorcall const_function(int a, int b) const noexcept
    {
        return a + b;
    };
};

TEST_CASE("function_traits", "[function_traits]")
{
    STATIC_REQUIRE(ljh::function_traits<void() noexcept>::is::function);
    STATIC_REQUIRE(ljh::function_traits<decltype(temp)>::is::function);
    STATIC_REQUIRE(ljh::function_traits<decltype(&temp)>::is::function_pointer);
    STATIC_REQUIRE(ljh::function_traits<decltype(temp_noexcept)>::is::function);
    STATIC_REQUIRE(ljh::function_traits<decltype(&temp_noexcept)>::is::function_pointer);
    STATIC_REQUIRE(ljh::function_traits<std::function<void()>>::is::std_function);
    STATIC_REQUIRE(ljh::function_traits<std::function<void() noexcept>>::is::std_function);
    STATIC_REQUIRE(ljh::function_traits<decltype(&member::function)>::is::function_pointer);
    STATIC_REQUIRE(ljh::function_traits<decltype(&member::const_function)>::is::function_pointer);
}

TEST_CASE("function_traits no_exceptions", "[function_traits]")
{
#if __cpp_noexcept_function_type >= 201510L
#define STATIC_REQUIRE_NOEXCEPT STATIC_REQUIRE
#else
#define STATIC_REQUIRE_NOEXCEPT STATIC_REQUIRE_FALSE
#endif
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<void() noexcept>::is::no_exceptions);
    STATIC_REQUIRE_FALSE(ljh::function_traits<decltype(temp)>::is::no_exceptions);
    STATIC_REQUIRE_FALSE(ljh::function_traits<decltype(&temp)>::is::no_exceptions);
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<decltype(temp_noexcept)>::is::no_exceptions);
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<decltype(&temp_noexcept)>::is::no_exceptions);
    STATIC_REQUIRE_FALSE(ljh::function_traits<std::function<void()>>::is::no_exceptions);
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<std::function<void() noexcept>>::is::no_exceptions);
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<decltype(&member::function)>::is::no_exceptions);
    STATIC_REQUIRE_NOEXCEPT(ljh::function_traits<decltype(&member::const_function)>::is::no_exceptions);
}