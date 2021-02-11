
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <ljh/_i_internal/helpers.hpp>
#include <ljh/char_traits.hpp>

namespace ljh
{
	template<typename _i_char, typename _i_traits = char_traits<_i_char>>
	class _i_basic_string_view
	{
	public:
		using traits_type            = _i_traits;
		using value_type             = _i_char;
		using size_type              = size_t;
		using difference_type        = size_type;
		using pointer                = const value_type*;
		using const_pointer          = const value_type*;
		using reference              = const value_type&;
		using const_reference        = const value_type&;
		using iterator               = pointer;
		using const_iterator         = const_pointer;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		static const size_type npos = -1;

	private:
		pointer   _i_data = nullptr;
		size_type _i_size = 0;

	public:
		constexpr _i_basic_string_view() noexcept = default;
		constexpr _i_basic_string_view(const _i_basic_string_view& other) noexcept = default;

		explicit constexpr _i_basic_string_view(const_pointer s, size_type count)
			: _i_data(s), _i_size(count)
		{}
		template<typename _i_it>
		explicit constexpr _i_basic_string_view(_i_it first, _i_it last)
			: _i_data(&*first), _i_size(last - first)
		{}
		constexpr _i_basic_string_view(const_pointer s)
			: _i_basic_string_view(s, traits_type::length(s))
		{}

		constexpr _i_basic_string_view& operator=(const _i_basic_string_view& view) noexcept = default;

		// Element access
		constexpr const_pointer data () const noexcept { return _i_data; }

		constexpr const_reference at(size_type pos) const { if (pos >= size()) { throw std::out_of_range{""}; } return data()[pos]; }
		constexpr const_reference operator[](size_type pos) const { return at(pos); }

		constexpr const_reference front() const { return at(0         ); }
		constexpr const_reference back () const { return at(size() - 1); }

		// Search
		constexpr size_type find(const_pointer str, size_type pos, size_type count) const noexcept
		{
			auto location = traits_type::find(data() + pos, size() - pos, str, count);
			if (location == nullptr) return npos;
			return location - data();
		}
		constexpr size_type find(const _i_basic_string_view& str, size_type pos = 0) const noexcept
		{
			return find(str.data(), pos, str.size());
		}
		constexpr size_type find(const_pointer str, size_type pos = 0) const noexcept
		{
			return find(str, pos, traits_type::length(str));
		}
		constexpr size_type find(value_type str, size_type pos = 0) const noexcept
		{
			return find(&str, pos, 1);
		}

		constexpr size_type rfind(const_pointer str, size_type pos, size_type count) const noexcept
		{
			auto location = traits_type::rfind(data() + pos, size() - pos, str, count);
			if (location == nullptr) return npos;
			return location - data();
		}
		constexpr size_type rfind(const _i_basic_string_view& str, size_type pos = npos) const noexcept
		{
			return rfind(str.data(), pos < size() - 1 ? pos : 0, str.size());
		}
		constexpr size_type rfind(const_pointer str, size_type pos = npos) const noexcept
		{
			return rfind(str, pos < size() - 1 ? pos : 0, traits_type::length(str));
		}
		constexpr size_type rfind(value_type str, size_type pos = npos) const noexcept
		{
			return rfind(&str, pos < size() - 1 ? pos : 0, 1);
		}

		// Capacity
		constexpr size_type size() const noexcept { return _i_size; }
		constexpr size_type length() const noexcept { return size(); }
		[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }	
		constexpr size_type max_size() const noexcept
		{
			return size_type(-1) / sizeof(value_type) - 1;
		}

		// Iterators
		constexpr const_iterator         begin  () const noexcept { return data  ()         ; }
		constexpr const_iterator         cbegin () const noexcept { return data  ()         ; }
		constexpr const_iterator         end    () const noexcept { return data  () + size(); }
		constexpr const_iterator         cend   () const noexcept { return data  () + size(); }
		constexpr const_reverse_iterator rbegin () const noexcept { return end   () - 1     ; }
		constexpr const_reverse_iterator crbegin() const noexcept { return cend  () - 1     ; }
		constexpr const_reverse_iterator rend   () const noexcept { return begin () - 1     ; }
		constexpr const_reverse_iterator crend  () const noexcept { return cbegin() - 1     ; }

