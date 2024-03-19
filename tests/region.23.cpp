
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0,.t,t or copy at
//          https://www.boost.org/LICENSE_1_0,.t,t)

#include "ljh/area/region.hpp"
#include "ljh/area/point.hpp"
#include "ljh/area/size.hpp"

#include <catch2/catch_test_macros.hpp>

namespace Catch
{
    template<typename T>
    concept formattable = requires(T const& value) {
        { std::format("{}", value) } -> std::same_as<std::string>;
        requires(!(is_range<T>::value && !::Catch::Detail::IsStreamInsertable<T>::value));
    };

    template<formattable T>
    struct StringMaker<T>
    {
        static std::string convert(T const& value)
        {
            return std::format("{}", value);
        }
    };
} // namespace Catch

TEST_CASE("region - constructor", "[test_23][region]")
{
    SECTION("default")
    {
        ljh::region region;
        REQUIRE(region == ljh::region{});
    }
    SECTION("initializer_list")
    {
        SECTION("one item")
        {
            ljh::region region{{0, 0, 2, 2}};
            REQUIRE(region == ljh::region{{0, 0, 2, 2}});
        }
        SECTION("two items")
        {
            ljh::region region{{0, 0, 2, 2}, {2, 2, 2, 2}};
            REQUIRE(region == ljh::region{{0, 0, 2, 2}, {2, 2, 2, 2}});
        }
        SECTION("merging items")
        {
            ljh::region region{{0, 0, 2, 2}, {0, 2, 2, 2}, {2, 0, 2, 2}, {2, 2, 2, 2}};
            REQUIRE(region == ljh::region{{0, 0, 4, 4}});
        }
    }
}

TEST_CASE("region - add (rect)", "[test_23][region]")
{
    SECTION("without merging")
    {
        ljh::region region;
        region += {0, 0, 2, 2};
        REQUIRE(region == ljh::region{{0, 0, 2, 2}});
        region += {2, 2, 2, 2};
        REQUIRE(region == ljh::region{{0, 0, 2, 2}, {2, 2, 2, 2}});
    }
    SECTION("with merging")
    {
        ljh::region region{{0, 0, 2, 2}};
        SECTION("order 1")
        {
            region.add({{0, 2, 2, 2}, {2, 0, 2, 2}, {2, 2, 2, 2}});
            REQUIRE(region == ljh::region{{0, 0, 4, 4}});
        }
        SECTION("order 2")
        {
            region.add({{2, 2, 2, 2}, {0, 2, 2, 2}, {2, 0, 2, 2}});
            REQUIRE(region == ljh::region{{0, 0, 4, 4}});
        }
        SECTION("order 3")
        {
            region.add({{2, 0, 2, 2}, {2, 2, 2, 2}, {0, 2, 2, 2}});
            REQUIRE(region == ljh::region{{0, 0, 4, 4}});
        }
        SECTION("order 4")
        {
            region.add({{2, 0, 2, 2}, {0, 2, 2, 2}, {2, 2, 2, 2}});
            REQUIRE(region == ljh::region{{0, 0, 4, 4}});
        }
    }
}

TEST_CASE("region - subtract (rect)", "[test_23][region]")
{
    SECTION("without merging")
    {
        ljh::region region{{0, 0, 2, 2}};

        region -= {0, 1, 1, 1};
        CHECK(region == ljh::region{{0, 0, 2, 1}, {1, 1, 1, 1}});

        region -= {1, 0, 1, 1};
        REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 1, 1}});
    }
    SECTION("with merging")
    {
        ljh::region region{{0, 0, 4, 4}};
        region.subtract({{0, 1, 1, 1}, {1, 0, 1, 1}});
        CHECK(region == ljh::region{{0, 0, 1, 1}, {2, 0, 2, 1}, {0, 2, 4, 2}, {1, 1, 3, 1}});

        SECTION("order 1")
        {
            region.subtract({{1, 0, 3, 1}, {0, 1, 1, 3}});
            REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 3, 3}});
        }

        SECTION("order 2")
        {
            region.subtract({{0, 1, 1, 3}, {1, 0, 3, 1}});
            REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 3, 3}});
        }
    }
}

TEST_CASE("region - subtract (region)", "[test_23][region]")
{
    SECTION("without merging")
    {
        ljh::region region{{0, 0, 2, 2}};
        region -= {{0, 1, 1, 1}, {1, 0, 1, 1}};
        REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 1, 1}});
    }
    SECTION("with merging")
    {
        ljh::region region{{0, 0, 4, 4}};
        region.subtract({{0, 1, 1, 1}, {1, 0, 1, 1}});
        CHECK(region == ljh::region{{0, 0, 1, 1}, {2, 0, 2, 1}, {0, 2, 4, 2}, {1, 1, 3, 1}});

        SECTION("order 1")
        {
            region.subtract({{1, 0, 3, 1}, {0, 1, 1, 3}});
            REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 3, 3}});
        }

        SECTION("order 2")
        {
            region.subtract({{0, 1, 1, 3}, {1, 0, 3, 1}});
            REQUIRE(region == ljh::region{{0, 0, 1, 1}, {1, 1, 3, 3}});
        }
    }
}

