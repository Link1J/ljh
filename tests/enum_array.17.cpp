
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/enum_array.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

enum class idx
{
    a,
    b,
    c,
    d,
    END,
};

TEST_CASE("enum_array constexpr size (END, static constexpr)", "[test_17][enum_array][constexpr_size]")
{
    static constexpr ljh::enum_array<int, idx> test;
    static_assert(std::size(test) == 4);
}

TEST_CASE("enum_array constexpr size (END, constexpr)", "[test_17][enum_array][constexpr_size]")
{
    constexpr ljh::enum_array<int, idx> test;
    static_assert(std::size(test) == 4);
}

TEST_CASE("enum_array constexpr size (END)", "[test_17][enum_array][constexpr_size]")
{
    ljh::enum_array<int, idx> test;
    static_assert(std::size(test) == 4);
}

TEST_CASE("enum_array constexpr size (Custom Size, static constexpr)", "[test_17][enum_array][constexpr_size]")
{
    static constexpr ljh::enum_array<int, idx, 2> test;
    static_assert(std::size(test) == 2);
}

TEST_CASE("enum_array constexpr size (Custom Size, constexpr)", "[test_17][enum_array][constexpr_size]")
{
    constexpr ljh::enum_array<int, idx, 2> test;
    static_assert(std::size(test) == 2);
}

TEST_CASE("enum_array constexpr size (Custom Size)", "[test_17][enum_array][constexpr_size]")
{
    ljh::enum_array<int, idx, 2> test;
    static_assert(std::size(test) == 2);
}

TEST_CASE("enum_array constexpr size (Custom Size, non constexpr)", "[test_17][enum_array][constexpr_size]")
{
    ljh::enum_array<std::shared_ptr<int>, idx, 2> test;
    static_assert(std::size(test) == 2);
}

TEST_CASE("enum_array ctor", "[test_17][enum_array][ctor]")
{
    static constexpr ljh::enum_array<int, idx> test{4, 3, 2, 1};
}
