
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// expected.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++20
//
// Implements http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0323r7.html
// 
// ABOUT
//     My version of std::expected
//     May not be complete or follow the paper correctly
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include <utility>
#include <memory>
#include <type_traits>

namespace _ljh
{
	template<bool copyable>
	struct not_copyable {};

	template<>
	struct not_copyable<true>
	{
		not_copyable() = default;
		not_copyable(const not_copyable&) = delete;
	};
}

namespace ljh
{
	using in_place_t = std::in_place_t;
	inline constexpr in_place_t in_place{};

	template<class T, class E>
	class expected;

	template<class E>
	class unexpected;
	template<class E>
	unexpected(E) -> unexpected<E>;

	struct unexpect_t {
		explicit unexpect_t() = default;
	};
	inline constexpr unexpect_t unexpect{};

	template<class E>
	class bad_expected_access;

	template<>
	class bad_expected_access<void>;

	template<class T, class E>
	class expected
		: _ljh::not_copyable<!(std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E>)>
	{
		using value_type = T;
		using error_type = E;
		using unexpected_type = unexpected<error_type>;

	private:
		struct _A{};
		using val_t = std::conditional_t<std::is_void_v<value_type>,_A,value_type>;
		using err_t = std::conditional_t<std::is_void_v<error_type>,_A,error_type>;

	public:
		template<class U>
		using rebind = expected<U, error_type>;

		template<typename T_ = value_type, typename = std::enable_if_t<
			std::is_default_constructible_v<T_> ||
			std::is_void_v<T_>
		>>
		constexpr expected()
			: has_val(true)
			, val(val_t{})
		{
		}

		template<typename T_ = value_type, typename E_ = error_type, typename = std::enable_if_t<
			std::is_copy_constructible_v<T_> &&
			std::is_copy_constructible_v<E_>
		>>
		constexpr expected(const expected& rhs)
			: has_val(rhs.has_val)
		{
			if (rhs)
			{
				val = rhs.val;
			}
			else
			{
				unex = rhs.unex;
			}
		}

		constexpr expected(expected&& rhs) noexcept
			: has_val(rhs.has_val)
		{
			if (rhs)
			{
				val = std::move(rhs.val);
				rhs.val.~val_t();
			}
			else
			{
				unex = std::move(rhs.unex);
				rhs.unex.~err_t();
			}
		}

		template<class U, class G>
		explicit constexpr expected(const expected<U, G>& rhs)
			: has_val(rhs.has_val)
		{
			if (rhs)
			{
				val = rhs.val;
			}
			else
			{
				unex = rhs.unex;
			}
		}
		template<class U, class G>
		explicit constexpr expected(expected<U, G>&& rhs)
			: has_val(rhs.has_val)
		{
			if (rhs)
			{
				val = std::move(rhs.val);
			}
			else
			{
				unex = std::move(rhs.error);
			}
		}

		template<class U = value_type, typename = std::enable_if_t<
			std::is_constructible_v<value_type,U&&> && 
			!std::is_same_v<std::remove_cvref_t<U>, in_place_t> &&
			!std::is_same_v<expected<value_type, E>, std::remove_cvref_t<U>> &&
			!std::is_same_v<unexpected<error_type>, std::remove_cvref_t<U>>
		>>
		explicit(!std::is_convertible_v<U&&, T>)
		constexpr expected(U&& rhs)
			: has_val(true)
			, val{std::forward<U>(rhs)}
		{
		}

		template<class G = error_type>
		explicit(!std::is_convertible_v<const G&, error_type>)
		constexpr expected(const unexpected<G>& rhs)
			: has_val(false)
			, unex(rhs.value())
		{
		}
		template<class G = error_type, typename = std::enable_if_t<
			std::is_constructible_v<error_type,G&&>
		>>
		explicit(!std::is_convertible_v<G&&, error_type>)
		constexpr expected(unexpected<G>&& rhs) noexcept(std::is_nothrow_constructible_v<error_type, G&&>)
			: has_val(false)
			, unex(std::move(rhs.value()))
		{
		}

		template<class... Args, typename = std::enable_if_t<
			(std::is_void_v<value_type> && sizeof...(Args) == 0) ||
			(!std::is_void_v<value_type> && std::is_constructible_v<value_type, Args...>)
		>>
		explicit constexpr expected(in_place_t, Args&&... args)
			: has_val(true)
			, val(std::forward<Args>(args)...)
		{
		}
		template<class U, class... Args, typename = std::enable_if_t<
			!std::is_void_v<value_type> && 
			std::is_constructible_v<value_type, std::initializer_list<U>&, Args...>
		>>
		explicit constexpr expected(in_place_t, std::initializer_list<U> il, Args&&... args)
			: has_val(true)
			, val(il, std::forward<Args>(args)...)
		{
		}
		template<class... Args, typename = std::enable_if_t<
			std::is_constructible_v<E, Args...>
		>>
		explicit constexpr expected(unexpect_t, Args&&... args)
			: has_val(false)
			, unex(std::forward<Args>(args)...)
		{
		}
		template<class U, class... Args, typename = std::enable_if_t<
			std::is_constructible_v<E, std::initializer_list<U>&, Args...>
		>>
		explicit constexpr expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
			: has_val(false)
			, unex(il, std::forward<Args>(args)...)
		{
		}

		~expected()
		{
			if (has_val)
			{
				val.~val_t();
			}
			else
			{
				unex.~err_t();
			}
		}

		expected& operator=(const expected& rhs)
		{
			if (has_val)
			{
				val.~val_t();
			}
			else
			{
				unex.~err_t();
			}
			has_val = rhs.has_val;
			if (rhs)
			{
				val = rhs.val;
			}
			else
			{
				unex = rhs.unex;
			}
			return *this;
		}
		expected& operator=(expected&& rhs) noexcept
		{
			if (has_val)
			{
				val.~val_t();
			}
			else
			{
				unex.~err_t();
			}
			has_val = rhs.has_val;
			if (rhs)
			{
				val = std::move(rhs.val);
			}
			else
			{
				unex = std::move(rhs.unex);
			}
			return *this;
		}
		template<class U = value_type>
		expected& operator=(U&&rhs)
		{
			if (!has_val)
			{
				unex.~err_t();
				has_val = true;
			}
			val = val_t(std::forward<U>(rhs));
			return *this;
		}
		template<class G = error_type>
		expected& operator=(const unexpected<G>& rhs)
		{
			if (has_val)
			{
				val.~val_t();
				has_val = false;
			}
			error = rhs.value();
			return *this;
		}
		template<class G = error_type>
		expected& operator=(unexpected<G>&& rhs)
		{
			if (has_val)
			{
				val.~val_t();
				has_val = false;
			}
			unex = std::move(rhs.value());
			return *this;
		}

		template<typename V = value_type>
		std::enable_if_t<
			std::is_void_v<V>,
		V> emplace()
		{
			if (!has_val)
			{
				unex.~err_t();
				has_val = true;
			}
		}
		template<class... Args, typename V = value_type>
		std::enable_if_t<
			!std::is_void_v<V>,
		V&> emplace(Args&&... args)
		{
			if constexpr (std::is_nothrow_constructible_v<value_type, Args...>)
			{
				if (!has_val)
				{
					unex.~err_t();
					has_val = true;
				}
				val = val_t{std::forward<Args>(args)...};
			}
			else if constexpr (std::is_nothrow_move_constructible_v<value_type>)
			{
				auto temp = val_t{std::forward<Args>(args)...};
				if (!has_val)
				{
					unex.~err_t();
					has_val = true;
				}
				val = std::move(temp);
			}
			else
			{
				if (!has_val)
				{
					unexpected<error_type> tmp{std::move(unex)};
					unex.~err_t();
					try
					{
						val = val_t{std::forward<Args>(args)...};
					}
					catch(...)
					{
						unex = std::move(tmp.value());
						throw;
					}
					has_val = true;
				}
				else
				{
					val = val_t{std::forward<Args>(args)...};
				}
			}
			return val;
		}
		template<class U, class... Args, typename V = value_type>
		std::enable_if_t<
			!std::is_void_v<V>,
		V&> emplace(std::initializer_list<U> il, Args&&... args)
		{
			if constexpr (std::is_nothrow_constructible_v<value_type, Args...>)
			{
				if (!has_val)
				{
					unex.~err_t();
					has_val = true;
				}
				val = val_t{il, std::forward<Args>(args)...};
			}
			else if constexpr (std::is_nothrow_move_constructible_v<value_type>)
			{
				auto temp = val_t{il, std::forward<Args>(args)...};
				if (!has_val)
				{
					unex.~err_t();
					has_val = true;
				}
				val = std::move(temp);
			}
			else
			{
				if (!has_val)
				{
					unexpected<error_type> tmp{std::move(unex)};
					unex.~err_t();
					try
					{
						val = val_t{il, std::forward<Args>(args)...};
					}
					catch(...)
					{
						unex = std::move(tmp.value());
						throw;
					}
					has_val = true;
				}
				else
				{
					val = val_t{il, std::forward<Args>(args)...};
				}
			}
			return val;
		}

		void swap(expected&) noexcept;

		template<typename V = value_type> constexpr const std::enable_if_t<!std::is_void_v<V>,V* > operator->() const   { if (!has_val) { throw bad_expected_access(error()); } return std::addressof(val); }
		template<typename V = value_type> constexpr       std::enable_if_t<!std::is_void_v<V>,V* > operator->()         { if (!has_val) { throw bad_expected_access(error()); } return std::addressof(val); }
		template<typename V = value_type> constexpr const std::enable_if_t<!std::is_void_v<V>,V& > operator* () const&  { if (!has_val) { throw bad_expected_access(error()); } return                val ; }
		template<typename V = value_type> constexpr       std::enable_if_t<!std::is_void_v<V>,V& > operator* ()      &  { if (!has_val) { throw bad_expected_access(error()); } return                val ; }
		template<typename V = value_type> constexpr const std::enable_if_t<!std::is_void_v<V>,V&&> operator* () const&& { if (!has_val) { throw bad_expected_access(error()); } return std::move     (val); }
		template<typename V = value_type> constexpr       std::enable_if_t<!std::is_void_v<V>,V&&> operator* ()      && { if (!has_val) { throw bad_expected_access(error()); } return std::move     (val); }
		template<typename V = value_type> constexpr const std::enable_if_t<!std::is_void_v<V>,V& > value     () const&  { if (!has_val) { throw bad_expected_access(error()); } return                val ; }
		template<typename V = value_type> constexpr       std::enable_if_t<!std::is_void_v<V>,V& > value     ()      &  { if (!has_val) { throw bad_expected_access(error()); } return                val ; }
		template<typename V = value_type> constexpr const std::enable_if_t<!std::is_void_v<V>,V&&> value     () const&& { if (!has_val) { throw bad_expected_access(error()); } return std::move     (val); }
		template<typename V = value_type> constexpr       std::enable_if_t<!std::is_void_v<V>,V&&> value     ()      && { if (!has_val) { throw bad_expected_access(error()); } return std::move     (val); }

		constexpr explicit operator bool () const noexcept { return has_value(); }
		constexpr          bool has_value() const noexcept { return has_val    ; }

		constexpr const error_type&  error() const&  { return           unex ; };
		constexpr       error_type&  error()      &  { return           unex ; };
		constexpr const error_type&& error() const&& { return std::move(unex); };
		constexpr       error_type&& error()      && { return std::move(unex); };
		
		template<class U>
		constexpr value_type value_or(U&& rhs) const&
		{
			return has_val ? val : static_cast<value_type>(std::forward<U>(rhs));
		}
		template<class U>
		constexpr value_type value_or(U&& rhs) &&
		{
			return has_val ? std::move(val) : static_cast<value_type>(std::forward<U>(rhs));
		}

		template<class T1, class E1, class T2, class E2>
		friend constexpr bool operator==(const expected<T1, E1>& lhs, const expected<T2, E2>& rhs);
		template<class T1, class E1, class T2, class E2>
		friend constexpr bool operator!=(const expected<T1, E1>& lhs, const expected<T2, E2>& rhs);

		template<class T1, class E1, class T2>
		friend constexpr bool operator==(const expected<T1, E1>& lhs, const T2& rhs);
		template<class T1, class E1, class T2>
		friend constexpr bool operator==(const T2& lhs, const expected<T1, E1>& rhs);
		template<class T1, class E1, class T2>
		friend constexpr bool operator!=(const expected<T1, E1>& lhs, const T2& rhs);
		template<class T1, class E1, class T2>
		friend constexpr bool operator!=(const T2& lhs, const expected<T1, E1>& rhs);

		template<class T1, class E1, class E2>
		friend constexpr bool operator==(const expected<T1, E1>& lhs, const unexpected<E2>& rhs);
		template<class T1, class E1, class E2>
		friend constexpr bool operator==(const unexpected<E2>& lhs, const expected<T1, E1>& rhs);
		template<class T1, class E1, class E2>
		friend constexpr bool operator!=(const expected<T1, E1>& lhs, const unexpected<E2>& rhs);
		template<class T1, class E1, class E2>
		friend constexpr bool operator!=(const unexpected<E2>& lhs, const expected<T1, E1>& rhs);

		template<class T1, class E1>
		friend void swap(expected<T1, E1>& lhs, expected<T1, E1>& rhs) noexcept;

	private:
		bool has_val;
		union
		{
			val_t val ;
			err_t unex;
		};
	};

