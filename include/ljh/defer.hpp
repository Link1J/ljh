
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// defer.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include <utility>
#include <exception>

namespace ljh
{
    namespace __
    {
        template<typename D, typename T>
        struct defer_base
        {
            static constexpr bool _nothrow_constructible = std::is_nothrow_constructible<T, T>::value;

            ~defer_base() noexcept(noexcept(f()))
            {
                if (static_cast<D&>(*this)._run())
                    f();
            }

            defer_base(defer_base const&)            = delete;
            defer_base& operator=(defer_base const&) = delete;
            defer_base(defer_base&&)                 = default;
            defer_base& operator=(defer_base&&)      = default;

        protected:
            defer_base(T&& f) noexcept(_nothrow_constructible)
                : f(std::forward<T>(f))
            {}

        private:
            T f;
        };

        template<typename D, typename T>
        struct condition_defer_base : defer_base<D, T>
        {
            condition_defer_base(condition_defer_base&&)            = default;
            condition_defer_base& operator=(condition_defer_base&&) = default;

            void make_active() noexcept
            {
                run = true;
            }

            void make_inactive() noexcept
            {
                run = false;
            }

            void toggle() noexcept
            {
                run = !run;
            }

        protected:
            using defer = __::defer_base<D, T>;

            condition_defer_base(bool default_state, T&& f) noexcept(defer::_nothrow_constructible)
                : defer(std::forward<T>(f))
                , run(default_state)
            {}

            bool run;
        };

        inline bool in_exception() noexcept
        {
#if __cpp_lib_uncaught_exceptions >= 201411L
            return std::uncaught_exceptions() > 0;
#else
            return std::uncaught_exception();
#endif
        }
    } // namespace __
} // namespace ljh

namespace ljh
{
    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct scope_exit : __::defer_base<scope_exit<T>, T>
    {
        using _base = __::defer_base<scope_exit<T>, T>;

        scope_exit(T&& f) noexcept(_base::_nothrow_constructible)
            : _base(std::forward<T>(f))
        {}

        static constexpr bool _run() noexcept
        {
            return true;
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct scope_fail : __::defer_base<scope_fail<T>, T>
    {
        using _base = __::defer_base<scope_fail<T>, T>;

        scope_fail(T&& f) noexcept(_base::_nothrow_constructible)
            : _base(std::forward<T>(f))
        {}

        static bool _run() noexcept
        {
            return __::in_exception();
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct scope_success : __::defer_base<scope_success<T>, T>
    {
        using _base = __::defer_base<scope_success<T>, T>;

        scope_success(T&& f) noexcept(_base::_nothrow_constructible)
            : _base(std::forward<T>(f))
        {}

        static bool _run() noexcept
        {
            return !__::in_exception();
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct condition_scope_exit : __::condition_defer_base<condition_scope_exit<T>, T>
    {
        using _base = __::condition_defer_base<condition_scope_exit, T>;

        condition_scope_exit(bool default_state, T&& f) noexcept(_base::defer::_nothrow_constructible)
            : _base(default_state, std::forward<T>(f))
        {}

        bool _run() const noexcept
        {
            return _base::run;
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct condition_scope_fail : __::condition_defer_base<condition_scope_fail<T>, T>
    {
        using _base = __::condition_defer_base<condition_scope_fail, T>;

        condition_scope_fail(bool default_state, T&& f) noexcept(_base::defer::_nothrow_constructible)
            : _base(default_state, std::forward<T>(f))
        {}

        bool _run() const noexcept
        {
            return _base::run && __::in_exception();
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    struct condition_scope_success : __::condition_defer_base<condition_scope_success<T>, T>
    {
        using _base = __::condition_defer_base<condition_scope_success, T>;

        condition_scope_success(bool default_state, T&& f) noexcept(_base::defer::_nothrow_constructible)
            : _base(default_state, std::forward<T>(f))
        {}

        bool _run() const noexcept
        {
            return _base::run && !__::in_exception();
        }
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    scope_exit<T> make_scope_exit(T&& f) noexcept(scope_exit<T>::_nothrow_constructible)
    {
        return scope_exit<T>{std::forward<T>(f)};
    }

    LJH_MODULE_MAIN_EXPORT template<typename T>
    scope_fail<T> make_scope_fail(T&& f) noexcept(scope_fail<T>::_nothrow_constructible)
    {
        return scope_fail<T>{std::forward<T>(f)};
    }

    LJH_MODULE_MAIN_EXPORT template<typename T>
    scope_success<T> make_scope_success(T&& f) noexcept(scope_success<T>::_nothrow_constructible)
    {
        return scope_success<T>{std::forward<T>(f)};
    }

    LJH_MODULE_MAIN_EXPORT template<typename T>
    condition_scope_exit<T> make_condition_scope_exit(bool default_state, T&& f) noexcept(condition_scope_exit<T>::_nothrow_constructible)
    {
        return condition_scope_exit<T>{default_state, std::forward<T>(f)};
    }

    LJH_MODULE_MAIN_EXPORT template<typename T>
    condition_scope_fail<T> make_condition_scope_fail(bool default_state, T&& f) noexcept(condition_scope_fail<T>::_nothrow_constructible)
    {
        return condition_scope_fail<T>{default_state, std::forward<T>(f)};
    }

    LJH_MODULE_MAIN_EXPORT template<typename T>
    condition_scope_success<T> make_condition_scope_success(bool default_state, T&& f) noexcept(condition_scope_success<T>::_nothrow_constructible)
    {
        return condition_scope_success<T>{default_state, std::forward<T>(f)};
    }
} // namespace ljh