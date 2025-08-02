
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#include "ljh/string_utils.hpp"
#include "ljh/compile_time_string.hpp"
#include "ljh/char_convertions.hpp"
#include "ljh/case_insensitive_string.hpp"
#include "ljh/byte_constant.hpp"
export module ljh.string;
export namespace ljh {
    inline namespace literals
	{
        using ljh::literals::operator""_bytes;
	}
    using ljh::case_insensitive_char_traits;
    using ljh::basic_case_insensitive_string;
	using ljh::case_insensitive_string;
	using ljh::wcase_insensitive_string;
#if __cpp_char8_t >= 201811L
	using ljh::u8case_insensitive_string;
#endif
#if __cpp_unicode_characters >= 200704L
	using ljh::u16case_insensitive_string;
	using ljh::u32case_insensitive_string;
#endif
#if __cpp_lib_string_view >= 201606L
    using ljh::basic_case_insensitive_string_view;
	using ljh::case_insensitive_string_view;
	using ljh::wcase_insensitive_string_view;
#if __cpp_char8_t >= 201811L
	using ljh::u8case_insensitive_string_view;
#endif
#if __cpp_unicode_characters >= 200704L
	using ljh::u16case_insensitive_string_view;
	using ljh::u32case_insensitive_string_view;
#endif
#endif
    using ljh::from_string;
    using ljh::compile_time_string;
    using ljh::compile_time_string_literal;
#if __cpp_nontype_template_args >= 201911L
	inline namespace compile_time_string_literals
	{
		using ljh::compile_time_string_literals::operator ""_cts;
		using ljh::compile_time_string_literals::operator ""_hash;
	}
#endif
    using ljh::operator+;
    using ljh::operator==;
    using ljh::ltrim;
    using ljh::rtrim;
    using ljh::trim;
    using ljh::ltrim_copy;
    using ljh::rtrim_copy;
    using ljh::trim_copy;
    using ljh::split;
    using ljh::convert_string;
}