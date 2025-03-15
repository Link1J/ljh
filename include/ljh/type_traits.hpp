
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// type_traits.hpp - v1.5
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
// Requires cpp_version.hpp
//
// Based on code from https://stackoverflow.com/a/10724828/13155694
// Based on code from https://stackoverflow.com/a/15396757/7932993
// Based on code from https://devblogs.microsoft.com/oldnewthing/20190711-00/?p=102682
// Based on code from https://devblogs.microsoft.com/oldnewthing/20190712-00/?p=102690
//
// ABOUT
//     Adds extra type traits
//
// USAGE
//
// Version History
//     1.0 Inital Version
//     1.1 Add is_char and rename to type_traits.hpp
//     1.2 Change is_char to is_char_type
//     1.3 Add is_callable and is_function_pointer
//     1.4 Add is_type_complete
//     1.5 Add type_if_defined

#pragma once

#include "cpp_version.hpp"
#include <type_traits>

#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
#include <string_view>
#endif

namespace ljh
{
    template<typename T, bool = std::is_enum<T>::value>
    struct is_scoped_enum : std::false_type
    {};

    template<typename T>
    struct is_scoped_enum<T, true> : std::integral_constant<bool, !std::is_convertible<T, typename std::underlying_type<T>::type>::value>
    {};

    template<typename T>
    struct is_char_type : std::false_type
    {};

    template<>
    struct is_char_type<char> : std::true_type
    {};
    template<>
    struct is_char_type<signed char> : std::true_type
    {};
    template<>
    struct is_char_type<unsigned char> : std::true_type
    {};
    template<>
    struct is_char_type<wchar_t> : std::true_type
    {};
#if defined(__cpp_char8_t)
    template<>
    struct is_char_type<char8_t> : std::true_type
    {};
#endif
    template<>
    struct is_char_type<char16_t> : std::true_type
    {};
    template<>
    struct is_char_type<char32_t> : std::true_type
    {};

    template<typename T>
    struct is_callable
    {
    private:
        typedef char (&yes)[1];
        typedef char (&no)[2];

        struct Fallback
        {
            void operator()();
        };
        struct Derived : T, Fallback
        {};

        template<typename U, U>
        struct Check;

        template<typename>
        static yes test(...);

        template<typename C>
        static no test(Check<void (Fallback::*)(), &C::operator()>*);

    public:
        static bool const value = sizeof(test<Derived>(0)) == sizeof(yes);
    };

    template<typename T>
    struct is_function_pointer : std::integral_constant<bool, std::is_pointer<T>::value && std::is_function<typename std::remove_pointer<T>::type>::value>
    {};

    template<typename, template<typename...> class>
    struct _is_instance_impl : public std::false_type
    {};

    template<template<typename...> class U, typename... Ts>
    struct _is_instance_impl<U<Ts...>, U> : public std::true_type
    {};

    template<typename T, template<typename...> class U>
    using is_instance = _is_instance_impl<std::decay_t<T>, U>;

    template<typename, typename = void>
    struct is_type_complete : std::false_type
    {};

    template<typename...>
    using void_t = void;

    template<typename T>
    struct is_type_complete<T, void_t<decltype(sizeof(T))>> : public std::true_type
    {};

    struct empty
    {};

    template<typename T, typename = void>
    struct type_or_empty
    {
        using type = empty;
    };

    template<typename T>
    struct type_or_empty<T, void_t<decltype(sizeof(T))>>
    {
        using type = T;
    };

    template<typename T>
    using type_if_defined = typename type_or_empty<T>::type;

#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    template<typename T>
    inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

    template<typename T>
    inline constexpr bool is_char_type_v = is_char_type<T>::value;

    template<typename T>
    inline constexpr bool is_callable_v = is_callable<T>::value;

    template<typename T>
    inline constexpr bool is_function_pointer_v = is_function_pointer<T>::value;

    template<typename T, template<typename...> typename U>
    inline constexpr bool is_instance_v = is_instance<T, U>::value;

    template<typename T>
    inline constexpr bool is_type_complete_v = is_type_complete<T>::value;
#endif

#if __cpp_lib_remove_cvref >= 201711L
    template<typename T>
    using remove_cvref = std::remove_cvref<T>;
    template<typename T>
    using remove_cvref_t = std::remove_cvref_t<T>;
#else
    template<typename T>
    struct remove_cvref
    {
        using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    };

#if LJH_CPP_VERSION >= LJH_CPP14_VERSION
    template<typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;
#endif
#endif

#if __cpp_lib_bounded_array_traits >= 201902L
    template<typename T>
    using is_bounded_array = std::is_bounded_array<T>;
    template<typename T>
    inline constexpr bool is_bounded_array_v = std::is_bounded_array_v<T>;
#else
    template<typename T>
    struct is_bounded_array : std::false_type
    {};
    template<typename T, std::size_t N>
    struct is_bounded_array<T[N]> : std::true_type
    {};

#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
    template<typename T>
    inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;
#endif
#endif

#if __cpp_lib_type_identity >= 201806L
    template<typename T>
    using type_identity = std::type_identity<T>;
    template<typename T>
    using type_identity_t = std::type_identity_t<T>;
#else
    template<typename T>
    struct type_identity
    {
        using type = T;
    };

    template<typename T>
    using type_identity_t = typename type_identity<T>::type;
#endif

    template<typename T, typename U>
    struct make_like
    {
        using type = U;
    };

    template<typename T, typename U>
    struct make_like<T const, U>
    {
        using type = typename std::add_const<U>::type;
    };

    template<typename T, typename U>
    struct make_like<T&, U>
    {
        using type = typename std::add_lvalue_reference<typename make_like<typename std::remove_reference<T>::type, U>::type>::type;
    };

    template<typename T, typename U>
    struct make_like<T&&, U>
    {
        using type = typename std::add_rvalue_reference<typename make_like<typename std::remove_reference<T>::type, U>::type>::type;
    };

#if LJH_CPP_VERSION >= LJH_CPP14_VERSION
    template<typename T, typename U>
    using make_like_t = typename make_like<T, U>::type;
#endif
} // namespace ljh