
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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

    static_assert(std::ranges::range<decltype(testing)>);
    static_assert(std::ranges::common_range<decltype(testing)>);
    static_assert(!std::ranges::constant_range<decltype(testing)>);

    auto begin   = std::begin(testing);
    auto end     = std::end(testing);
    auto cbegin  = std::cbegin(testing);
    auto cend    = std::cend(testing);
    auto rbegin  = std::rbegin(testing);
    auto rend    = std::rend(testing);
    auto crbegin = std::crbegin(testing);
    auto crend   = std::crend(testing);

    static_assert(std::same_as<decltype(begin), decltype(end)>);
    static_assert(std::same_as<decltype(cbegin), decltype(cend)>);
    static_assert(std::same_as<decltype(rbegin), decltype(rend)>);
    static_assert(std::same_as<decltype(crbegin), decltype(crend)>);

    static_assert(!std::same_as<decltype(begin), decltype(cbegin)>);
    static_assert(!std::same_as<decltype(end), decltype(cend)>);
    static_assert(!std::same_as<decltype(rbegin), decltype(crbegin)>);
    static_assert(!std::same_as<decltype(rend), decltype(crend)>);

    auto ranges_begin   = std::ranges::begin(testing);
    auto ranges_end     = std::ranges::end(testing);
    auto ranges_cbegin  = std::ranges::cbegin(testing);
    auto ranges_cend    = std::ranges::cend(testing);
    auto ranges_rbegin  = std::ranges::rbegin(testing);
    auto ranges_rend    = std::ranges::rend(testing);
    auto ranges_crbegin = std::ranges::crbegin(testing);
    auto ranges_crend   = std::ranges::crend(testing);

    static_assert(std::sentinel_for<decltype(ranges_end), decltype(ranges_begin)>);
    static_assert(std::sentinel_for<decltype(ranges_cend), decltype(ranges_cbegin)>);
    static_assert(std::sentinel_for<decltype(ranges_rend), decltype(ranges_rbegin)>);
    static_assert(std::sentinel_for<decltype(ranges_crend), decltype(ranges_crbegin)>);

    static_assert(!std::same_as<decltype(ranges_begin), decltype(ranges_cbegin)>);
    static_assert(!std::same_as<decltype(ranges_end), decltype(ranges_cend)>);
    static_assert(!std::same_as<decltype(ranges_rbegin), decltype(ranges_crbegin)>);
    static_assert(!std::same_as<decltype(ranges_rend), decltype(ranges_crend)>);

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

    static_assert(std::ranges::range<decltype(testing)>);
    static_assert(std::ranges::common_range<decltype(testing)>);
    static_assert(std::ranges::constant_range<decltype(testing)>);

    auto begin   = std::begin(testing);
    auto end     = std::end(testing);
    auto cbegin  = std::cbegin(testing);
    auto cend    = std::cend(testing);
    auto rbegin  = std::rbegin(testing);
    auto rend    = std::rend(testing);
    auto crbegin = std::crbegin(testing);
    auto crend   = std::crend(testing);

    static_assert(std::same_as<decltype(begin), decltype(end)>);
    static_assert(std::same_as<decltype(cbegin), decltype(cend)>);
    static_assert(std::same_as<decltype(rbegin), decltype(rend)>);
    static_assert(std::same_as<decltype(crbegin), decltype(crend)>);

    static_assert(std::same_as<decltype(begin), decltype(cbegin)>);
    static_assert(std::same_as<decltype(end), decltype(cend)>);
    static_assert(std::same_as<decltype(rbegin), decltype(crbegin)>);
    static_assert(std::same_as<decltype(rend), decltype(crend)>);

    auto ranges_begin   = std::ranges::begin(testing);
    auto ranges_end     = std::ranges::end(testing);
    auto ranges_cbegin  = std::ranges::cbegin(testing);
    auto ranges_cend    = std::ranges::cend(testing);
    auto ranges_rbegin  = std::ranges::rbegin(testing);
    auto ranges_rend    = std::ranges::rend(testing);
    auto ranges_crbegin = std::ranges::crbegin(testing);
    auto ranges_crend   = std::ranges::crend(testing);

    static_assert(std::sentinel_for<decltype(ranges_end), decltype(ranges_begin)>);
    static_assert(std::sentinel_for<decltype(ranges_cend), decltype(ranges_cbegin)>);
    static_assert(std::sentinel_for<decltype(ranges_rend), decltype(ranges_rbegin)>);
    static_assert(std::sentinel_for<decltype(ranges_crend), decltype(ranges_crbegin)>);

    static_assert(std::same_as<decltype(ranges_begin), decltype(ranges_cbegin)>);
    static_assert(std::same_as<decltype(ranges_end), decltype(ranges_cend)>);
    static_assert(std::same_as<decltype(ranges_rbegin), decltype(ranges_crbegin)>);
    static_assert(std::same_as<decltype(ranges_rend), decltype(ranges_crend)>);

    for (auto&& i : testing)
    {
        static_assert(std::same_as<test const*, decltype(&i)>);
    }
}
