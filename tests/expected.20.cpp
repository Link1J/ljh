
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/expected.hpp"
#include <type_traits>

struct trivial_data_type {
	int a, b, c;
};
static_assert(std::is_trivial<trivial_data_type>::value);

TEST_CASE("expected<trivial_data_type, int>::emplace", "[expected][emplace]" ) {
	ljh::expected<trivial_data_type, int> test;
	REQUIRE(test.has_value());

	SECTION("emplace trivial_data_type") {
		test.emplace(1,2,3);
		REQUIRE(test.has_value());

		SECTION("Overwrite with error") {
			test = ljh::unexpected{100};
			REQUIRE(!test.has_value());
		}
	}
}

TEST_CASE("expected<void, int>", "[expected]" ) {
	ljh::expected<void, int> test;
	REQUIRE(test.has_value());
}

TEST_CASE("expected<int, int>", "[expected]" ) {
	ljh::expected<int, int> test;
	REQUIRE(test.has_value());
}