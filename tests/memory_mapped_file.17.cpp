
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_translate_exception.hpp>
#include "ljh/memory_mapped_file.hpp"
#include <cstring>

using namespace std::literals;

const char LICENSE[] = {
	0x42,0x6F,0x6F,0x73,0x74,0x20,0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x20,0x4C,
	0x69,0x63,0x65,0x6E,0x73,0x65,0x20,0x2D,0x20,0x56,0x65,0x72,0x73,0x69,0x6F,0x6E,
	0x20,0x31,0x2E,0x30,0x20,0x2D,0x20,0x41,0x75,0x67,0x75,0x73,0x74,0x20,0x31,0x37,
	0x74,0x68,0x2C,0x20,0x32,0x30,0x30,0x33,0x0A,0x0A,0x50,0x65,0x72,0x6D,0x69,0x73,
	0x73,0x69,0x6F,0x6E,0x20,0x69,0x73,0x20,0x68,0x65,0x72,0x65,0x62,0x79,0x20,0x67,
	0x72,0x61,0x6E,0x74,0x65,0x64,0x2C,0x20,0x66,0x72,0x65,0x65,0x20,0x6F,0x66,0x20,
	0x63,0x68,0x61,0x72,0x67,0x65,0x2C,0x20,0x74,0x6F,0x20,0x61,0x6E,0x79,0x20,0x70,
	0x65,0x72,0x73,0x6F,0x6E,0x20,0x6F,0x72,0x20,0x6F,0x72,0x67,0x61,0x6E,0x69,0x7A,
	0x61,0x74,0x69,0x6F,0x6E,0x0A,0x6F,0x62,0x74,0x61,0x69,0x6E,0x69,0x6E,0x67,0x20,
	0x61,0x20,0x63,0x6F,0x70,0x79,0x20,0x6F,0x66,0x20,0x74,0x68,0x65,0x20,0x73,0x6F,
	0x66,0x74,0x77,0x61,0x72,0x65,0x20,0x61,0x6E,0x64,0x20,0x61,0x63,0x63,0x6F,0x6D,
	0x70,0x61,0x6E,0x79,0x69,0x6E,0x67,0x20,0x64,0x6F,0x63,0x75,0x6D,0x65,0x6E,0x74,
	0x61,0x74,0x69,0x6F,0x6E,0x20,0x63,0x6F,0x76,0x65,0x72,0x65,0x64,0x20,0x62,0x79,
	0x0A,0x74,0x68,0x69,0x73,0x20,0x6C,0x69,0x63,0x65,0x6E,0x73,0x65,0x20,0x28,0x74,
	0x68,0x65,0x20,0x22,0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x22,0x29,0x20,0x74,
	0x6F,0x20,0x75,0x73,0x65,0x2C,0x20,0x72,0x65,0x70,0x72,0x6F,0x64,0x75,0x63,0x65,
	0x2C,0x20,0x64,0x69,0x73,0x70,0x6C,0x61,0x79,0x2C,0x20,0x64,0x69,0x73,0x74,0x72,
	0x69,0x62,0x75,0x74,0x65,0x2C,0x0A,0x65,0x78,0x65,0x63,0x75,0x74,0x65,0x2C,0x20,
	0x61,0x6E,0x64,0x20,0x74,0x72,0x61,0x6E,0x73,0x6D,0x69,0x74,0x20,0x74,0x68,0x65,
	0x20,0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x2C,0x20,0x61,0x6E,0x64,0x20,0x74,
	0x6F,0x20,0x70,0x72,0x65,0x70,0x61,0x72,0x65,0x20,0x64,0x65,0x72,0x69,0x76,0x61,
	0x74,0x69,0x76,0x65,0x20,0x77,0x6F,0x72,0x6B,0x73,0x20,0x6F,0x66,0x20,0x74,0x68,
	0x65,0x0A,0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x2C,0x20,0x61,0x6E,0x64,0x20,
	0x74,0x6F,0x20,0x70,0x65,0x72,0x6D,0x69,0x74,0x20,0x74,0x68,0x69,0x72,0x64,0x2D,
	0x70,0x61,0x72,0x74,0x69,0x65,0x73,0x20,0x74,0x6F,0x20,0x77,0x68,0x6F,0x6D,0x20,
	0x74,0x68,0x65,0x20,0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x20,0x69,0x73,0x20,
	0x66,0x75,0x72,0x6E,0x69,0x73,0x68,0x65,0x64,0x20,0x74,0x6F,0x0A,0x64,0x6F,0x20,
	0x73,0x6F,0x2C,0x20,0x61,0x6C,0x6C,0x20,0x73,0x75,0x62,0x6A,0x65,0x63,0x74,0x20,
	0x74,0x6F,0x20,0x74,0x68,0x65,0x20,0x66,0x6F,0x6C,0x6C,0x6F,0x77,0x69,0x6E,0x67,
	0x3A,0x0A,0x0A,0x54,0x68,0x65,0x20,0x63,0x6F,0x70,0x79,0x72,0x69,0x67,0x68,0x74,
	0x20,0x6E,0x6F,0x74,0x69,0x63,0x65,0x73,0x20,0x69,0x6E,0x20,0x74,0x68,0x65,0x20,
	0x53,0x6F,0x66,0x74,0x77,0x61,0x72,0x65,0x20,0x61,0x6E,0x64,0x20,0x74,0x68,0x69,
	0x73,0x20,0x65,0x6E,0x74,0x69,0x72,0x65,0x20,0x73,0x74,0x61,0x74,0x65,0x6D,0x65,
	0x6E,0x74,0x2C,0x20,0x69,0x6E,0x63,0x6C,0x75,0x64,0x69,0x6E,0x67,0x0A,0x74,0x68,
	0x65,0x20,0x61,0x62,0x6F,0x76,0x65,0x20,0x6C,0x69,0x63,0x65,0x6E,0x73,0x65,0x20,
	0x67,0x72,0x61,0x6E,0x74,0x2C,0x20,0x74,0x68,0x69,0x73,0x20,0x72,0x65,0x73,0x74,
	0x72,0x69,0x63,0x74,0x69,0x6F,0x6E,0x20,0x61,0x6E,0x64,0x20,0x74,0x68,0x65,0x20,
	0x66,0x6F,0x6C,0x6C,0x6F,0x77,0x69,0x6E,0x67,0x20,0x64,0x69,0x73,0x63,0x6C,0x61,
	0x69,0x6D,0x65,0x72,0x2C,0x0A,0x6D,0x75,0x73,0x74,0x20,0x62,0x65,0x20,0x69,0x6E,
	0x63,0x6C,0x75,0x64,0x65,0x64,0x20,0x69,0x6E,0x20,0x61,0x6C,0x6C,0x20,0x63,0x6F,
	0x70,0x69,0x65,0x73,0x20,0x6F,0x66,0x20,0x74,0x68,0x65,0x20,0x53,0x6F,0x66,0x74,
	0x77,0x61,0x72,0x65,0x2C,0x20,0x69,0x6E,0x20,0x77,0x68,0x6F,0x6C,0x65,0x20,0x6F,
	0x72,0x20,0x69,0x6E,0x20,0x70,0x61,0x72,0x74,0x2C,0x20,0x61,0x6E,0x64,0x0A,0x61,
	0x6C,0x6C,0x20,0x64,0x65,0x72,0x69,0x76,0x61,0x74,0x69,0x76,0x65,0x20,0x77,0x6F,
	0x72,0x6B,0x73,0x20,0x6F,0x66,0x20,0x74,0x68,0x65,0x20,0x53,0x6F,0x66,0x74,0x77,
	0x61,0x72,0x65,0x2C,0x20,0x75,0x6E,0x6C,0x65,0x73,0x73,0x20,0x73,0x75,0x63,0x68,
	0x20,0x63,0x6F,0x70,0x69,0x65,0x73,0x20,0x6F,0x72,0x20,0x64,0x65,0x72,0x69,0x76,
	0x61,0x74,0x69,0x76,0x65,0x0A,0x77,0x6F,0x72,0x6B,0x73,0x20,0x61,0x72,0x65,0x20,
	0x73,0x6F,0x6C,0x65,0x6C,0x79,0x20,0x69,0x6E,0x20,0x74,0x68,0x65,0x20,0x66,0x6F,
	0x72,0x6D,0x20,0x6F,0x66,0x20,0x6D,0x61,0x63,0x68,0x69,0x6E,0x65,0x2D,0x65,0x78,
	0x65,0x63,0x75,0x74,0x61,0x62,0x6C,0x65,0x20,0x6F,0x62,0x6A,0x65,0x63,0x74,0x20,
	0x63,0x6F,0x64,0x65,0x20,0x67,0x65,0x6E,0x65,0x72,0x61,0x74,0x65,0x64,0x20,0x62,
	0x79,0x0A,0x61,0x20,0x73,0x6F,0x75,0x72,0x63,0x65,0x20,0x6C,0x61,0x6E,0x67,0x75,
	0x61,0x67,0x65,0x20,0x70,0x72,0x6F,0x63,0x65,0x73,0x73,0x6F,0x72,0x2E,0x0A,0x0A,
	0x54,0x48,0x45,0x20,0x53,0x4F,0x46,0x54,0x57,0x41,0x52,0x45,0x20,0x49,0x53,0x20,
	0x50,0x52,0x4F,0x56,0x49,0x44,0x45,0x44,0x20,0x22,0x41,0x53,0x20,0x49,0x53,0x22,
	0x2C,0x20,0x57,0x49,0x54,0x48,0x4F,0x55,0x54,0x20,0x57,0x41,0x52,0x52,0x41,0x4E,
	0x54,0x59,0x20,0x4F,0x46,0x20,0x41,0x4E,0x59,0x20,0x4B,0x49,0x4E,0x44,0x2C,0x20,
	0x45,0x58,0x50,0x52,0x45,0x53,0x53,0x20,0x4F,0x52,0x0A,0x49,0x4D,0x50,0x4C,0x49,
	0x45,0x44,0x2C,0x20,0x49,0x4E,0x43,0x4C,0x55,0x44,0x49,0x4E,0x47,0x20,0x42,0x55,
	0x54,0x20,0x4E,0x4F,0x54,0x20,0x4C,0x49,0x4D,0x49,0x54,0x45,0x44,0x20,0x54,0x4F,
	0x20,0x54,0x48,0x45,0x20,0x57,0x41,0x52,0x52,0x41,0x4E,0x54,0x49,0x45,0x53,0x20,
	0x4F,0x46,0x20,0x4D,0x45,0x52,0x43,0x48,0x41,0x4E,0x54,0x41,0x42,0x49,0x4C,0x49,
	0x54,0x59,0x2C,0x0A,0x46,0x49,0x54,0x4E,0x45,0x53,0x53,0x20,0x46,0x4F,0x52,0x20,
	0x41,0x20,0x50,0x41,0x52,0x54,0x49,0x43,0x55,0x4C,0x41,0x52,0x20,0x50,0x55,0x52,
	0x50,0x4F,0x53,0x45,0x2C,0x20,0x54,0x49,0x54,0x4C,0x45,0x20,0x41,0x4E,0x44,0x20,
	0x4E,0x4F,0x4E,0x2D,0x49,0x4E,0x46,0x52,0x49,0x4E,0x47,0x45,0x4D,0x45,0x4E,0x54,
	0x2E,0x20,0x49,0x4E,0x20,0x4E,0x4F,0x20,0x45,0x56,0x45,0x4E,0x54,0x0A,0x53,0x48,
	0x41,0x4C,0x4C,0x20,0x54,0x48,0x45,0x20,0x43,0x4F,0x50,0x59,0x52,0x49,0x47,0x48,
	0x54,0x20,0x48,0x4F,0x4C,0x44,0x45,0x52,0x53,0x20,0x4F,0x52,0x20,0x41,0x4E,0x59,
	0x4F,0x4E,0x45,0x20,0x44,0x49,0x53,0x54,0x52,0x49,0x42,0x55,0x54,0x49,0x4E,0x47,
	0x20,0x54,0x48,0x45,0x20,0x53,0x4F,0x46,0x54,0x57,0x41,0x52,0x45,0x20,0x42,0x45,
	0x20,0x4C,0x49,0x41,0x42,0x4C,0x45,0x0A,0x46,0x4F,0x52,0x20,0x41,0x4E,0x59,0x20,
	0x44,0x41,0x4D,0x41,0x47,0x45,0x53,0x20,0x4F,0x52,0x20,0x4F,0x54,0x48,0x45,0x52,
	0x20,0x4C,0x49,0x41,0x42,0x49,0x4C,0x49,0x54,0x59,0x2C,0x20,0x57,0x48,0x45,0x54,
	0x48,0x45,0x52,0x20,0x49,0x4E,0x20,0x43,0x4F,0x4E,0x54,0x52,0x41,0x43,0x54,0x2C,
	0x20,0x54,0x4F,0x52,0x54,0x20,0x4F,0x52,0x20,0x4F,0x54,0x48,0x45,0x52,0x57,0x49,
	0x53,0x45,0x2C,0x0A,0x41,0x52,0x49,0x53,0x49,0x4E,0x47,0x20,0x46,0x52,0x4F,0x4D,
	0x2C,0x20,0x4F,0x55,0x54,0x20,0x4F,0x46,0x20,0x4F,0x52,0x20,0x49,0x4E,0x20,0x43,
	0x4F,0x4E,0x4E,0x45,0x43,0x54,0x49,0x4F,0x4E,0x20,0x57,0x49,0x54,0x48,0x20,0x54,
	0x48,0x45,0x20,0x53,0x4F,0x46,0x54,0x57,0x41,0x52,0x45,0x20,0x4F,0x52,0x20,0x54,
	0x48,0x45,0x20,0x55,0x53,0x45,0x20,0x4F,0x52,0x20,0x4F,0x54,0x48,0x45,0x52,0x0A,
	0x44,0x45,0x41,0x4C,0x49,0x4E,0x47,0x53,0x20,0x49,0x4E,0x20,0x54,0x48,0x45,0x20,
	0x53,0x4F,0x46,0x54,0x57,0x41,0x52,0x45,0x2E
};

