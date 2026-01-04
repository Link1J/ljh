
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// expected.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
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
#include "cpp_version.hpp"
#include "type_traits.hpp"

namespace _ljh
{
    template<bool condition>
    struct copy_constructible;

    template<>
    struct copy_constructible<true>
    {
        constexpr copy_constructible() noexcept                                     = default;
        constexpr copy_constructible(copy_constructible const&) noexcept            = default;
        constexpr copy_constructible(copy_constructible&&) noexcept                 = default;
        constexpr copy_constructible& operator=(copy_constructible const&) noexcept = default;
        constexpr copy_constructible& operator=(copy_constructible&&) noexcept      = default;
    };

    template<>
    struct copy_constructible<false>
    {
        constexpr copy_constructible() noexcept                                     = default;
        constexpr copy_constructible(copy_constructible const&) noexcept            = delete;
        constexpr copy_constructible(copy_constructible&&) noexcept                 = default;
        constexpr copy_constructible& operator=(copy_constructible const&) noexcept = default;
        constexpr copy_constructible& operator=(copy_constructible&&) noexcept      = default;
    };

    template<bool condition>
    struct move_constructible;

    template<>
    struct move_constructible<true>
    {
        constexpr move_constructible() noexcept                                     = default;
        constexpr move_constructible(move_constructible const&) noexcept            = default;
        constexpr move_constructible(move_constructible&&) noexcept                 = default;
        constexpr move_constructible& operator=(move_constructible const&) noexcept = default;
        constexpr move_constructible& operator=(move_constructible&&) noexcept      = default;
    };

    template<>
    struct move_constructible<false>
    {
        constexpr move_constructible() noexcept                                     = default;
        constexpr move_constructible(move_constructible const&) noexcept            = default;
        constexpr move_constructible(move_constructible&&) noexcept                 = delete;
        constexpr move_constructible& operator=(move_constructible const&) noexcept = default;
        constexpr move_constructible& operator=(move_constructible&&) noexcept      = default;
    };

    template<bool condition>
    struct copy_assignable;

    template<>
    struct copy_assignable<true>
    {
        constexpr copy_assignable() noexcept                                  = default;
        constexpr copy_assignable(copy_assignable const&) noexcept            = default;
        constexpr copy_assignable(copy_assignable&&) noexcept                 = default;
        constexpr copy_assignable& operator=(copy_assignable const&) noexcept = default;
        constexpr copy_assignable& operator=(copy_assignable&&) noexcept      = default;
    };

    template<>
    struct copy_assignable<false>
    {
        constexpr copy_assignable() noexcept                                  = default;
        constexpr copy_assignable(copy_assignable const&) noexcept            = default;
        constexpr copy_assignable(copy_assignable&&) noexcept                 = default;
        constexpr copy_assignable& operator=(copy_assignable const&) noexcept = delete;
        constexpr copy_assignable& operator=(copy_assignable&&) noexcept      = default;
    };

    template<bool condition>
    struct move_assignable;

    template<>
    struct move_assignable<true>
    {
        constexpr move_assignable() noexcept                                  = default;
        constexpr move_assignable(move_assignable const&) noexcept            = default;
        constexpr move_assignable(move_assignable&&) noexcept                 = default;
        constexpr move_assignable& operator=(move_assignable const&) noexcept = default;
        constexpr move_assignable& operator=(move_assignable&&) noexcept      = default;
    };

    template<>
    struct move_assignable<false>
    {
        constexpr move_assignable() noexcept                                  = default;
        constexpr move_assignable(move_assignable const&) noexcept            = default;
        constexpr move_assignable(move_assignable&&) noexcept                 = default;
        constexpr move_assignable& operator=(move_assignable const&) noexcept = default;
        constexpr move_assignable& operator=(move_assignable&&) noexcept      = delete;
    };
} // namespace _ljh

namespace ljh
{
    LJH_MODULE_MAIN_EXPORT using in_place_t = std::in_place_t;
    LJH_MODULE_MAIN_EXPORT inline constexpr in_place_t in_place{};

    LJH_MODULE_MAIN_EXPORT template<typename T, typename E>
    struct expected;

    LJH_MODULE_MAIN_EXPORT template<typename E>
    struct unexpected;
    template<typename E>
    unexpected(E) -> unexpected<E>;

