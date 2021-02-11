
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <initializer_list>
#include <type_traits>

#include <ljh/_i_internal/helpers.hpp>
#include <ljh/_i_internal/string_view.hpp>
#include <ljh/char_traits.hpp>

namespace ljh
{
	template<typename _i_char, typename _i_traits = char_traits<_i_char>>
	class _i_basic_string
	{
	public:
		using traits_type            = _i_traits;
		using value_type             = _i_char;
		using size_type              = size_t;
		using difference_type        = size_type;
		using pointer                = value_type*;
		using const_pointer          = const value_type*;
		using reference              = value_type&;
		using const_reference        = const value_type&;
		using iterator               = pointer;
		using const_iterator         = const_pointer;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		static const size_type npos = -1;

	private:
		using view_type = _i_basic_string_view<value_type, traits_type>;
		using cref_of_type = const _i_basic_string&;

		template<typename T>
		struct convert_to_view : std::integral_constant<bool, std::is_convertible<T, view_type>::value> {};
		template<typename T>
		struct convert_to_view_only : std::integral_constant<bool, convert_to_view<T>::value && !std::is_convertible<T, const value_type*>::value> {};

		union _i_data_t
		{
			struct _i_full
			{
				size_type size;
				size_type capacity;
				pointer data;
			} full;
			struct _i_sso
			{
				static constexpr auto len = sizeof(_i_data_t::_i_full) / sizeof(value_type);
				value_type data[len];
			} sso;

			_i_LJH_CONSTEXPR_CONTAINER _i_data_t(_i_sso a)
				: sso{a}
			{
				sso.data[_i_sso::len - 1] = _i_sso::len - 1;
			}
			_i_LJH_CONSTEXPR_CONTAINER _i_data_t(_i_full a)
				: full{a}
			{
				full.size     = 0;
				full.capacity = 0;
				full.data     = 0;
			}

			_i_LJH_CONSTEXPR_CONTAINER _i_data_t(bool is_sso)
				: _i_data_t{is_sso ? _i_data_t{_i_data_t::_i_sso()} : _i_data_t{_i_data_t::_i_full()}}
			{}

			_i_LJH_CONSTEXPR_CONTAINER _i_data_t()
				: _i_data_t{_i_data_t::_i_sso()}
			{}

		} _i_data;
		bool _i_is_full = false;

		static constexpr auto _i_sso_data_len = _i_data_t::_i_sso::len;

		_i_LJH_CONSTEXPR_CONTAINER void _i_setup(size_type count, size_type cap = 0)
		{
			if (count < _i_sso_data_len)
			{
				_i_is_full = false;
			}
			else
			{
				_i_is_full = true;
				if (cap < count)
					_i_data.full.capacity = _i_round_to_power2(count);
				else
					_i_data.full.capacity = _i_round_to_power2(cap);
				_i_data.full.data = new value_type[capacity() + 1];
				data()[capacity()] = 0;
			}

			_i_update_size(count);
			data()[size()] = 0;
		}

		template<typename _i_it>
		_i_LJH_CONSTEXPR_CONTAINER void _i_fill(_i_it first        , _i_it last     ) { for (auto it = first; it != last; it++) data()[it - first] = *it; }
		_i_LJH_CONSTEXPR_CONTAINER void _i_fill(const_pointer other, size_type count) { for (size_type a = 0; a < count ; a++ ) data()[a] =     other[a]; }
		_i_LJH_CONSTEXPR_CONTAINER void _i_fill(value_type    other, size_type count) { for (size_type a = 0; a < count ; a++ ) data()[a] =     other   ; }

		_i_LJH_CONSTEXPR_CONTAINER void _i_memory_cleanup() noexcept
		{
			if (_i_is_full) delete[] data();
		}

		_i_LJH_CONSTEXPR_CONTAINER void _i_update_size(size_type new_size) noexcept
		{
			if (!_i_is_full)
				_i_data.sso.data[_i_sso_data_len - 1] = (char)(capacity() - new_size);
			else
				_i_data.full.size = new_size;
		}

		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(_i_data_t&& in_data, bool&& in_is_full)
			: _i_data(_i_move(in_data)), _i_is_full(_i_move(in_is_full))
		{}

		_i_LJH_CONSTEXPR_CONTAINER void _i_create_space(size_type index, size_type count)
		{
			auto new_size = count + size();
			reserve(new_size);
			auto old_size = size();
			_i_update_size(new_size);
			data()[size()] = 0;
			for (int a = old_size - 1; a >= index; a--)
				data()[a + count] = data()[a];
		}

