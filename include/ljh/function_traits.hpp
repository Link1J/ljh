
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// function_traits.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
//
// ABOUT
//     Adds function traits
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include <type_traits>
#include <tuple>
#include <functional>
#include "cpp_version.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#if defined(__clang__)
#define LJH_CALLING_CONVENTION_cdecl __attribute__((cdecl))
#define LJH_CALLING_CONVENTION_stdcall __attribute__((stdcall))
#define LJH_CALLING_CONVENTION_fastcall __attribute__((fastcall))
#define LJH_CALLING_CONVENTION_vectorcall __attribute__((vectorcall))
#define LJH_CALLING_CONVENTION_ms_abi __attribute__((ms_abi))
#define LJH_CALLING_CONVENTION_sysv_abi __attribute__((sysv_abi))
#define LJH_CALLING_CONVENTION_regcall __attribute__((regcall))
#define LJH_CALLING_CONVENTION_thiscall __attribute__((thiscall))
#elif defined(__INTEL_COMPILER)
#define LJH_CALLING_CONVENTION_cdecl __cdecl
#define LJH_CALLING_CONVENTION_stdcall __stdcall
#define LJH_CALLING_CONVENTION_fastcall __fastcall
#define LJH_CALLING_CONVENTION_vectorcall __vectorcall
#define LJH_CALLING_CONVENTION_ms_abi __cdecl
#define LJH_CALLING_CONVENTION_sysv_abi __cdecl
#define LJH_CALLING_CONVENTION_regcall __regcall
#define LJH_CALLING_CONVENTION_thiscall __thiscall
#elif defined(__GNUC__)
#define LJH_CALLING_CONVENTION_cdecl __attribute__((cdecl))
#define LJH_CALLING_CONVENTION_stdcall __attribute__((stdcall))
#define LJH_CALLING_CONVENTION_fastcall __attribute__((fastcall))
#define LJH_CALLING_CONVENTION_vectorcall __attribute__((vectorcall))
#define LJH_CALLING_CONVENTION_ms_abi __attribute__((ms_abi))
#define LJH_CALLING_CONVENTION_sysv_abi __attribute__((sysv_abi))
#define LJH_CALLING_CONVENTION_regcall __attribute__((regcall))
#define LJH_CALLING_CONVENTION_thiscall __attribute__((thiscall))
#elif defined(_MSC_VER)
#define LJH_CALLING_CONVENTION_cdecl __cdecl
#define LJH_CALLING_CONVENTION_stdcall __stdcall
#define LJH_CALLING_CONVENTION_fastcall __fastcall
#if defined(_M_ARM64EC)
#define LJH_CALLING_CONVENTION_vectorcall __cdecl
#else
#define LJH_CALLING_CONVENTION_vectorcall __vectorcall
#endif
#define LJH_CALLING_CONVENTION_ms_abi __cdecl
#define LJH_CALLING_CONVENTION_sysv_abi __cdecl
#define LJH_CALLING_CONVENTION_regcall __cdecl
#define LJH_CALLING_CONVENTION_thiscall __thiscall
#else
#define LJH_CALLING_CONVENTION_cdecl
#define LJH_CALLING_CONVENTION_stdcall
#define LJH_CALLING_CONVENTION_fastcall
#define LJH_CALLING_CONVENTION_vectorcall
#define LJH_CALLING_CONVENTION_ms_abi
#define LJH_CALLING_CONVENTION_sysv_abi
#define LJH_CALLING_CONVENTION_regcall
#define LJH_CALLING_CONVENTION_thiscall
#endif

#if defined(cdecl)
#undef cdecl
#endif

