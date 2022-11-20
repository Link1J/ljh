
//          Copyright Jared Irwin 2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/ranges/terminators.hpp"
#include "ljh/ranges/views.hpp"
#include "ljh/ranges/actions.hpp"

inline constexpr ljh::ranges::adaptor transform = []<std::ranges::viewable_range R, typename F>(R &&r, F &&f) {
	return std::ranges::transform_view(std::forward<R>(r), std::forward<F>(f));
};

TEST_CASE("adaptor", "[test_20][ranges][adaptor]")
{
	std::vector<int> count{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto items = count | transform([](int a){ return a + 10; });

	auto start = items.begin();
	REQUIRE(start != items.end());
	CHECK(*(start++) == 10);
	CHECK(*(start++) == 11);
	CHECK(*(start++) == 12);
	CHECK(*(start++) == 13);
	CHECK(*(start++) == 14);
	CHECK(*(start++) == 15);
	CHECK(*(start++) == 16);
	CHECK(*(start++) == 17);
	CHECK(*(start++) == 18);
	CHECK(*(start++) == 19);
	REQUIRE(start == items.end());
}

TEST_CASE("first", "[test_20][ranges][adaptor]")
{
	std::vector<int> count{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	SECTION("has value")
	{
		auto items = count | ljh::ranges::first;

		REQUIRE(items);
		REQUIRE(*items == 0);
	}

	SECTION("has value (chained)")
	{
		auto items = count | std::views::filter([](int i) { return i % 2 == 0; }) | ljh::ranges::first;
	
		REQUIRE(items);
		REQUIRE(*items == 0);
	}

	SECTION("no value")
	{
		auto items = count | std::views::filter([](int) { return false; }) | ljh::ranges::first;

		REQUIRE(!items);
	}

	SECTION("or")
	{
		auto items = count | std::views::filter([](int) { return false; }) | ljh::ranges::first_or(10);

		REQUIRE(items == 10);
	}
}

TEST_CASE("last", "[test_20][ranges][adaptor]")
{
	std::vector<int> count{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	SECTION("has value")
	{
		auto items = count | ljh::ranges::last;

		REQUIRE(items);
		REQUIRE(*items == 9);
	}

	SECTION("has value (chained)")
	{
		auto items = count | std::views::filter([](int i) { return i % 2 == 0; }) | ljh::ranges::last;
	
		REQUIRE(items);
		REQUIRE(*items == 8);
	}

	SECTION("no value")
	{
		auto items = count | std::views::filter([](int) { return false; }) | ljh::ranges::last;

		REQUIRE(!items);
	}

	SECTION("or")
	{
		auto items = count | std::views::filter([](int) { return false; }) | ljh::ranges::last_or(10);

		REQUIRE(items == 10);
	}
}