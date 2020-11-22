
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include "ljh/concepts.hpp"
#include "ljh/memory_mapped_file.hpp"

TEST_CASE("char_type", "[concepts]") {
	CHECK(ljh::char_type<char>);
	CHECK(ljh::char_type<wchar_t>);
	CHECK(ljh::char_type<std::string::value_type>);
	CHECK(ljh::char_type<std::u16string::value_type>);
	CHECK(!ljh::char_type<int>);
	CHECK(!ljh::char_type<long>);
	CHECK(!ljh::char_type<float>);
	CHECK(!ljh::char_type<double>);
}

enum test_enum { AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA };

TEST_CASE("scoped_enum", "[concepts]") {
	CHECK(ljh::scoped_enum<ljh::memory_mapped::permissions>);
	CHECK(!ljh::scoped_enum<int>);
	CHECK(!ljh::scoped_enum<test_enum>);
}