namespace ljh
{
    namespace calling_conventions
    {
        namespace cdecl_
        {
            using type          = void(LJH_CALLING_CONVENTION_cdecl*)();
            inline int const id = 0;
        } // namespace cdecl_
        namespace stdcall_
        {
            using type          = void(LJH_CALLING_CONVENTION_stdcall*)();
            inline int const id = 1;
        } // namespace stdcall_
        namespace fastcall_
        {
            using type          = void(LJH_CALLING_CONVENTION_fastcall*)();
            inline int const id = 2;
        } // namespace fastcall_
        namespace vectorcall_
        {
            using type          = void(LJH_CALLING_CONVENTION_vectorcall*)();
            inline int const id = 3;
        } // namespace vectorcall_
        namespace ms_abi_
        {
            using type          = void(LJH_CALLING_CONVENTION_ms_abi*)();
            inline int const id = 4;
        } // namespace ms_abi_
        namespace sysv_abi_
        {
            using type          = void(LJH_CALLING_CONVENTION_sysv_abi*)();
            inline int const id = 5;
        } // namespace sysv_abi_
        namespace regcall_
        {
            using type          = void(LJH_CALLING_CONVENTION_regcall*)();
            inline int const id = 6;
        } // namespace regcall_
        namespace thiscall_
        {
            using type          = void(LJH_CALLING_CONVENTION_thiscall*)();
            inline int const id = 7;
        } // namespace thiscall_
    }; // namespace calling_conventions

    template<typename R, typename... Args>
    struct _function_traits_impl
    {
        using return_type                           = R;
        using argument_types                        = std::tuple<Args...>;
        static constexpr std::size_t argument_count = sizeof...(Args);
        template<std::size_t N>
        using argument_type = typename std::tuple_element<N, argument_types>::type;
    };

