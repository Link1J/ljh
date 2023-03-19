
//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// fixed_point.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
///
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include "int_types.hpp"
#include "type_traits.hpp"
#if LJH_CPP_VERSION > LJH_CPP17_VERSION
#    if __has_include(<compare>)
#        include <compare>
#    endif
#endif

namespace ljh
{
	template<std::size_t current>
	struct _f_storage_info 
	{
		static constexpr bool is_specialized = false;

		template<bool _sign>
		using t = void;
	};

	template<typename type>
	struct _f_storage_info_help
	{
		static constexpr bool is_specialized = true;
		using next = _f_storage_info<sizeof(type) * 2>;

		using s = std::make_signed_t<type>;
		using u = std::make_unsigned_t<type>;

		template<bool _sign>
		using t = std::conditional_t<_sign, s, u>;
	};

	template<>
	struct _f_storage_info<sizeof(u08)> : _f_storage_info_help<u08> {};

	template<>
	struct _f_storage_info<sizeof(u16)> : _f_storage_info_help<u16> {};

	template<>
	struct _f_storage_info<sizeof(u32)> : _f_storage_info_help<u32> {};

	template<>
	struct _f_storage_info<sizeof(u64)> : _f_storage_info_help<u64> {};

	template<std::size_t _storage_bits_size, std::size_t _factional_bits_size, bool _signed>
	struct fixed_point
	{
	private:
		static constexpr auto stor_size = _storage_bits_size;
		static constexpr auto fact_size = _factional_bits_size;

		using stor_info = _f_storage_info<stor_size / CHAR_BIT>;
		using next_info = typename stor_info::next;

		static_assert(stor_size > fact_size);
		static_assert(stor_info::is_specialized);

		using stor_t = typename stor_info::template t<_signed>;
		using stor_u = typename stor_info::u;
		using next_t = typename next_info::template t<_signed>;

		static constexpr stor_t i_mask = (~stor_u(0)) << fact_size;
		static constexpr stor_t f_mask = ~i_mask;
		static constexpr stor_t one    = stor_t(1) << fact_size;

	public:
		constexpr fixed_point() noexcept = default;

		template<typename _value>
		constexpr fixed_point(_value init_value) noexcept
		{
			if constexpr (std::is_floating_point<_value>::value)
				_data = stor_t(init_value * one);
			else if constexpr (std::is_convertible<stor_u, _value>::value)
				_data = stor_t(stor_u(init_value) << fact_size);
		}

		template<typename _value>
		constexpr explicit operator _value() const noexcept
		{
			if constexpr (std::is_floating_point<_value>::value)
				return _data / _value(one);
			else if constexpr (std::is_convertible<stor_t, _value>::value)
				return _value(_data >> fact_size);
		}

		constexpr fixed_point& operator+=(const fixed_point& rhs) noexcept
		{
			_data += rhs._data;
			return *this;
		}

		constexpr fixed_point& operator-=(const fixed_point& rhs) noexcept
		{
			_data -= rhs._data;
			return *this;
		}

		constexpr fixed_point& operator*=(const fixed_point& rhs) noexcept
		{
			if constexpr (next_info::is_specialized)
			{
				next_t t = next_t(_data) * next_t(rhs._data);
				_data = stor_t(t >> fact_size);
			}
			else
			{
				stor_u a_hi = (_data     & i_mask) >> fact_size;
				stor_u b_hi = (rhs._data & i_mask) >> fact_size;
				stor_u a_lo = (_data     & f_mask);
				stor_u b_lo = (rhs._data & f_mask);

				_data = (a_hi * b_hi << fact_size) + a_lo * b_hi + a_hi * b_lo + (a_lo * b_lo >> fact_size);
			}
			return *this;
		}