CATCH_TRANSLATE_EXCEPTION(const ljh::memory_mapped::io_error& ex)
{
	return std::string{ex.what()} + " - " + ex.error_string() + " (" + std::to_string(ex.error_code()) + ")";
}

CATCH_TRANSLATE_EXCEPTION(const ljh::memory_mapped::invalid_position& ex)
{
	return std::string{ex.what()} + " - " + ex.error_string() + " (" + std::to_string(ex.error_code()) + ")";
}

CATCH_TRANSLATE_EXCEPTION(const ljh::memory_mapped::invalid_file& ex)
{
	return std::string{ex.what()} + " - " + ex.error_string() + " (" + std::to_string(ex.error_code()) + ")";
}

CATCH_TRANSLATE_EXCEPTION(const ljh::memory_mapped::invalid_permissions& ex)
{
	return std::string{ex.what()} + " - " + ex.error_string() + " (" + std::to_string(ex.error_code()) + ")";
}

TEST_CASE("file","[test_17][memory_mapped_file][!mayfail]")
{
	REQUIRE_NOTHROW(ljh::memory_mapped::file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::r});
	ljh::memory_mapped::file file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::r};
	REQUIRE(file.is_open());
	REQUIRE(file.size() == sizeof(LICENSE));
	SECTION("view constructors")
	{
		REQUIRE_NOTHROW(ljh::memory_mapped::view{file, ljh::memory_mapped::permissions::r, 0, file.size()});
	}
	SECTION("view")
	{
		ljh::memory_mapped::view view{file, ljh::memory_mapped::permissions::r, 0, file.size()};
		REQUIRE(view.valid());
		REQUIRE(memcmp(view.as<char>(), LICENSE, sizeof(LICENSE)) == 0);
	}
}