    LJH_MODULE_MAIN_EXPORT template<typename F, int = 0>
    struct function_traits;

#define TRAIT_INTERALS(FUNCTION, CC, Noexcept, ArgList)                                                                                                        \
    template<typename R, typename... Args>                                                                                                                     \
    struct function_traits<FUNCTION,                                                                                                                           \
                           std::is_same<calling_conventions::cdecl_::type, calling_conventions::CC##_::type>::value ? calling_conventions::CC##_::id : 0>      \
        : _function_traits_impl<R, Args...>                                                                                                                    \
    {                                                                                                                                                          \
        struct as                                                                                                                                              \
        {                                                                                                                                                      \
            using function         = R LJH_CALLING_CONVENTION_##CC ArgList LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept);                                               \
            using function_pointer = R(LJH_CALLING_CONVENTION_##CC*) ArgList LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept);                                             \
            using std_function     = std::function<function>;                                                                                                  \
        };                                                                                                                                                     \
        struct is                                                                                                                                              \
        {                                                                                                                                                      \
            constexpr static bool no_exceptions    = Noexcept;                                                                                                 \
            constexpr static bool variadic         = !std::is_same<void(*) ArgList, void (*)(Args...)>::value;                                                 \
            constexpr static bool function         = std::is_same<FUNCTION, typename as::function>::value;                                                     \
            constexpr static bool function_pointer = std::is_same<FUNCTION, typename as::function_pointer>::value;                                             \
            constexpr static bool std_function     = std::is_same<FUNCTION, typename as::std_function>::value;                                                 \
            constexpr static bool member_function  = false;                                                                                                    \
            constexpr static bool constant         = false;                                                                                                    \
        };                                                                                                                                                     \
        constexpr static int calling_convention = calling_conventions::CC##_::id;                                                                              \
    }

#define TRAIT_INTERALS_MEMBER(FUNCTION, CC, Noexcept, ArgList)                                                                                                 \
    template<typename R, typename M, typename... Args>                                                                                                         \
    struct function_traits<                                                                                                                                    \
        FUNCTION, std::is_same<void(LJH_CALLING_CONVENTION_cdecl*)(), void(LJH_CALLING_CONVENTION_##CC*)()>::value ? calling_conventions::CC##_::id : 0>       \
        : _function_traits_impl<R, Args...>                                                                                                                    \
    {                                                                                                                                                          \
        struct as                                                                                                                                              \
        {                                                                                                                                                      \
            using function_pointer = R(LJH_CALLING_CONVENTION_##CC M::*) ArgList LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept);                                         \
            using std_function     = std::function<function_pointer>;                                                                                          \
        };                                                                                                                                                     \
        struct is                                                                                                                                              \
        {                                                                                                                                                      \
            constexpr static bool no_exceptions    = Noexcept;                                                                                                 \
            constexpr static bool variadic         = !std::is_same<void(*) ArgList, void (*)(Args...)>::value;                                                 \
            constexpr static bool function         = false;                                                                                                    \
            constexpr static bool function_pointer = std::is_same<FUNCTION, typename as::function_pointer>::value;                                             \
            constexpr static bool std_function     = std::is_same<FUNCTION, typename as::std_function>::value;                                                 \
            constexpr static bool member_function  = true;                                                                                                     \
            constexpr static bool constant         = false;                                                                                                    \
        };                                                                                                                                                     \
        constexpr static int calling_convention = calling_conventions::CC##_::id;                                                                              \
    }

#define TRAIT_INTERALS_MEMBER_CONST(FUNCTION, CC, Noexcept, ArgList)                                                                                           \
    template<typename R, typename M, typename... Args>                                                                                                         \
    struct function_traits<                                                                                                                                    \
        FUNCTION, std::is_same<void(LJH_CALLING_CONVENTION_cdecl*)(), void(LJH_CALLING_CONVENTION_##CC*)()>::value ? calling_conventions::CC##_::id : 0>       \
        : _function_traits_impl<R, Args...>                                                                                                                    \
    {                                                                                                                                                          \
        struct as                                                                                                                                              \
        {                                                                                                                                                      \
            using function_pointer = R(LJH_CALLING_CONVENTION_##CC M::*) ArgList const LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept);                                   \
            using std_function     = std::function<function_pointer>;                                                                                          \
        };                                                                                                                                                     \
        struct is                                                                                                                                              \
        {                                                                                                                                                      \
            constexpr static bool no_exceptions    = Noexcept;                                                                                                 \
            constexpr static bool variadic         = !std::is_same<void(*) ArgList, void (*)(Args...)>::value;                                                 \
            constexpr static bool function         = false;                                                                                                    \
            constexpr static bool function_pointer = std::is_same<FUNCTION, typename as::function_pointer>::value;                                             \
            constexpr static bool std_function     = std::is_same<FUNCTION, typename as::std_function>::value;                                                 \
            constexpr static bool member_function  = true;                                                                                                     \
            constexpr static bool constant         = true;                                                                                                     \
        };                                                                                                                                                     \
        constexpr static int calling_convention = calling_conventions::CC##_::id;                                                                              \
    }

#define MAKE_ALL_TRAITS(CC, Noexcept, ArgList)                                                                                                                 \
    TRAIT_INTERALS(R(LJH_CALLING_CONVENTION_##CC*) ArgList noexcept(Noexcept), CC, Noexcept, ArgList);                                                         \
    TRAIT_INTERALS(R LJH_CALLING_CONVENTION_##CC ArgList noexcept(Noexcept), CC, Noexcept, ArgList);                                                           \
    TRAIT_INTERALS(std::function<R LJH_CALLING_CONVENTION_##CC ArgList noexcept(Noexcept)>, CC, Noexcept, ArgList);                                            \
    TRAIT_INTERALS_MEMBER(R(LJH_CALLING_CONVENTION_##CC M::*) ArgList noexcept(Noexcept), CC, Noexcept, ArgList);                                              \
    TRAIT_INTERALS_MEMBER_CONST(R(LJH_CALLING_CONVENTION_##CC M::*) ArgList const noexcept(Noexcept), CC, Noexcept, ArgList);                                  \
    TRAIT_INTERALS_MEMBER(std::function<R(LJH_CALLING_CONVENTION_##CC M::*) ArgList noexcept(Noexcept)>, CC, Noexcept, ArgList);                               \
    TRAIT_INTERALS_MEMBER_CONST(std::function<R(LJH_CALLING_CONVENTION_##CC M::*) ArgList const noexcept(Noexcept)>, CC, Noexcept, ArgList)

#define MAKE_ALL_TRAITS_M(CC, Noexcept, ArgList)                                                                                                               \
    TRAIT_INTERALS_MEMBER(R(LJH_CALLING_CONVENTION_##CC M::*) ArgList noexcept(Noexcept), CC, Noexcept, ArgList);                                              \
    TRAIT_INTERALS_MEMBER_CONST(R(LJH_CALLING_CONVENTION_##CC M::*) ArgList const noexcept(Noexcept), CC, Noexcept, ArgList);                                  \
    TRAIT_INTERALS_MEMBER(std::function<R(LJH_CALLING_CONVENTION_##CC M::*) ArgList noexcept(Noexcept)>, CC, Noexcept, ArgList);                               \
    TRAIT_INTERALS_MEMBER_CONST(std::function<R(LJH_CALLING_CONVENTION_##CC M::*) ArgList const noexcept(Noexcept)>, CC, Noexcept, ArgList)

#if __cpp_noexcept_function_type >= 201510L
#define MAKE_TRAITS(CC, ArgList)                                                                                                                               \
    MAKE_ALL_TRAITS(CC, false, ArgList);                                                                                                                       \
    MAKE_ALL_TRAITS(CC, true, ArgList)
#else
#define MAKE_TRAITS(CC, ArgList) MAKE_ALL_TRAITS(CC, false, ArgList)
#endif
#if __cpp_noexcept_function_type >= 201510L
#define MAKE_TRAITS_M(CC, ArgList)                                                                                                                             \
    MAKE_ALL_TRAITS_M(CC, false, ArgList);                                                                                                                     \
    MAKE_ALL_TRAITS_M(CC, true, ArgList)
#else
#define MAKE_TRAITS_M(CC, ArgList) MAKE_ALL_TRAITS_M(CC, false, ArgList)
#endif

    MAKE_TRAITS(cdecl, (Args...));
    MAKE_TRAITS(cdecl, (Args..., ...));
    MAKE_TRAITS(stdcall, (Args...));
    //	MAKE_TRAITS  (stdcall   , (Args..., ...));
    MAKE_TRAITS(fastcall, (Args...));
    //	MAKE_TRAITS  (fastcall  , (Args..., ...));
    MAKE_TRAITS(vectorcall, (Args...));
    //	MAKE_TRAITS  (vectorcall, (Args..., ...));
    MAKE_TRAITS(regcall, (Args...));
    //	MAKE_TRAITS  (regcall   , (Args..., ...));
    //	MAKE_TRAITS  (ms_abi    , (Args...)     );
    //	MAKE_TRAITS  (ms_abi    , (Args..., ...));
    MAKE_TRAITS(sysv_abi, (Args...));
    MAKE_TRAITS(sysv_abi, (Args..., ...));
    MAKE_TRAITS_M(thiscall, (Args...));
    //	MAKE_TRAITS_M(thiscall  , (Args..., ...));

#undef MAKE_TRAITS
#undef MAKE_ALL_TRAITS
#undef TRAIT_INTERALS
#undef TRAIT_INTERALS_MEMBER

#if __cpp_concepts >= 201907L && __cpp_lib_concepts >= 202002L
    template<typename T, auto F>
    concept matches_return_type = std::same_as<T, typename ljh::function_traits<decltype(F)>::return_type>;
#elif LJH_CPP_VERSION > LJH_CPP17_VERSION
    template<typename T, auto F>
    inline constexpr bool matches_return_type = std::is_same_v<T, typename ljh::function_traits<decltype(F)>::return_type>;
#endif
} // namespace ljh

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