    LJH_MODULE_MAIN_EXPORT struct unexpect_t
    {
        explicit unexpect_t() = default;
    };
    LJH_MODULE_MAIN_EXPORT inline constexpr unexpect_t unexpect{};

    LJH_MODULE_MAIN_EXPORT template<typename E>
    struct bad_expected_access;

    template<>
    struct bad_expected_access<void>;

    template<typename T, typename E>
    struct expected_storage
    {
        struct _A
        {};
        using val_t = std::conditional_t<std::is_void_v<T>, _A, T>;
        using err_t = std::conditional_t<std::is_void_v<E>, _A, E>;

        bool has_val;
        union
        {
            val_t val;
            err_t unex;
        };

        template<typename T_ = void, typename = std::enable_if_t<std::is_default_constructible_v<T> || std::is_void_v<T>, T_>>
        constexpr expected_storage()
            : has_val(true)
            , val(val_t{})
        {}

        constexpr expected_storage(expected_storage const& rhs)
            : has_val(rhs.has_val)
        {
            if (rhs.has_val)
            {
                val = rhs.val;
            }
            else
            {
                unex = rhs.unex;
            }
        }

        constexpr expected_storage(expected_storage&& rhs) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
            : has_val(rhs.has_val)
        {
            if (rhs.has_val)
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

        constexpr expected_storage(bool has_val, val_t& rhs) noexcept(std::is_nothrow_copy_constructible_v<val_t>)
            : has_val(has_val)
            , val(rhs)
        {}

        constexpr expected_storage(bool has_val, val_t&& rhs) noexcept(std::is_nothrow_move_constructible_v<val_t>)
            : has_val(has_val)
            , val(std::move(rhs))
        {}

        constexpr expected_storage(bool has_val, err_t& rhs, unexpect_t) noexcept(std::is_nothrow_copy_constructible_v<err_t>)
            : has_val(has_val)
            , unex(rhs)
        {}

        constexpr expected_storage(bool has_val, err_t&& rhs, unexpect_t) noexcept(std::is_nothrow_move_constructible_v<err_t>)
            : has_val(has_val)
            , unex(std::move(rhs))
        {}

        expected_storage& operator=(expected_storage const& rhs)
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
            if (rhs.has_val)
            {
                val = rhs.val;
            }
            else
            {
                unex = rhs.unex;
            }
            return *this;
        }

        expected_storage& operator=(expected_storage&& rhs) noexcept
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
            if (rhs.has_val)
            {
                val = std::move(rhs.val);
            }
            else
            {
                unex = std::move(rhs.unex);
            }
            return *this;
        }

