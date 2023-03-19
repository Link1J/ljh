
//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/function_pointer.hpp"
#include "ljh/os_build_info.hpp"

namespace Catch
{
	template<typename T>
	struct StringMaker<ljh::function_pointer<T>>
	{
		static std::string convert(ljh::function_pointer<T> const& value)
		{
			return StringMaker<typename ljh::function_pointer<T>::type>{}.convert(value.get());
		}
	};
}

static bool temp() { return true; }
static bool temp_noexcept() noexcept { return false; }

TEST_CASE("function_pointer", "[test_11][function_pointer]") {
	ljh::function_pointer<decltype(temp)> test {temp};
	REQUIRE(test);
	CHECK(test != (uintptr_t)0);
	CHECK(test == temp);
	CHECK(test != temp_noexcept);
	REQUIRE(test());
}

TEST_CASE("function_pointer noexcept", "[test_11][function_pointer]") {
	ljh::function_pointer<decltype(temp_noexcept)> test {temp_noexcept};
	REQUIRE(test);
	CHECK(test != (uintptr_t)0);
	CHECK(test != temp);
	CHECK(test == temp_noexcept);
	REQUIRE(!test());
}

TEST_CASE("function_pointer empty", "[test_11][function_pointer]") {
	ljh::function_pointer<void()> test;
	REQUIRE(test.empty());
	CHECK(test == (uintptr_t)0);
	REQUIRE(!test);
}