		_i_LJH_CONSTEXPR_CONTAINER const_pointer _i_get() const
		{
			if (!_i_is_full)
				return _i_data.sso.data;
			return _i_data.full.data;
		}

		_i_LJH_CONSTEXPR_CONTAINER void _i_reconfig(size_type count)
		{
			_i_memory_cleanup();
			_i_data = _i_move(_i_data_t{count < _i_sso_data_len});
			_i_setup(count);
		}

	public:
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string() noexcept = default;

		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(const_pointer s, size_type count)
			: _i_data(count < _i_sso_data_len)
		{
			_i_setup(count);
			_i_fill(s, count);
		}
		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(size_type count, value_type ch)
			: _i_data(count < _i_sso_data_len)
		{
			_i_setup(count);
			_i_fill(ch, count);
		}
		template<typename _i_it>
		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(_i_it first, _i_it last)
			: _i_data((last - first) < _i_sso_data_len)
		{
			_i_setup(last - first);
			_i_fill(first, last);
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string(const_pointer s)
			: _i_basic_string(s, traits_type::length(s))
		{}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string(_i_basic_string&& s) noexcept
			: _i_data(_i_move(s._i_data)), _i_is_full(s._i_is_full)
		{
			s._i_data = _i_move(_i_data_t{});
			s._i_is_full = false;
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string(std::initializer_list<value_type> list)
			: _i_basic_string(list.begin(), list.end())
		{}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string(cref_of_type s)
			: _i_basic_string(s.data(), s.size())
		{}
		template<typename T, typename _ = typename std::enable_if<convert_to_view<const T&>::value>::type>
		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(const T& t, size_type pos, size_type n)
			: _i_basic_string(view_type(t).substr(pos, n).data(), n)
		{}
		template<typename T, typename _ = typename std::enable_if<convert_to_view_only<const T&>::value>::type>
		explicit _i_LJH_CONSTEXPR_CONTAINER _i_basic_string(const T& t)
			: _i_basic_string(t, 0, view_type(t).size())
		{}

		_i_LJH_CONSTEXPR_CONTAINER ~_i_basic_string() noexcept
		{
			_i_memory_cleanup();
		}

		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(const_pointer s, size_type count)
		{
			if (s.data() == data()) return *this;
			_i_reconfig(count);
			_i_fill(s, count);
			return *this;
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(size_type count, value_type ch)
		{
			_i_reconfig(count);
			_i_fill(ch, count);
			return *this;
		}
		template<typename _i_it>
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(_i_it first, _i_it last)
		{
			_i_reconfig(last - first);
			_i_fill(first, last);
			return *this;
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(_i_basic_string&& s) noexcept
		{
			if (s.data() == data()) return *this;
			_i_memory_cleanup();
			_i_data = _i_move(s._i_data);
			_i_is_full = s._i_is_full;
			s._i_data = _i_move(_i_data_t{});
			s._i_is_full = false;
			return *this;
		}
		template<typename T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view<const T&>::value,
		_i_basic_string&>::type assign(const T& t, size_type pos, size_type count = npos)
		{
			view_type view(t);
			auto part = view.substr(pos, count);
			return assign(part.data(), part.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(cref_of_type s                        ) { return assign(s.data(), s.size()       ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(std::initializer_list<value_type> list) { return assign(list.begin(), list.end() ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& assign(const_pointer s                       ) { return assign(s, traits_type::length(s)); }
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view_only<const T&>::value, 
		_i_basic_string&>::type    assign(const T& t                            ) { return assign(t                        ); }

		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator=(cref_of_type s                        )          { return assign(s         ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator=(_i_basic_string&& s                   ) noexcept { return assign(_i_move(s)); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator=(value_type ch                         )          { return assign(&ch, 1    ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator=(std::initializer_list<value_type> list)          { return assign(list      ); }
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view_only<const T&>::value, 
		_i_basic_string&>::type    operator=(const T& t                            )          { return assign(t         ); }

		_i_LJH_CONSTEXPR_CONTAINER operator view_type() const noexcept
		{
			return view_type(data(), size());
		}

		// Element access
		_i_LJH_CONSTEXPR_CONTAINER       pointer data ()       noexcept { return const_cast<pointer>(_i_get()); }
		_i_LJH_CONSTEXPR_CONTAINER const_pointer data () const noexcept { return _i_get(); }
		_i_LJH_CONSTEXPR_CONTAINER const_pointer c_str() const noexcept { return data  (); }

		_i_LJH_CONSTEXPR_CONTAINER       reference at(size_type pos)       { if (pos >= size()) { throw std::out_of_range{""}; } return data()[pos]; }
		_i_LJH_CONSTEXPR_CONTAINER const_reference at(size_type pos) const { if (pos >= size()) { throw std::out_of_range{""}; } return data()[pos]; }
		_i_LJH_CONSTEXPR_CONTAINER reference       operator[](size_type pos)       { _i_LJH_CONSTEXPR_CONTAINER value_type temp; if (pos == size()) { return temp; } return at(pos); }
		_i_LJH_CONSTEXPR_CONTAINER const_reference operator[](size_type pos) const { _i_LJH_CONSTEXPR_CONTAINER value_type temp; if (pos == size()) { return temp; } return at(pos); }

		_i_LJH_CONSTEXPR_CONTAINER       reference front()       { return at(0         ); }
		_i_LJH_CONSTEXPR_CONTAINER const_reference front() const { return at(0         ); }
		_i_LJH_CONSTEXPR_CONTAINER       reference back ()       { return at(size() - 1); }
		_i_LJH_CONSTEXPR_CONTAINER const_reference back () const { return at(size() - 1); }


		// Search
		_i_LJH_CONSTEXPR_CONTAINER size_type find(const_pointer str, size_type pos, size_type count) const noexcept
		{
			auto location = traits_type::find(data() + pos, size() - pos, str, count);
			if (location == nullptr) return npos;
			return location - data();
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type find(cref_of_type str, size_type pos = 0) const noexcept
		{
			return find(str.data(), pos, str.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type find(const_pointer str, size_type pos = 0) const noexcept
		{
			return find(str, pos, traits_type::length(str));
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type find(value_type str, size_type pos = 0) const noexcept
		{
			return find(&str, pos, 1);
		}

		_i_LJH_CONSTEXPR_CONTAINER size_type rfind(const_pointer str, size_type pos, size_type count) const noexcept
		{
			auto location = traits_type::rfind(data() + pos, size() - pos, str, count);
			if (location == nullptr) return npos;
			return location - data();
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type rfind(cref_of_type str, size_type pos = npos) const noexcept
		{
			return rfind(str.data(), pos < size() - 1 ? pos : 0, str.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type rfind(const_pointer str, size_type pos = npos) const noexcept
		{
			return rfind(str, pos < size() - 1 ? pos : 0, traits_type::length(str));
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type rfind(value_type str, size_type pos = npos) const noexcept
		{
			return rfind(&str, pos < size() - 1 ? pos : 0, 1);
		}

		// Capacity
		_i_LJH_CONSTEXPR_CONTAINER size_type size() const noexcept
		{
			if (!_i_is_full)
				return capacity() - _i_data.sso.data[_i_sso_data_len - 1];
			return _i_data.full.size;
		}
		_i_LJH_CONSTEXPR_CONTAINER size_type length() const noexcept { return size(); }

		_i_LJH_CONSTEXPR_CONTAINER size_type capacity() const noexcept
		{
			if (!_i_is_full)
				return _i_sso_data_len - 1;
			return _i_data.full.capacity - 1;
		}

		[[nodiscard]] _i_LJH_CONSTEXPR_CONTAINER bool empty() const noexcept { return begin() == end(); }
		
		_i_LJH_CONSTEXPR_CONTAINER size_type max_size() const noexcept
		{
			return size_type(-1) / sizeof(value_type) - 1;
		}

		_i_LJH_CONSTEXPR_CONTAINER void reserve(size_type new_cap)
		{
			if (new_cap > capacity())
			{
				_i_data_t new_data; bool new_is_full = false;
				auto current_size = size();
				_i_swap(new_data   , _i_data   );
				_i_swap(new_is_full, _i_is_full);
				_i_setup(current_size, new_cap);
				_i_basic_string old_data(_i_move(new_data), _i_move(new_is_full));
				for (int a = 0; a < old_data.size(); a++)
					data()[a] = old_data.data()[a];
			}
		}
		_i_LJH_CONSTEXPR_CONTAINER void reserve() {} // No-op

		// Iterators
		_i_LJH_CONSTEXPR_CONTAINER       iterator         begin  ()       noexcept { return data  ()         ; }
		_i_LJH_CONSTEXPR_CONTAINER const_iterator         begin  () const noexcept { return data  ()         ; }
		_i_LJH_CONSTEXPR_CONTAINER const_iterator         cbegin () const noexcept { return data  ()         ; }
		_i_LJH_CONSTEXPR_CONTAINER       iterator         end    ()       noexcept { return data  () + size(); }
		_i_LJH_CONSTEXPR_CONTAINER const_iterator         end    () const noexcept { return data  () + size(); }
		_i_LJH_CONSTEXPR_CONTAINER const_iterator         cend   () const noexcept { return data  () + size(); }
		_i_LJH_CONSTEXPR_CONTAINER       reverse_iterator rbegin ()       noexcept { return end   () -      1; }
		_i_LJH_CONSTEXPR_CONTAINER const_reverse_iterator rbegin () const noexcept { return end   () -      1; }
		_i_LJH_CONSTEXPR_CONTAINER const_reverse_iterator crbegin() const noexcept { return cend  () -      1; }
		_i_LJH_CONSTEXPR_CONTAINER       reverse_iterator rend   ()       noexcept { return begin () -      1; }
		_i_LJH_CONSTEXPR_CONTAINER const_reverse_iterator rend   () const noexcept { return begin () -      1; }
		_i_LJH_CONSTEXPR_CONTAINER const_reverse_iterator crend  () const noexcept { return cbegin() -      1; }

		// Operations (Modify)
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& insert(size_type index, const_pointer s, size_type count)
		{
			if (index > size()) throw std::out_of_range{""};

			_i_create_space(index, count);
			for (int a = 0; a < count; a++)
				data()[index + a] = s[a];

			return *this;
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& insert(size_type index, cref_of_type str, size_type index_str, size_type count = npos)
		{
			if (index > size() || index_str > size()) throw std::out_of_range{""};
			return insert(index, str.data() + index_str, count != npos ? count : str.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& insert(size_type index, size_type count, value_type ch)
		{
			if (index > size()) throw std::out_of_range{""};

			_i_create_space(index, count);
			for (int a = 0; a < count; a++)
				data()[index + a] = ch;
			
			return *this;
		}
		template<typename _i_it>
		_i_LJH_CONSTEXPR_CONTAINER iterator insert(const_iterator pos, _i_it first, _i_it last)
		{
			size_type index = pos - begin();
			size_type count = last - first;
			_i_create_space(index, count);
			for (int a = 0; a < count; a++)
				data()[index + a] = first + a;
			return begin() + index;
		}
		_i_LJH_CONSTEXPR_CONTAINER iterator insert(const_iterator pos, size_type count, value_type ch)
		{
			size_type index = pos - begin();
			insert(index, count, ch);
			return begin() + index;
		}
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view<const T&>::value,
		_i_basic_string&>::type insert(size_type index, const T& t, size_type pos, size_type count = npos) 
		{
			view_type view(t);
			auto part = view.substr(pos, count);
			return insert(index, part.data(), part.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& insert(size_type index, const_pointer s                          ) { return insert(index, s, traits_type::length(s)); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& insert(size_type index, cref_of_type str                         ) { return insert(index, str.data(), str.size()   ); }
		_i_LJH_CONSTEXPR_CONTAINER iterator         insert(const_iterator pos, value_type ch                         ) { return insert(pos, 1, ch                      ); }
		_i_LJH_CONSTEXPR_CONTAINER iterator         insert(const_iterator pos, std::initializer_list<value_type> list) { return insert(pos, list.begin(), list.end()   ); }
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view_only<const T&>::value,
		_i_basic_string&>::type    insert(size_type index, const T& t                               ) { return insert(index, t                        ); }

		_i_LJH_CONSTEXPR_CONTAINER iterator erase(const_iterator first, const_iterator last) noexcept
		{
			auto start = const_cast<iterator>(first);
			auto diff = last - first;
			while (last != end())
				*start++ = *last++;
			_i_update_size(size() - diff);
			return const_cast<iterator>(first);
		}
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& erase(size_type index, size_type count)
		{
			if (index > size()) throw std::out_of_range{""};
			count = size() < count ? size() : count;
			erase(begin() + index, begin() + index + count);
			return *this;
		}
		_i_LJH_CONSTEXPR_CONTAINER iterator erase(const_iterator position) noexcept { return erase(position, position + 1); }
		
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& append(const_pointer s, size_type count                             ) { return insert(size(), s, count             ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& append(const_pointer s                                              ) { return insert(size(), s                    ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& append(cref_of_type str, size_type index_str, size_type count = npos) { return insert(size(), str, index_str, count); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& append(cref_of_type str                                             ) { return insert(size(), str                  ); }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& append(size_type count, value_type ch                               ) { return insert(size(), count, ch            ); }
		template<typename _i_it>
		_i_LJH_CONSTEXPR_CONTAINER iterator         append(_i_it first, _i_it last                                      ) { return insert(end (), first, last          ); }
		_i_LJH_CONSTEXPR_CONTAINER iterator         append(std::initializer_list<value_type> list                       ) { return insert(end (), list                 ); }
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view_only<const T&>::value,
		_i_basic_string&>::type    append(const T& t                                                   ) { return insert(size(), t                    ); }
		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view<const T&>::value,
		_i_basic_string&>::type    append(const T& t, size_type pos, size_type count = npos            ) { return insert(size(), t, pos, count        ); }

		_i_LJH_CONSTEXPR_CONTAINER void resize(size_type count, value_type ch)
		{
			if (count < size())
			{
				_i_update_size(count);
				data()[size()] = 0;
			}
			else if (count < capacity())
			{
				int a = size();
				_i_update_size(count);
				data()[size()] = 0;
				for (; a < size(); a++)
					data()[a] = ch;
			}
			else
			{
				int a = size();
				reserve(count);
				_i_update_size(count);
				data()[size()] = 0;
				for (; a < size(); a++)
					data()[a] = ch;
			}
		}
		_i_LJH_CONSTEXPR_CONTAINER void resize(size_type count) { resize(count, value_type()); }

		_i_LJH_CONSTEXPR_CONTAINER void push_back(value_type ch) { insert(end(), ch); }
		_i_LJH_CONSTEXPR_CONTAINER void pop_back (             ) { erase (end() - 1); }

		template<class T> _i_LJH_CONSTEXPR_CONTAINER typename std::enable_if<convert_to_view_only<const T&>::value,
		_i_basic_string&>::type    operator+=(const T&                          t   ) { return append   (t  )     ; }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator+=(cref_of_type                      str ) { return append   (str)     ; }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator+=(value_type                        ch  ) { return push_back(ch )     ; }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator+=(const_pointer                     s   ) { return append   (s  )     ; }
		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string& operator+=(std::initializer_list<value_type> list) { append(list); return *this; }

		_i_LJH_CONSTEXPR_CONTAINER void swap(_i_basic_string& str) noexcept
		{
			_i_swap(_i_data   , str._i_data   );
			_i_swap(_i_is_full, str._i_is_full);
		}

		_i_LJH_CONSTEXPR_CONTAINER void clear() const noexcept
		{
			for (int a = 0; a < capacity(); a++)
				data()[a] = 0;
		}

		// Operations (Duplicate)
		_i_LJH_CONSTEXPR_CONTAINER size_type copy(pointer dest, size_type count, size_type pos = 0) const
		{
			if (pos > size())
				throw std::out_of_range{""};
			auto randge = count < size() ? count : size();
			for (int a = 0; a < randge; a++)
				dest[a] = data()[pos + a];
			return randge;
		}

		_i_LJH_CONSTEXPR_CONTAINER _i_basic_string substr(size_type pos = 0, size_type count = npos) const
		{
			if (pos > size()) throw std::out_of_range{""};
			return _i_basic_string(data() + pos, count < size() ? count : size());
		}

		// Operations (Compare)
		_i_LJH_CONSTEXPR_CONTAINER int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const noexcept
		{
			auto state = traits_type::compare(data() + pos1, s, count1 < count2 ? count1 : count2);
			if (state != 0     ) return state;
			if (count1 < count2) return -1;
			if (count1 > count2) return  1;
			return 0;
		}
		_i_LJH_CONSTEXPR_CONTAINER int compare(size_type pos1, size_type count1, cref_of_type s, size_type pos2, size_type count2 = npos) const noexcept
		{
			return compare(pos1, count1, s.data() + pos2, count2 == npos ? s.size() : count2);
		}
		_i_LJH_CONSTEXPR_CONTAINER int compare(size_type pos1, size_type count1, cref_of_type s) const noexcept
		{
			return compare(pos1, count1, s.data(), s.size());
		}
		_i_LJH_CONSTEXPR_CONTAINER int compare(cref_of_type str) const noexcept
		{
			return compare(0, size(), str);
		}
		_i_LJH_CONSTEXPR_CONTAINER int compare(size_type pos1, size_type count1, const_pointer s) const noexcept
		{
			return compare(pos1, count1, s, traits_type::length(s));
		}
		_i_LJH_CONSTEXPR_CONTAINER int compare(const_pointer s) const noexcept
		{
			return compare(0, size(), s);
		}

		_i_LJH_CONSTEXPR_CONTAINER bool starts_with(const view_type& str) const noexcept { return find(str) ==                                   0; }
		_i_LJH_CONSTEXPR_CONTAINER bool starts_with(const_pointer    str) const noexcept { return find(str) ==                                   0; }
		_i_LJH_CONSTEXPR_CONTAINER bool starts_with(value_type       str) const noexcept { return find(str) ==                                   0; }
		_i_LJH_CONSTEXPR_CONTAINER bool ends_with  (const view_type& str) const noexcept { return find(str) == (size() - str.size()              ); }
		_i_LJH_CONSTEXPR_CONTAINER bool ends_with  (const_pointer    str) const noexcept { return find(str) == (size() - traits_type::length(str)); }
		_i_LJH_CONSTEXPR_CONTAINER bool ends_with  (value_type       str) const noexcept { return find(str) == (size() -                        1); }
		_i_LJH_CONSTEXPR_CONTAINER bool contains   (const view_type& str) const noexcept { return find(str) !=  npos                              ; }
		_i_LJH_CONSTEXPR_CONTAINER bool contains   (const_pointer    str) const noexcept { return find(str) !=  npos                              ; }
		_i_LJH_CONSTEXPR_CONTAINER bool contains   (value_type       str) const noexcept { return find(str) !=  npos                              ; }
		
		// Non-member functions
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER bool operator==(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs) noexcept;
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER bool operator==(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs) noexcept;
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER std::weak_ordering operator<=>(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs) noexcept;
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER std::weak_ordering operator<=>(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs) noexcept;
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, _i_char1 rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_char1* lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_char1 lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, const _i_char1* rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, _i_char1 rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_char1* lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs);
		//template<typename _i_char1, typename _i_traits1>
		//friend _i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_char1 lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs);
	};

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER bool operator==(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs) noexcept
	{
		if (lhs.size() != rhs.size())
			return false;
		return lhs.compare(rhs) == 0;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER bool operator==(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs) noexcept
	{
		auto len = _i_traits1::length(rhs);
		if (lhs.size() != len)
			return false;
		return lhs.compare(rhs) == 0;
	}

#if LJH_CPP_VERSION > LJH_CPP17_VERSION
	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER std::weak_ordering operator<=>(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs) noexcept
	{
		return static_cast<decltype(lhs, rhs)>(lhs.compare(rhs) <=> 0);
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER std::weak_ordering operator<=>(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs) noexcept
	{
		return static_cast<decltype(lhs, rhs)>(lhs.compare(rhs) <=> 0);
	}
#endif

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, const _i_char1* rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_basic_string<_i_char1, _i_traits1>& lhs, _i_char1 rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_char1* lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_char1 lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, const _i_basic_string<_i_char1, _i_traits1>& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, const _i_char1* rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_basic_string<_i_char1, _i_traits1>&& lhs, _i_char1 rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(const _i_char1* lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

	template<typename _i_char1, typename _i_traits1>
	_i_LJH_CONSTEXPR_CONTAINER _i_basic_string<_i_char1, _i_traits1> operator+(_i_char1 lhs, _i_basic_string<_i_char1, _i_traits1>&& rhs)
	{
		_i_basic_string<_i_char1, _i_traits1> new_string = lhs;
		new_string += rhs;
		return new_string;
	}

#if __cpp_deduction_guides >= 201703L
	template<typename InputIt>
	_i_basic_string(InputIt, InputIt)
	-> _i_basic_string<typename std::iterator_traits<InputIt>::value_type, std::char_traits<typename std::iterator_traits<InputIt>::value_type>>;
	
	template<typename CharT, typename Traits>
	explicit _i_basic_string(_i_basic_string_view<CharT, Traits>)
	-> _i_basic_string<CharT, Traits>;

	template<typename CharT, typename Traits>
	_i_basic_string(_i_basic_string_view<CharT, Traits>, typename _i_basic_string<CharT, Traits>::size_type, typename _i_basic_string<CharT, Traits>::size_type)
	-> _i_basic_string<CharT, Traits>;
#endif
}