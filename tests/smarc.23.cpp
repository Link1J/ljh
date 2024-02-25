
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

#include "ljh/smarc/node.hpp"
#include "ljh/smarc/list.hpp"
#include "ljh/smarc/ptr.hpp"
#include "ljh/smarc/ref.hpp"

struct test : ljh::smarc::node<test>
{
    bool* operator&()
    {
        return new bool;
    }
};

TEST_CASE("ljh::smarc::list range type", "[test_23][smarc_list]")
{
    SECTION("normal")
    {
        ljh::smarc::list<test> testing;
        STATIC_REQUIRE(std::ranges::range<decltype(testing)>);
        STATIC_REQUIRE(std::ranges::common_range<decltype(testing)>);
        STATIC_REQUIRE_FALSE(std::ranges::constant_range<decltype(testing)>);
    }
    SECTION("const")
    {
        ljh::smarc::list<test> const testing;
        STATIC_REQUIRE(std::ranges::range<decltype(testing)>);
        STATIC_REQUIRE(std::ranges::common_range<decltype(testing)>);
        STATIC_REQUIRE(std::ranges::constant_range<decltype(testing)>);
    }
}

TEST_CASE("ljh::smarc::list C++11 iterator types", "[test_23][smarc_list]")
{
    SECTION("normal")
    {
        ljh::smarc::list<test> testing;
        SECTION("begin/end")
        {
            auto begin = std::begin(testing);
            auto end   = std::end(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("cbegin/cend")
        {
            auto begin = std::cbegin(testing);
            auto end   = std::cend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("rbegin/rend")
        {
            auto begin = std::rbegin(testing);
            auto end   = std::rend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("crbegin/crend")
        {
            auto begin = std::crbegin(testing);
            auto end   = std::crend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("normal and const")
        {
            SECTION("begin")
            {
                auto normal   = std::begin(testing);
                auto constant = std::cbegin(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("end")
            {
                auto normal   = std::end(testing);
                auto constant = std::cend(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rbegin")
            {
                auto normal   = std::rbegin(testing);
                auto constant = std::crbegin(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rend")
            {
                auto normal   = std::rend(testing);
                auto constant = std::crend(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
        }
    }
    SECTION("const")
    {
        ljh::smarc::list<test> const testing;
        SECTION("begin/end")
        {
            auto begin = std::begin(testing);
            auto end   = std::end(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("cbegin/cend")
        {
            auto begin = std::cbegin(testing);
            auto end   = std::cend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("rbegin/rend")
        {
            auto begin = std::rbegin(testing);
            auto end   = std::rend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("crbegin/crend")
        {
            auto begin = std::crbegin(testing);
            auto end   = std::crend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("normal and const")
        {
            SECTION("begin")
            {
                auto normal   = std::begin(testing);
                auto constant = std::cbegin(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("end")
            {
                auto normal   = std::end(testing);
                auto constant = std::cend(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rbegin")
            {
                auto normal   = std::rbegin(testing);
                auto constant = std::crbegin(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rend")
            {
                auto normal   = std::rend(testing);
                auto constant = std::crend(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
        }
    }
}

TEST_CASE("ljh::smarc::list C++20 iterator types", "[test_23][smarc_list]")
{
    SECTION("normal")
    {
        ljh::smarc::list<test> testing;
        SECTION("begin/end")
        {
            auto begin = std::ranges::begin(testing);
            auto end   = std::ranges::end(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("cbegin/cend")
        {
            auto begin = std::ranges::cbegin(testing);
            auto end   = std::ranges::cend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("rbegin/rend")
        {
            auto begin = std::ranges::rbegin(testing);
            auto end   = std::ranges::rend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("crbegin/crend")
        {
            auto begin = std::ranges::crbegin(testing);
            auto end   = std::ranges::crend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("normal and const")
        {
            SECTION("begin")
            {
                auto normal   = std::ranges::begin(testing);
                auto constant = std::ranges::cbegin(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("end")
            {
                auto normal   = std::ranges::end(testing);
                auto constant = std::ranges::cend(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rbegin")
            {
                auto normal   = std::ranges::rbegin(testing);
                auto constant = std::ranges::crbegin(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rend")
            {
                auto normal   = std::ranges::rend(testing);
                auto constant = std::ranges::crend(testing);
                STATIC_REQUIRE_FALSE(std::same_as<decltype(normal), decltype(constant)>);
            }
        }
    }
    SECTION("const")
    {
        ljh::smarc::list<test> const testing;
        SECTION("begin/end")
        {
            auto begin = std::ranges::begin(testing);
            auto end   = std::ranges::end(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("cbegin/cend")
        {
            auto begin = std::ranges::cbegin(testing);
            auto end   = std::ranges::cend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("rbegin/rend")
        {
            auto begin = std::ranges::rbegin(testing);
            auto end   = std::ranges::rend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("crbegin/crend")
        {
            auto begin = std::ranges::crbegin(testing);
            auto end   = std::ranges::crend(testing);
            STATIC_REQUIRE(std::same_as<decltype(begin), decltype(end)>);
        }
        SECTION("normal and const")
        {
            SECTION("begin")
            {
                auto normal   = std::ranges::begin(testing);
                auto constant = std::ranges::cbegin(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("end")
            {
                auto normal   = std::ranges::end(testing);
                auto constant = std::ranges::cend(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rbegin")
            {
                auto normal   = std::ranges::rbegin(testing);
                auto constant = std::ranges::crbegin(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
            SECTION("rend")
            {
                auto normal   = std::ranges::rend(testing);
                auto constant = std::ranges::crend(testing);
                STATIC_REQUIRE(std::same_as<decltype(normal), decltype(constant)>);
            }
        }
    }
}

decltype(auto) setup()
{
    auto                   item = ljh::smarc::make_item<test>();
    ljh::smarc::list<test> testing;
    item->insert_after(testing.last());
    return testing;
}

void testfn()
{
    auto            testing = setup();
    ljh::smarc::ref weak    = testing.first();
    auto            weak2   = weak;
    auto            item2   = weak.lock();
    item2->insert_after(testing.last());
    item2->insert_after(testing.last());

    for (auto&& i : testing)
    {
        static_assert(std::same_as<bool*, decltype(&i)>);
    }
}

void testfnconst()
{
    auto const      testing = setup();
    ljh::smarc::ref weak    = testing.first();
    auto            weak2   = weak;
    auto            item2   = weak.lock();

    for (auto&& i : testing)
    {
        static_assert(std::same_as<test const*, decltype(&i)>);
    }
}