        ~expected_storage()
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
    };

    template<typename T, typename E>
    struct expected_traits
    {
        static constexpr bool copy_constructible = (std::is_void_v<T> || std::is_copy_constructible_v<T>) && std::is_copy_constructible_v<E>;
        static constexpr bool move_constructible = (std::is_void_v<T> || std::is_move_constructible_v<T>) && std::is_move_constructible_v<E>;
        static constexpr bool copy_assignable =
            (std::is_void_v<T> && std::is_copy_assignable_v<E> && std::is_copy_constructible_v<E>) ||
            (!std::is_void_v<T> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<E> &&
             std::is_copy_constructible_v<E> && (std::is_nothrow_move_constructible_v<E> || std::is_nothrow_move_constructible_v<T>));
        static constexpr bool move_assignable = (std::is_void_v<T> || (std::is_move_assignable_v<T> && std::is_move_constructible_v<T>)) &&
                                                std::is_nothrow_move_constructible_v<E> && std::is_nothrow_move_assignable_v<E>;
    };

    template<typename T, typename E>
    struct expected : _ljh::copy_constructible<expected_traits<T, E>::copy_constructible>,
                      _ljh::move_constructible<expected_traits<T, E>::move_constructible>,
                      _ljh::copy_assignable<expected_traits<T, E>::copy_assignable>,
                      _ljh::move_assignable<expected_traits<T, E>::move_assignable>
    {
        expected_storage<T, E> storage;
        using val_t = typename expected_storage<T, E>::val_t;
        using err_t = typename expected_storage<T, E>::err_t;

    public:
        using value_type      = T;
        using error_type      = E;
        using unexpected_type = unexpected<error_type>;

        template<typename U>
        using rebind = expected<U, error_type>;

        constexpr expected()                           = default;
        constexpr expected(expected const&)            = default;
        constexpr expected(expected&&)                 = default;
        constexpr expected& operator=(expected const&) = default;
        constexpr expected& operator=(expected&&)      = default;

        template<
            typename U, typename G,
            typename = std::enable_if_t<
                std::is_constructible_v<T, U const&> && !std::is_constructible_v<T, expected<U, G>&> && !std::is_constructible_v<T, expected<U, G>&&> &&
                !std::is_constructible_v<T, expected<U, G> const&> && !std::is_constructible_v<T, expected<U, G> const&&> &&
                !std::is_convertible_v<expected<U, G>&, T> && !std::is_convertible_v<expected<U, G>&&, T> && !std::is_convertible_v<expected<U, G> const&, T> &&
                !std::is_convertible_v<expected<U, G> const&&, T> && std::is_constructible_v<E, G const&> &&
                !std::is_constructible_v<unexpected<E>, expected<U, G>&> && !std::is_constructible_v<unexpected<E>, expected<U, G>&&> &&
                !std::is_constructible_v<unexpected<E>, expected<U, G> const&> && !std::is_constructible_v<unexpected<E>, expected<U, G> const&&> &&
                !std::is_convertible_v<expected<U, G>&, unexpected<E>> && !std::is_convertible_v<expected<U, G>&&, unexpected<E>> &&
                !std::is_convertible_v<expected<U, G> const&, unexpected<E>> && !std::is_convertible_v<expected<U, G> const&&, unexpected<E>>>,
            U>
        LJH_CPP20_EXPLICIT((!(std::is_void_v<T> && std::is_void_v<U>) && !std::is_convertible_v<U const&, T>) || !std::is_convertible_v<G const&, E>)
        constexpr expected(expected<U, G> const& rhs)
            : storage(rhs.storage.has_val)
        {
            if (rhs)
            {
                storage.val = rhs.storage.val;
            }
            else
            {
                storage.unex = rhs.storage.unex;
            }
        }
        template<typename U, typename G,
                 typename = std::enable_if_t<
                     std::is_constructible_v<T, U&&> && !std::is_constructible_v<T, expected<U, G>&> && !std::is_constructible_v<T, expected<U, G>&&> &&
                     !std::is_constructible_v<T, expected<U, G> const&> && !std::is_constructible_v<T, expected<U, G> const&&> &&
                     !std::is_convertible_v<expected<U, G>&, T> && !std::is_convertible_v<expected<U, G>&&, T> &&
                     !std::is_convertible_v<expected<U, G> const&, T> && !std::is_convertible_v<expected<U, G> const&&, T> && std::is_constructible_v<E, G&&> &&
                     !std::is_constructible_v<unexpected<E>, expected<U, G>&> && !std::is_constructible_v<unexpected<E>, expected<U, G>&&> &&
                     !std::is_constructible_v<unexpected<E>, expected<U, G> const&> && !std::is_constructible_v<unexpected<E>, expected<U, G> const&&> &&
                     !std::is_convertible_v<expected<U, G>&, unexpected<E>> && !std::is_convertible_v<expected<U, G>&&, unexpected<E>> &&
                     !std::is_convertible_v<expected<U, G> const&, unexpected<E>> && !std::is_convertible_v<expected<U, G> const&&, unexpected<E>>>,
                 U>
        LJH_CPP20_EXPLICIT((!(std::is_void_v<T> && std::is_void_v<U>) && !std::is_convertible_v<U&&, T>) || !std::is_convertible_v<G&&, E>)
        constexpr expected(expected<U, G>&& rhs)
            : storage(rhs.storage.has_val)
        {
            if (rhs)
            {
                storage.val = std::move(rhs.storage.val);
            }
            else
            {
                storage.unex = std::move(rhs.storage.unex);
            }
        }

        template<typename U = value_type,
                 typename   = std::enable_if_t<!std::is_void_v<T> && std::is_constructible_v<T, U&&> && !std::is_same_v<remove_cvref_t<U>, in_place_t> &&
                                               !std::is_same_v<expected<value_type, E>, remove_cvref_t<U>> &&
                                               !std::is_same_v<unexpected<error_type>, remove_cvref_t<U>>>>
        LJH_CPP20_EXPLICIT(!std::is_convertible_v<U&&, T>)
        constexpr expected(U&& rhs)
            : storage(true, val_t{std::forward<U>(rhs)})
        {}

        template<typename G = error_type, typename = std::enable_if_t<std::is_constructible_v<error_type, G const&>>>
        LJH_CPP20_EXPLICIT(!std::is_convertible_v<G const&, error_type>)
        constexpr expected(unexpected<G> const& rhs)
            : storage(false, rhs.value(), unexpect)
        {}
        template<typename G = error_type, typename = std::enable_if_t<std::is_constructible_v<error_type, G&&>>>
        LJH_CPP20_EXPLICIT(!std::is_convertible_v<G&&, error_type>)
        constexpr expected(unexpected<G>&& rhs) noexcept(std::is_nothrow_constructible_v<error_type, G&&>)
            : storage(false, std::move(rhs.error()), unexpect)
        {}

        template<class... Args, typename = std::enable_if_t<(std::is_void_v<value_type> && sizeof...(Args) == 0) ||
                                                            (!std::is_void_v<value_type> && std::is_constructible_v<value_type, Args...>)>>
        explicit constexpr expected(in_place_t, Args&&... args)
            : storage(true, val_t{std::forward<Args>(args)...})
        {}
        template<typename U, class... Args,
                 typename = std::enable_if_t<!std::is_void_v<value_type> && std::is_constructible_v<value_type, std::initializer_list<U>&, Args...>>>
        explicit constexpr expected(in_place_t, std::initializer_list<U> il, Args&&... args)
            : storage(true, val_t{il, std::forward<Args>(args)...})
        {}
        template<class... Args, typename = std::enable_if_t<std::is_constructible_v<E, Args...>>>
        explicit constexpr expected(unexpect_t, Args&&... args)
            : storage(false, err_t{std::forward<Args>(args)...}, unexpect)
        {}
        template<typename U, class... Args, typename = std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args...>>>
        explicit constexpr expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
            : storage(false, err_t{il, std::forward<Args>(args)...}, unexpect)
        {}

        template<typename U = value_type,
                 typename   = std::enable_if_t<!std::is_void_v<T> && !std::is_same_v<expected<T, E>, remove_cvref_t<U>> &&
                                               !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> && std::is_constructible_v<T, U> &&
                                               std::is_assignable_v<val_t&, U> && std::is_nothrow_move_constructible_v<E>>>
        expected& operator=(U&& rhs)
        {
            if (!storage.has_val)
            {
                storage.unex.~err_t();
                storage.has_val = true;
            }
            new (&storage.val) val_t(std::forward<U>(rhs));
            return *this;
        }
        template<typename G = error_type, typename GF = G const&,
                 typename = std::enable_if_t<std::is_constructible_v<E, GF> && std::is_assignable_v<E&, GF> &&
                                             (std::is_nothrow_constructible_v<E, GF> || std::is_nothrow_move_constructible_v<T> ||
                                              std::is_nothrow_move_constructible_v<E>)>>
        expected& operator=(unexpected<G> const& rhs)
        {
            if (storage.has_val)
            {
                storage.val.~val_t();
                storage.has_val = false;
            }
            new (&storage.unex) err_t(rhs.error());
            return *this;
        }
        template<typename G = error_type, typename GF = G,
                 typename = std::enable_if_t<std::is_constructible_v<E, GF> && std::is_assignable_v<E&, GF> &&
                                             (std::is_nothrow_constructible_v<E, GF> || std::is_nothrow_move_constructible_v<T> ||
                                              std::is_nothrow_move_constructible_v<E>)>>
        expected& operator=(unexpected<G>&& rhs)
        {
            if (storage.has_val)
            {
                storage.val.~val_t();
                storage.has_val = false;
            }
            new (&storage.unex) err_t(std::move(rhs.error()));
            return *this;
        }

        template<typename V = value_type>
        std::enable_if_t<std::is_void_v<V>, V> emplace()
        {
            if (!storage.has_val)
            {
                storage.unex.~err_t();
                storage.has_val = true;
            }
        }
        template<class... Args, typename V = value_type>
        std::enable_if_t<!std::is_void_v<V>, V&> emplace(Args&&... args)
        {
            if constexpr (std::is_nothrow_constructible_v<value_type, Args...>)
            {
                if (!storage.has_val)
                {
                    storage.unex.~err_t();
                    storage.has_val = true;
                }

                new (&storage.val) val_t{std::forward<Args>(args)...};
            }
            else if constexpr (std::is_nothrow_move_constructible_v<value_type>)
            {
                auto temp = val_t{std::forward<Args>(args)...};
                if (!storage.has_val)
                {
                    storage.unex.~err_t();
                    storage.has_val = true;
                }
                storage.val = std::move(temp);
            }
            else
            {
                if (!storage.has_val)
                {
                    unexpected<error_type> tmp{std::move(storage.unex)};
                    storage.unex.~err_t();
                    try
                    {

                        new (&storage.val) val_t{std::forward<Args>(args)...};
                    }
                    catch (...)
                    {
                        storage.unex = std::move(tmp.value());
                        throw;
                    }
                    storage.has_val = true;
                }
                else
                {

                    new (&storage.val) val_t{std::forward<Args>(args)...};
                }
            }
            return storage.val;
        }
        template<typename U, class... Args, typename V = value_type>
        std::enable_if_t<!std::is_void_v<V>, V&> emplace(std::initializer_list<U> il, Args&&... args)
        {
            if constexpr (std::is_nothrow_constructible_v<value_type, Args...>)
            {
                if (!storage.has_val)
                {
                    storage.unex.~err_t();
                    storage.has_val = true;
                }

                new (&storage.val) val_t{il, std::forward<Args>(args)...};
            }
            else if constexpr (std::is_nothrow_move_constructible_v<value_type>)
            {
                auto temp = val_t{il, std::forward<Args>(args)...};
                if (!storage.has_val)
                {
                    storage.unex.~err_t();
                    storage.has_val = true;
                }
                storage.val = std::move(temp);
            }
            else
            {
                if (!storage.has_val)
                {
                    unexpected<error_type> tmp{std::move(storage.unex)};
                    storage.unex.~err_t();
                    try
                    {
                        new (&storage.val) val_t{il, std::forward<Args>(args)...};
                    }
                    catch (...)
                    {
                        storage.unex = std::move(tmp.value());
                        throw;
                    }
                    storage.has_val = true;
                }
                else
                {

                    new (&storage.val) val_t{il, std::forward<Args>(args)...};
                }
            }
            return storage.val;
        }

        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V const*> operator->() const
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::addressof(storage.val);
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V*> operator->()
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::addressof(storage.val);
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V const&> operator*() const&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return storage.val;
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V&> operator*() &
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return storage.val;
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V const&&> operator*() const&&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::move(storage.val);
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V&&> operator*() &&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::move(storage.val);
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V const&> value() const&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return storage.val;
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V&> value() &
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return storage.val;
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V const&&> value() const&&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::move(storage.val);
        }
        template<typename V = value_type>
        constexpr std::enable_if_t<!std::is_void_v<T>, V&&> value() &&
        {
            if (!storage.has_val)
            {
                throw bad_expected_access(error());
            }
            return std::move(storage.val);
        }

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }
        constexpr bool has_value() const noexcept
        {
            return storage.has_val;
        }

        constexpr error_type const& error() const&
        {
            return storage.unex;
        };
        constexpr error_type& error() &
        {
            return storage.unex;
        };
        constexpr error_type const&& error() const&&
        {
            return std::move(storage.unex);
        };
        constexpr error_type&& error() &&
        {
            return std::move(storage.unex);
        };

        template<typename U>
        constexpr value_type value_or(U&& rhs) const&
        {
            return storage.has_val ? storage.val : static_cast<value_type>(std::forward<U>(rhs));
        }
        template<typename U>
        constexpr value_type value_or(U&& rhs) &&
        {
            return storage.has_val ? std::move(storage.val) : static_cast<value_type>(std::forward<U>(rhs));
        }

        template<typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const& lhs, expected<T2, E2> const& rhs);
        template<typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const& lhs, expected<T2, E2> const& rhs);

        template<typename T1, typename E1, typename T2>
        friend constexpr bool operator==(expected<T1, E1> const& lhs, const T2& rhs);
        template<typename T1, typename E1, typename T2>
        friend constexpr bool operator==(const T2& lhs, expected<T1, E1> const& rhs);
        template<typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(expected<T1, E1> const& lhs, const T2& rhs);
        template<typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(const T2& lhs, expected<T1, E1> const& rhs);

        template<typename T1, typename E1, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const& lhs, unexpected<E2> const& rhs);
        template<typename T1, typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E2> const& lhs, expected<T1, E1> const& rhs);
        template<typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const& lhs, unexpected<E2> const& rhs);
        template<typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E2> const& lhs, expected<T1, E1> const& rhs);

        std::enable_if_t<std::is_void_v<T> || (std::is_move_constructible_v<T> || std::is_move_constructible_v<E>), void> swap(expected& rhs) noexcept(
            std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<E> &&
            std::is_nothrow_swappable_v<E>)
        {
            if (!rhs.storage.has_val)
            {
                if (storage.has_val)
                {
                    if constexpr (!std::is_void_v<T>)
                    {
                        using std::swap;
                        swap(storage.val, rhs.storage.val);
                    }
                }
                else
                {
                    rhs.storage.swap(*this);
                }
            }
            else
            {
                if (storage.has_val)
                {
                    if constexpr (std::is_void_v<T>)
                    {
                        storage.unex    = std::move(rhs.storage.unex);
                        storage.has_val = false;
                        rhs.storage.unex.~err_t();
                        rhs.storage.has_val = true;
                    }
                    else if constexpr (std::is_nothrow_move_constructible_v<E>)
                    {
                        err_t tmp = std::move(rhs.storage.unex);
                        rhs.storage.unex.~err_t();

                        try
                        {
                            rhs.storage.val = std::move(storage.val);
                            storage.val.~val_t();
                            storage.unex        = std::move(tmp);
                            storage.has_val     = false;
                            rhs.storage.has_val = true;
                        }
                        catch (...)
                        {
                            rhs.storage.unex = std::move(tmp);
                            tmp.~err_t();
                            throw;
                        }
                    }
                    else if constexpr (std::is_nothrow_move_constructible_v<T>)
                    {
                        val_t tmp = std::move(rhs.storage.val);
                        rhs.storage.val.~val_t();

                        try
                        {
                            rhs.storage.unex = std::move(storage.unex);
                            storage.unex.~err_t();
                            storage.val         = std::move(tmp);
                            storage.has_val     = false;
                            rhs.storage.has_val = true;
                        }
                        catch (...)
                        {
                            rhs.storage.val = std::move(tmp);
                            tmp.~val_t();
                            throw;
                        }
                    }
                    else if constexpr (std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
                    {
                        storage.val         = std::move(rhs.storage.val);
                        rhs.storage.unex    = std::move(storage.unex);
                        storage.has_val     = false;
                        rhs.storage.has_val = true;
                    }
                }
                else
                {
                    using std::swap;
                    swap(storage.unex, rhs.storage.unex);
                }
            }
        }

        template<typename T1, typename E1>
        friend std::enable_if_t<!((std::is_void_v<T1> || std::is_move_constructible_v<T1>) && std::is_swappable_v<T1> && std::is_move_constructible_v<E1> &&
                                  std::is_swappable_v<E1>),
                                void>
        swap(expected<T1, E1>& x, expected<T1, E1>& y) noexcept(noexcept(x.swap(y)));
    };

    template<typename E>
    struct unexpected
    {
    public:
        using value_type = E;

        constexpr unexpected(unexpected const&) = default;
        constexpr unexpected(unexpected&&)      = default;
        template<class... Args>
        constexpr explicit unexpected(in_place_t, Args&&... args)
        {
            val = value_type(std::forward<Args>(args)...);
        }
        template<typename U, class... Args>
        constexpr explicit unexpected(in_place_t, std::initializer_list<U> il, Args&&... args)
        {
            val = value_type(il, std::forward<Args>(args)...);
        }
        template<typename Err = value_type>
        constexpr explicit unexpected(Err&& rhs)
        {
            val = value_type(std::forward<Err>(rhs));
        }
        template<typename Err>
        constexpr explicit unexpected(unexpected<Err> const& rhs)
        {
            val = rhs.val;
        }
        template<typename Err>
        constexpr explicit unexpected(unexpected<Err>&& rhs)
        {
            val = std::move(rhs.val);
            rhs.val.~Err();
        }

        constexpr unexpected& operator=(unexpected const&) = default;
        constexpr unexpected& operator=(unexpected&&)      = default;
        template<typename Err = value_type>
        constexpr unexpected& operator=(unexpected<Err> const&);
        template<typename Err = value_type>
        constexpr unexpected& operator=(unexpected<Err>&&);

        constexpr value_type const& error() const& noexcept
        {
            return val;
        }
        constexpr value_type& error() & noexcept
        {
            return val;
        }
        constexpr value_type const&& error() const&& noexcept
        {
            return std::move(val);
        }
        constexpr value_type&& error() && noexcept
        {
            return std::move(val);
        }

        template<typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const& lhs, unexpected<E2> const& rhs);
        template<typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const& lhs, unexpected<E2> const& rhs);

        std::enable_if_t<std::is_swappable_v<E>, void> swap(unexpected& rhs) noexcept(std::is_nothrow_swappable_v<E>)
        {
            using std::swap;
            swap(val, rhs.val);
        }

        template<typename E1>
        friend std::enable_if_t<std::is_swappable_v<E1>, void> swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)));

    private:
        value_type val;
    };

    template<>
    struct bad_expected_access<void> : public std::exception
    {
    public:
        explicit bad_expected_access() = default;
    };

    template<typename E>
    struct bad_expected_access : public bad_expected_access<void>
    {
    public:
        explicit bad_expected_access(E val)
            : val(val) {};

        virtual char const* what() const noexcept override
        {
            return "bad_expected_access";
        }

        E& error() &
        {
            return val;
        }
        E const& error() const&
        {
            return val;
        }
        E&& error() &&
        {
            return std::move(val);
        }
        E const&& error() const&&
        {
            return std::move(val);
        }

    private:
        E val;
    };

    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2, typename E2>
    constexpr bool operator==(expected<T1, E1> const& lhs, expected<T2, E2> const& rhs)
    {
        if (lhs.storage.has_val != rhs.storage.has_val)
        {
            return false;
        }
        if (lhs.storage.has_val)
            return lhs.storage.val == rhs.storage.val;
        else
            return lhs.storage.unex == rhs.storage.unex;
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2, typename E2>
    constexpr bool operator!=(expected<T1, E1> const& lhs, expected<T2, E2> const& rhs)
    {
        return !(lhs == rhs);
    }

    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2>
    constexpr bool operator==(expected<T1, E1> const& lhs, const T2& rhs)
    {
        if (!lhs.storage.has_val)
        {
            return false;
        }
        return lhs.storage.val == rhs;
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2>
    constexpr bool operator==(const T2& lhs, expected<T1, E1> const& rhs)
    {
        return rhs == lhs;
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2>
    constexpr bool operator!=(expected<T1, E1> const& lhs, const T2& rhs)
    {
        return !(lhs == rhs);
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename T2>
    constexpr bool operator!=(const T2& lhs, expected<T1, E1> const& rhs)
    {
        return !(lhs == rhs);
    }

    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename E2>
    constexpr bool operator==(expected<T1, E1> const& lhs, unexpected<E2> const& rhs)
    {
        if (lhs.storage.has_val)
        {
            return false;
        }
        return lhs.storage.val == rhs.val;
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename E2>
    constexpr bool operator==(unexpected<E2> const& lhs, expected<T1, E1> const& rhs)
    {
        return rhs == lhs;
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename E2>
    constexpr bool operator!=(expected<T1, E1> const& lhs, unexpected<E2> const& rhs)
    {
        return !(lhs == rhs);
    }
    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1, typename E2>
    constexpr bool operator!=(unexpected<E2> const& lhs, expected<T1, E1> const& rhs)
    {
        return !(lhs == rhs);
    }

    LJH_MODULE_MAIN_EXPORT template<typename T1, typename E1>
    std::enable_if_t<!((std::is_void_v<T1> || std::is_move_constructible_v<T1>) && std::is_swappable_v<T1> && std::is_move_constructible_v<E1> &&
                       std::is_swappable_v<E1>),
                     void>
    swap(expected<T1, E1>& x, expected<T1, E1>& y) noexcept(noexcept(x.swap(y)))
    {
        x.swap(y);
    }

    LJH_MODULE_MAIN_EXPORT template<typename E1, typename E2>
    constexpr bool operator==(unexpected<E1> const& lhs, unexpected<E2> const& rhs)
    {
        return lhs.val == rhs.val;
    }

    LJH_MODULE_MAIN_EXPORT template<typename E1, typename E2>
    constexpr bool operator!=(unexpected<E1> const& lhs, unexpected<E2> const& rhs)
    {
        return !(lhs == rhs);
    }

    LJH_MODULE_MAIN_EXPORT template<typename E1>
    std::enable_if_t<std::is_swappable_v<E1>, void> swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)))
    {
        x.swap(y);
    }
} // namespace ljh