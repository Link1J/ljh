//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// compile_time_string.hpp - v1.1
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
// Requires MVSC 19.26 or GCC 9
//
// ABOUT
//     Adds compile time strings
//
// USAGE
//    Meant for use in templates
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#if __cpp_nontype_template_args < 201911L
#error "C++20 support for non-type template args is needed"
#endif

#include <string_view>

namespace ljh
{
	template <typename Char, std::size_t Size, typename Traits = std::char_traits<Char>>
	struct basic_compile_time_string
	{
		using char_type = Char;

		char_type content[Size] = {};

		constexpr basic_compile_time_string(const char_type (&input)[Size + 1]) noexcept
		{
			if constexpr (Size != 0)
			{
				for (std::size_t i{0}; i < Size; ++i)
				{
					content[i] = input[i];
				}
			}
		}

		constexpr basic_compile_time_string(const basic_compile_time_string &other) noexcept
		{
			for (std::size_t i{0}; i < Size; ++i)
			{
				content[i] = other.content[i];
			}
		}

        [[nodiscard]] constexpr std::size_t size() const noexcept
		{
			return Size;
		}

		constexpr const char_type* begin() const noexcept
		{
			return content;
		}

		constexpr const char_type* end() const noexcept
		{
			return content + Size;
		}

		constexpr char_type operator[](std::size_t i) const noexcept
		{
			return i < Size ? content[i] : throw std::out_of_range("");
		}

		constexpr operator std::basic_string_view<char_type>() const noexcept
		{
			return std::basic_string_view<char_type>{content, Size};
		}

		static constexpr std::size_t FNV_offset_basis_table[] = {
			0, 0, 0, 0, 0x811C9DC5, 0, 0, 0, 0, 0xCBF29CE484222325
		};

		static constexpr std::size_t FNV_prime_table[] = {
			0, 0, 0, 0, 0x01000193, 0, 0, 0, 0, 0x00000100000001B3
		};

		constexpr std::enable_if_t<sizeof(char_type) == 1, std::size_t> hash() const noexcept
		{
			const std::size_t FNV_offset_basis = FNV_offset_basis_table[sizeof(std::size_t)];
			const std::size_t FNV_prime        = FNV_prime_table       [sizeof(std::size_t)];

			std::size_t hash_data = FNV_offset_basis;
			for (auto &byte_of_data : *this)
			{
				hash_data = hash_data ^ byte_of_data;
				hash_data = hash_data * FNV_prime;
			}
			return hash_data;
		}
	};

	template<typename Char, std::size_t Size, typename Traits = std::char_traits<Char>>
	basic_compile_time_string(const Char (&)[Size]) -> basic_compile_time_string<Char, Size - 1, Traits>;
	template<typename Char, std::size_t Size, typename Traits = std::char_traits<Char>>
	basic_compile_time_string(basic_compile_time_string<Char, Size>) -> basic_compile_time_string<Char, Size, Traits>;

	template<std::size_t N> using compile_time_string    = basic_compile_time_string<char    , N>;
	template<std::size_t N> using wcompile_time_string   = basic_compile_time_string<wchar_t , N>;
	template<std::size_t N> using u8compile_time_string  = basic_compile_time_string<char8_t , N>;
	template<std::size_t N> using u16compile_time_string = basic_compile_time_string<char16_t, N>;
	template<std::size_t N> using u32compile_time_string = basic_compile_time_string<char32_t, N>;

	
}

namespace std
{
	template<typename Char, std::size_t Size, typename Traits>
	struct hash<ljh::basic_compile_time_string<Char, Size, Traits>>
	{
		std::size_t operator()(ljh::basic_compile_time_string<Char, Size, Traits> const& s) const noexcept
		{
			return s.hash();
		}
	};
}
