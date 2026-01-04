
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// compile_time_string.hpp - v1.1
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
// Requires MVSC 19.23 or GCC 10 or clang 10
//
// ABOUT
//     Adds some basic concepts
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#if LJH_CPP_VERSION < LJH_CPP17_VERSION || !(__cpp_concepts > 0)
#error "C++20 concepts are needed for this header"
#endif

#include <concepts>
#include <type_traits>
#include <utility>
#include <tuple>
#include "type_traits.hpp"

namespace ljh
{
    LJH_MODULE_MAIN_EXPORT template<typename T, typename... Args>
    concept one_of = (... || std::same_as<T, Args>);

    LJH_MODULE_MAIN_EXPORT template<typename T>
    concept char_type = one_of<T, char, signed char, unsigned char, wchar_t,
#if __cpp_char8_t >= 201811L
                               char8_t,
#endif
                               char16_t, char32_t>;

    LJH_MODULE_MAIN_EXPORT template<typename T>
    concept scoped_enum = is_scoped_enum_v<T>;

    LJH_MODULE_MAIN_EXPORT template<typename T>
    concept function_type = std::is_function_v<T>;

    LJH_MODULE_MAIN_EXPORT template<typename L, typename R, typename T>
    concept either_same_as = std::same_as<L, T> || std::same_as<R, T>;

    LJH_MODULE_MAIN_EXPORT template<typename T, template<typename...> typename U>
    concept instance = ljh::is_instance_v<T, U>;

    LJH_MODULE_MAIN_EXPORT template<typename T, std::size_t N>
    concept has_tuple_element = requires(T t) {
        typename std::tuple_element_t<N, std::remove_const_t<T>>;
        { get<N>(t) } -> std::convertible_to<std::tuple_element_t<N, T> const&>;
    };

    LJH_MODULE_MAIN_EXPORT template<typename T>
    concept tuple_like = !std::is_reference_v<T> && requires(T t) {
        typename std::tuple_size<T>::type;
        requires std::derived_from<std::tuple_size<T>, std::integral_constant<std::size_t, std::tuple_size_v<T>>>;
    } && []<std::size_t... N>(std::index_sequence<N...>) { return (has_tuple_element<T, N> && ...); }(std::make_index_sequence<std::tuple_size_v<T>>());

    LJH_MODULE_MAIN_EXPORT template<typename T>
    concept pair_like = tuple_like<T> && std::tuple_size_v<std::remove_cvref_t<T>> == 2;
} // namespace ljh