TEST_CASE("exec","[test_17][memory_mapped_file][!mayfail]")
{
	REQUIRE_NOTHROW(ljh::memory_mapped::file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::rwx});
	ljh::memory_mapped::file file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::rwx};
	REQUIRE(file.is_open());
	REQUIRE(file.size() == sizeof(LICENSE));
	SECTION("view")
	{
		REQUIRE_NOTHROW(ljh::memory_mapped::view{file, ljh::memory_mapped::permissions::rx, 0, file.size()});
		ljh::memory_mapped::view view{file, ljh::memory_mapped::permissions::rx, 0, file.size()};
		REQUIRE(view.valid());
		REQUIRE(memcmp(view.as<char>(), LICENSE, sizeof(LICENSE)) == 0);
	}
	SECTION("view write ")
	{
		REQUIRE_NOTHROW(ljh::memory_mapped::view{file, ljh::memory_mapped::permissions::rwx, 0, file.size()});
		ljh::memory_mapped::view view{file, ljh::memory_mapped::permissions::rwx, 0, file.size()};
		REQUIRE(view.valid());
		REQUIRE(memcmp(view.as<char>(), LICENSE, sizeof(LICENSE)) == 0);
	}
}

TEST_CASE("copy on write","[test_17][memory_mapped_file][!mayfail]")
{
	ljh::memory_mapped::file file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::rw};
	ljh::memory_mapped::view ref{file, ljh::memory_mapped::permissions::r, 0, file.size()};
	REQUIRE(ref.valid());
	ljh::memory_mapped::view write{file, ljh::memory_mapped::permissions::r | ljh::memory_mapped::permissions::copy_on_write, 0, file.size()};
	REQUIRE(write.valid());
	write.as<char>()[0] = 0xA;
	REQUIRE(memcmp(ref.as<char>(), write.as<char>(), file.size()) != 0);
}

TEST_CASE("error_string","[test_17][memory_mapped_file][!mayfail]")
{
	try
	{
		ljh::memory_mapped::file no_file{"C://%APPDATA%", ljh::memory_mapped::permissions::rwx};
	}
	catch (const ljh::memory_mapped::io_error& e)
	{
		REQUIRE_FALSE(std::string{e.error_string()}.empty());
	}
}

TEST_CASE("misaligned view","[test_17][memory_mapped_file][!mayfail]")
{
	ljh::memory_mapped::file file{"../../LICENSE_1_0.txt", ljh::memory_mapped::permissions::rw};
	REQUIRE_NOTHROW(ljh::memory_mapped::view{file, ljh::memory_mapped::permissions::r, 40, 1});
	ljh::memory_mapped::view ref{file, ljh::memory_mapped::permissions::r, 40, 1};
	REQUIRE(ref.valid());
	REQUIRE(*ref.as<char>() == LICENSE[40]);
}