TEST_CASE("region (tagged) - constructor", "[test_23][region][tagged_region]")
{
    SECTION("default")
    {
        ljh::tagged_region<int> region;
        REQUIRE(region == ljh::tagged_region<int>{});
    }
    SECTION("initializer_list")
    {
        SECTION("one item")
        {
            ljh::tagged_region<int> region{{0, 0, 2, 2, 1}};
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 2, 2, 1}});
        }
        SECTION("two items")
        {
            ljh::tagged_region<int> region{{0, 0, 2, 2, 1}, {2, 2, 2, 2, 1}};
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 2, 2, 1}, {2, 2, 2, 2, 1}});
        }
        SECTION("merging items")
        {
            ljh::tagged_region<int> region{{0, 0, 2, 2, 1}, {0, 2, 2, 2, 1}, {2, 0, 2, 2, 1}, {2, 2, 2, 2, 1}};
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 4, 4, 1}});
        }
        SECTION("unmergable items")
        {
            ljh::tagged_region<int> region{{0, 0, 2, 2, 1}, {0, 2, 2, 2, 2}, {2, 0, 2, 2, 2}, {2, 2, 2, 2, 1}};
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 2, 2, 1}, {0, 2, 2, 2, 2}, {2, 0, 2, 2, 2}, {2, 2, 2, 2, 1}});
        }
    }
}

TEST_CASE("region (tagged) - add (rect)", "[test_23][region][tagged_region]")
{
    SECTION("without merging")
    {
        ljh::tagged_region<int> region;
        region += {0, 0, 2, 2, 1};
        REQUIRE(region == ljh::tagged_region<int>{{0, 0, 2, 2, 1}});
        region += {2, 2, 2, 2, 1};
        REQUIRE(region == ljh::tagged_region<int>{{0, 0, 2, 2, 1}, {2, 2, 2, 2, 1}});
    }
    SECTION("with merging")
    {
        ljh::tagged_region<int> region{{0, 0, 2, 2, 1}};
        SECTION("order 1")
        {
            region.add({{0, 2, 2, 2, 1}, {2, 0, 2, 2, 1}, {2, 2, 2, 2, 1}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 4, 4, 1}});
        }
        SECTION("order 2")
        {
            region.add({{2, 2, 2, 2, 1}, {0, 2, 2, 2, 1}, {2, 0, 2, 2, 1}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 4, 4, 1}});
        }
        SECTION("order 3")
        {
            region.add({{2, 0, 2, 2, 1}, {2, 2, 2, 2, 1}, {0, 2, 2, 2, 1}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 4, 4, 1}});
        }
        SECTION("order 4")
        {
            region.add({{2, 0, 2, 2, 1}, {0, 2, 2, 2, 1}, {2, 2, 2, 2, 1}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 4, 4, 1}});
        }
    }
}

TEST_CASE("region (tagged) - subtract (rect)", "[test_23][region][tagged_region]")
{
    SECTION("without merging")
    {
        ljh::tagged_region<int> region{{0, 0, 2, 2, 1}};

        region -= {0, 1, 1, 1};
        CHECK(region == ljh::tagged_region<int>{{0, 0, 2, 1, 1}, {1, 1, 1, 1, 1}});

        region -= {1, 0, 1, 1};
        REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 1, 1, 1}});
    }
    SECTION("with merging")
    {
        ljh::tagged_region<int> region{{0, 0, 4, 4, 1}};
        region.subtract({{0, 1, 1, 1}, {1, 0, 1, 1}});
        CHECK(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {2, 0, 2, 1, 1}, {0, 2, 4, 2, 1}, {1, 1, 3, 1, 1}});

        SECTION("order 1")
        {
            region.subtract({{1, 0, 3, 1}, {0, 1, 1, 3}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 3, 3, 1}});
        }

        SECTION("order 2")
        {
            region.subtract({{0, 1, 1, 3}, {1, 0, 3, 1}});
            REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 3, 3, 1}});
        }
    }
    SECTION("with unmergable")
    {
        SECTION("without mergeing")
        {
            ljh::tagged_region<int> region{{0, 0, 2, 2, 1}, {0, 2, 2, 2, 2}, {2, 0, 2, 2, 2}, {2, 2, 2, 2, 1}};
            region.subtract({{0, 1, 1, 1}, {1, 0, 1, 1}});
            CHECK(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 1, 1, 1}, {0, 2, 2, 2, 2}, {2, 0, 2, 2, 2}, {2, 2, 2, 2, 1}});

            SECTION("order 1")
            {
                region.subtract({{1, 0, 3, 1}, {0, 1, 1, 3}});
                REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 2, 1, 2, 2}, {2, 1, 2, 1, 2}, {2, 2, 2, 2, 1}});
            }

            SECTION("order 2")
            {
                region.subtract({{0, 1, 1, 3}, {1, 0, 3, 1}});
                REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 2, 1, 2, 2}, {2, 1, 2, 1, 2}, {2, 2, 2, 2, 1}});
            }
        }
        SECTION("with merging")
        {
            ljh::tagged_region<int> region{{0, 0, 1, 1, 1}, {0, 1, 2, 3, 2}, {1, 0, 3, 1, 2}, {2, 1, 2, 1, 2}, {2, 2, 2, 2, 1}};
            region.subtract({{0, 1, 1, 1}, {1, 0, 1, 1}});
            CHECK(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {0, 2, 2, 2, 2}, {1, 1, 3, 1, 2}, {2, 0, 2, 1, 2}, {2, 2, 2, 2, 1}});

            SECTION("order 1")
            {
                region.subtract({{1, 0, 3, 1}, {0, 1, 1, 3}});
                REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 2, 1, 2, 2}, {1, 1, 3, 1, 2}, {2, 2, 2, 2, 1}});
            }

            SECTION("order 2")
            {
                region.subtract({{0, 1, 1, 3}, {1, 0, 3, 1}});
                REQUIRE(region == ljh::tagged_region<int>{{0, 0, 1, 1, 1}, {1, 2, 1, 2, 2}, {1, 1, 3, 1, 2}, {2, 2, 2, 2, 1}});
            }
        }
    }
}