
//          Copyright Jared Irwin 2023-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

#include "ljh/generator.hpp"

#include <string_view>

ljh::generator<std::uint64_t> fibonacci_sequence(unsigned n)
{
	if (n == 0)
		co_return;

	if (n > 94)
		throw std::runtime_error("Too big Fibonacci sequence. Elements would overflow.");

	co_yield 0;

	if (n == 1)
		co_return;

	co_yield 1;

	if (n == 2)
		co_return;

	std::uint64_t a = 0;
	std::uint64_t b = 1;

	for (unsigned i = 2; i < n; i++)
	{
		std::uint64_t s = a + b;
		co_yield s;
		a = b;
		b = s;
	}
}

TEST_CASE("generator", "[test_20][generator]")
{
	auto testing = fibonacci_sequence(10);
	auto it = std::ranges::begin(testing);
	auto end = std::ranges::end(testing);

	      REQUIRE(it != end); CHECK(*it == 0);	
	it++; REQUIRE(it != end); CHECK(*it == 1);
	it++; REQUIRE(it != end); CHECK(*it == 1);
	it++; REQUIRE(it != end); CHECK(*it == 2);
	it++; REQUIRE(it != end); CHECK(*it == 3);
	it++; REQUIRE(it != end); CHECK(*it == 5);
	it++; REQUIRE(it != end); CHECK(*it == 8);
	it++; REQUIRE(it != end); CHECK(*it == 13);
	it++; REQUIRE(it != end); CHECK(*it == 21);
	it++; REQUIRE(it != end); CHECK(*it == 34);
	it++; REQUIRE(it == end);
}

ljh::generator<std::string const&> delete_rows(std::string table, std::vector<int> ids)
{
	for (int id : ids)
	{
		co_yield std::format("DELETE FROM {0} WHERE id = {1};", table, id);
	}
}

ljh::generator<std::string const&> all_queries()
{
	co_yield ljh::ranges::elements_of(delete_rows("user", {4, 7, 9, 10}));
	co_yield ljh::ranges::elements_of(delete_rows("order", {11, 19}));
}

TEST_CASE("nested generator", "[test_20][generator]")
{
	auto testing = all_queries();
	auto it = std::ranges::begin(testing);
	auto end = std::ranges::end(testing);
	
	      REQUIRE(it != end); CHECK(*it == "DELETE FROM user WHERE id = 4;");
	it++; REQUIRE(it != end); CHECK(*it == "DELETE FROM user WHERE id = 7;");
	it++; REQUIRE(it != end); CHECK(*it == "DELETE FROM user WHERE id = 9;");
	it++; REQUIRE(it != end); CHECK(*it == "DELETE FROM user WHERE id = 10;");
	it++; REQUIRE(it != end); CHECK(*it == "DELETE FROM order WHERE id = 11;");
	it++; REQUIRE(it != end); CHECK(*it == "DELETE FROM order WHERE id = 19;");
	it++; REQUIRE(it == end);
}

TEST_CASE("exception handling - first throws", "[test_20][generator]")
{
	REQUIRE_NOTHROW(fibonacci_sequence(95));
	auto testing = fibonacci_sequence(95);
	REQUIRE_THROWS(std::ranges::begin(testing));
}

struct error : std::exception
{
	const char* what() const noexcept
	{
		return "test";
	}
};

ljh::generator<int> fails()
{
	co_yield 1;
	throw error{};
}

TEST_CASE("exception handling - second throws", "[test_20][generator]")
{
	SECTION("when") {
		auto testing = fails();
		auto it = std::ranges::begin(testing);
		auto end = std::ranges::end(testing);

		REQUIRE(it != end);
		CHECK(*it == 1);
		REQUIRE_THROWS(it++);
	}
	SECTION("for each loop") {
		try {
			for (auto part : fails()) {}
		} catch (std::exception const& e) {
			using namespace std::literals;
			REQUIRE(e.what() == "test"s);
		}
	}
}

ljh::generator<int> no_fail(auto&& start)
{
	co_yield start + 1;
	co_yield start + 2;
	co_yield start + 3;
}

ljh::generator<int> fails_main(auto&& start)
{
	auto nested = no_fail(start);
	for (auto it = std::begin(nested); it != std::end(nested); it++)
	{
		auto token = *it;
		if (token != start + 2)
			co_yield token;
		else 
			throw error{};
	}
}

TEST_CASE("exception handling - nested - main throws", "[test_20][generator]")
{
	SECTION("when") {
		auto testing = fails_main(0);
		auto it = std::ranges::begin(testing);
		auto end = std::ranges::end(testing);

		REQUIRE(it != end);
		CHECK(*it == 1);
		REQUIRE_THROWS(it++);
	}
	SECTION("for each loop") {
		try {
			for (auto part : fails_main(0)) {}
		} catch (std::exception const& e) {
			using namespace std::literals;
			REQUIRE(e.what() == "test"s);
		}
	}
}

struct unknown : std::invalid_argument
{
	explicit unknown(std::string const& item)
		: std::invalid_argument("Unknown " + item)
	{}
};

ljh::generator<int> fails_main2(auto&& start)
{
	auto nested = no_fail(start);
	for (auto it = std::begin(nested); it != std::end(nested); it++)
	{
		auto token = *it;
		if (token != start + 2)
			co_yield token;
		else
			throw unknown{std::format("{}", token)};
	}
}

TEST_CASE("exception handling - nested - main throws - custom error", "[test_20][generator]")
{
	SECTION("when") {
		auto testing = fails_main2(0);
		auto it = std::ranges::begin(testing);
		auto end = std::ranges::end(testing);

		REQUIRE(it != end);
		CHECK(*it == 1);
		REQUIRE_THROWS(it++);
	}
	SECTION("for each loop") {
		try {
			for (auto part : fails_main2(0)) {}
		} catch (std::exception const& e) {
			using namespace std::literals;
			REQUIRE(e.what() == "Unknown 2"s);
		}
	}
}