	template<class E>
	class unexpected 
	{
	public:
		using value_type = E;

		constexpr unexpected(const unexpected&) = default;
		constexpr unexpected(unexpected&&) = default;
		template<class... Args>
		constexpr explicit unexpected(in_place_t, Args&&...args)
		{
			val = value_type(std::forward<Args>(args)...);
		}
		template<class U, class... Args>
		constexpr explicit unexpected(in_place_t, std::initializer_list<U> il, Args&&...args)
		{
			val = value_type(il, std::forward<Args>(args)...);
		}
		template<class Err = value_type>
		constexpr explicit unexpected(Err&& rhs)
		{
			val = value_type(std::forward<Err>(rhs));
		}
		template<class Err>
		constexpr explicit unexpected(const unexpected<Err>& rhs)
		{
			val = rhs.val;
		}
		template<class Err>
		constexpr explicit unexpected(unexpected<Err>&& rhs)
		{
			val = std::move(rhs.val);
			rhs.val.~Err();
		}

		constexpr unexpected& operator=(const unexpected&) = default;
		constexpr unexpected& operator=(unexpected&&) = default;
		template<class Err = value_type>
		constexpr unexpected& operator=(const unexpected<Err>&);
		template<class Err = value_type>
		constexpr unexpected& operator=(unexpected<Err>&&);

		constexpr const value_type&  value() const&  noexcept { return           val ; }
		constexpr       value_type&  value()      &  noexcept { return           val ; }
		constexpr const value_type&& value() const&& noexcept { return std::move(val); }
		constexpr       value_type&& value()      && noexcept { return std::move(val); }

		void swap(unexpected& other) noexcept;

		template<class E1, class E2>
		friend constexpr bool operator==(const unexpected<E1>&, const unexpected<E2>&);
		template<class E1, class E2>
		friend constexpr bool operator!=(const unexpected<E1>&, const unexpected<E2>&);

		template<class E1>
		friend void swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)));

	private:
		value_type val;
	};

	template<>
	class bad_expected_access<void> : public std::exception {
	public:
		explicit bad_expected_access() = default;
	};

	template<class E>
	class bad_expected_access : public bad_expected_access<void> {
	public:
		explicit bad_expected_access(E val) : val(val) {};

		virtual const char* what() const noexcept override
		{
			return "bad_expected_access";
		}

		      E&  error()      &  { return           val ; }
		const E&  error() const&  { return           val ; }
		      E&& error()      && { return std::move(val); }
		const E&& error() const&& { return std::move(val); }
	private:
		E val;
	};
}