		// Operations (Modify)
		constexpr void swap(_i_basic_string_view& str) noexcept
		{
			_i_swap(_i_data, str._i_data);
			_i_swap(_i_size, str._i_size);
		}
		constexpr void remove_prefix(size_type n) noexcept { _i_data += n > _i_size ? _i_size : n; }
		constexpr void remove_suffix(size_type n) noexcept { _i_size -= n > _i_size ? _i_size : n; }

		// Operations (Duplicate)
		constexpr size_type copy(pointer dest, size_type count, size_type pos = 0) const
		{
			if (pos > size())
				throw std::out_of_range{""};
			auto range = count < size() ? count : size();
			for (int a = 0; a < range; a++)
				dest[a] = data()[pos + a];
			return range;
		}

		constexpr _i_basic_string_view substr(size_type pos = 0, size_type count = npos) const
		{
			if (pos > size()) throw std::out_of_range{""};
			return _i_basic_string_view(data() + pos, count < size() ? count : size());
		}

		// Operations (Compare)
		constexpr int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const noexcept
		{
			auto state = traits_type::compare(data() + pos1, s, count1 < count2 ? count1 : count2);
			if (state != 0     ) return state;
			if (count1 < count2) return -1;
			if (count1 > count2) return  1;
			return 0;
		}
		constexpr int compare(size_type pos1, size_type count1, const _i_basic_string_view& s, size_type pos2, size_type count2 = npos) const noexcept
		{
			return compare(pos1, count1, s.data() + pos2, count2 == npos ? s.size() : count2);
		}
		constexpr int compare(size_type pos1, size_type count1, const _i_basic_string_view& s) const noexcept
		{
			return compare(pos1, count1, s.data(), s.size());
		}
		constexpr int compare(const _i_basic_string_view& str) const noexcept
		{
			return compare(0, size(), str);
		}
		constexpr int compare(size_type pos1, size_type count1, const_pointer s) const noexcept
		{
			return compare(pos1, count1, s, traits_type::length(s));
		}
		constexpr int compare(const_pointer s) const noexcept
		{
			return compare(0, size(), s);
		}

		constexpr bool starts_with(const _i_basic_string_view& str) const noexcept { return find(str) ==                                   0; }
		constexpr bool starts_with(const_pointer               str) const noexcept { return find(str) ==                                   0; }
		constexpr bool starts_with(value_type                  str) const noexcept { return find(str) ==                                   0; }
		constexpr bool ends_with  (const _i_basic_string_view& str) const noexcept { return find(str) == (size() - str.size()              ); }
		constexpr bool ends_with  (const_pointer               str) const noexcept { return find(str) == (size() - traits_type::length(str)); }
		constexpr bool ends_with  (value_type                  str) const noexcept { return find(str) == (size() -                        1); }
		constexpr bool contains   (const _i_basic_string_view& str) const noexcept { return find(str) !=  npos                              ; }
		constexpr bool contains   (const_pointer               str) const noexcept { return find(str) !=  npos                              ; }
		constexpr bool contains   (value_type                  str) const noexcept { return find(str) !=  npos                              ; }
	};

	template<typename _i_char1, typename _i_traits>
	constexpr bool operator==(const _i_basic_string_view<_i_char1, _i_traits>& lhs, const _i_basic_string_view<_i_char1, _i_traits>& rhs) noexcept
	{
		if (lhs.size() != rhs.size())
			return false;
		return lhs.compare(rhs) == 0;
	}

	template<typename _i_char1, typename _i_traits>
	constexpr bool operator==(const _i_basic_string_view<_i_char1, _i_traits>& lhs, const _i_char1* rhs) noexcept
	{
		auto len = _i_traits::length(rhs);
		if (lhs.size() != len)
			return false;
		return lhs.compare(rhs) == 0;
	}

#if LJH_CPP_VERSION > LJH_CPP17_VERSION
	template<typename _i_char1, typename _i_traits>
	constexpr std::weak_ordering operator<=>(const _i_basic_string_view<_i_char1, _i_traits>& lhs, const _i_basic_string_view<_i_char1, _i_traits>& rhs) noexcept
	{
		return static_cast<decltype(lhs, rhs)>(lhs.compare(rhs) <=> 0);
	}

	template<typename _i_char1, typename _i_traits>
	constexpr std::weak_ordering operator<=>(const _i_basic_string_view<_i_char1, _i_traits>& lhs, const _i_char1* rhs) noexcept
	{
		return static_cast<decltype(lhs, rhs)>(lhs.compare(rhs) <=> 0);
	}
#endif
}