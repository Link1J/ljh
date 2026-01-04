
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

#include "ljh/cpp_version.hpp"
#include "ljh/os_build_info.hpp"
#if !(defined(LJH_COMPILER_CLANG) && defined(LJH_TARGET_Windows))

#define LJH_FUNCTION_POINTERS_LOAD_RETURNS_EXPECTED
#include "ljh/function_pointer.hpp"

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

TEST_CASE("function_pointer", "[test_17][function_pointer]") {
	ljh::function_pointer<decltype(temp)> test {temp};
	REQUIRE(test);
	CHECK(test != (uintptr_t)0);
	CHECK(test == temp);
	CHECK(test != temp_noexcept);
	REQUIRE(test());
}

TEST_CASE("function_pointer noexcept", "[test_17][function_pointer]") {
	ljh::function_pointer<decltype(temp_noexcept)> test {temp_noexcept};
	REQUIRE(test);
	CHECK(test != (uintptr_t)0);
	CHECK(test != temp);
	CHECK(test == temp_noexcept);
	REQUIRE(!test());
}

TEST_CASE("function_pointer empty", "[test_17][function_pointer]") {
	ljh::function_pointer<void()> test;
	REQUIRE(test.empty());
	CHECK(test == (uintptr_t)0);
	REQUIRE(!test);
}

#endif