		constexpr fixed_point& operator/=(const fixed_point& rhs) noexcept
		{
			if constexpr (next_info::is_specialized)
			{
				next_t t = next_t(_data) << fact_size;
				_data = stor_t(t / rhs._data);
			}
			else
			{
				bool sign = false;

				stor_t rem = _data;
				if (rem < 0)
				{
					rem = -rem;
					sign = !sign;
				}

				stor_t div = rhs._data;
				if (div < 0)
				{
					div = -div;
					sign = !sign;
				}

				stor_u quo = 0;
				int b = stor_size / 2 + 1;

				while (rem && b >= 0)
				{
					stor_u t = rem / div;
					rem %= div;
					quo += t << b;

					rem <<= 1;
					--b;
				}

				_data = quo >> 1;
				_data *= (sign ? -1 : 1);
			}
			return *this;
		}
		
		friend constexpr fixed_point operator+(const fixed_point& lhs, const fixed_point& rhs)
		{
			fixed_point output = lhs;
			return output += rhs;
		}

		friend constexpr fixed_point operator-(const fixed_point& lhs, const fixed_point& rhs)
		{
			fixed_point output = lhs;
			return output -= rhs;
		}
		
		friend constexpr fixed_point operator*(const fixed_point& lhs, const fixed_point& rhs)
		{
			fixed_point output = lhs;
			return output *= rhs;
		}

		friend constexpr fixed_point operator/(const fixed_point& lhs, const fixed_point& rhs)
		{
			fixed_point output = lhs;
			return output /= rhs;
		}

#ifdef LJH_HAS_CPP20_COMPARISON
		constexpr std::strong_ordering operator<=>(const fixed_point& rhs) const noexcept
		{
			return _data <=> rhs._data;
		}
		constexpr bool operator==(const fixed_point&) const noexcept = default;
#else
		constexpr bool operator==(const fixed_point& rhs) const noexcept
		{
			return _data == rhs._data;
		}
		constexpr bool operator<(const fixed_point& rhs) const noexcept
		{
			return _data < rhs._data;
		}
		constexpr bool operator!=(const fixed_point& rhs) const noexcept
		{
			return !(*this == rhs);
		}
		constexpr bool operator>(const fixed_point& rhs) const noexcept
		{
			return rhs < *this;
		}
		constexpr bool operator>=(const fixed_point& rhs) const noexcept
		{
			return !(*this < rhs);
		}
		constexpr bool operator<=(const fixed_point& rhs) const noexcept
		{
			return !(rhs < *this);
		}
#endif
		constexpr stor_t as_raw() const noexcept
		{
			return _data;
		}

		static constexpr fixed_point from_raw(stor_t raw) noexcept
		{
			fixed_point out;
			out._data = raw;
			return out;
		}

	private:
		stor_t _data = 0;
	};
}

/*
namespace a {
using ft_f26d6_t = ljh::fixed_point<64, 32, true>;
constexpr ft_f26d6_t n = -50;
constexpr ft_f26d6_t d = 5;
constexpr ft_f26d6_t v = n / d;
constexpr double     a = double(v);
constexpr ljh::u64   b = v.as_raw();
static_assert(a == -10);
}
namespace b {
using ft_f26d6_t = ljh::fixed_point<32, 16, true>;
constexpr ft_f26d6_t n = -2;
constexpr ft_f26d6_t d = 5;
constexpr ft_f26d6_t v = n * d;
constexpr double     a = double(v);
constexpr ljh::u64   b = v.as_raw();
static_assert(a == -10);
}
namespace c {
using ft_f26d6_t = ljh::fixed_point<64, 16, true>;
constexpr ft_f26d6_t n = -2;
constexpr ft_f26d6_t d = 5;
constexpr ft_f26d6_t v = n * d;
constexpr double     a = double(v);
constexpr ljh::u64   b = v.as_raw();
static_assert(a == -10);
}
namespace d {
using ft_f26d6_t = ljh::fixed_point<64, 32, true>;
constexpr ft_f26d6_t n = -50;
constexpr ft_f26d6_t d = 5;
constexpr ft_f26d6_t v = n / d;
constexpr double     a = double(v);
constexpr ljh::u64   b = v.as_raw();
static_assert(a == -10);
} 
//*/