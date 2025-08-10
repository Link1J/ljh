
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// checked_math.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include <cassert>
#include <concepts>
#include <compare>
#include <limits>

#if __has_include(<format>)
#include <format>
#endif

#if defined(LJH_COMPILER_MSVC)
#include <intrin.h>
#endif

#ifndef LJH__fastfail
#if defined(LJH_COMPILER_MSVC)
#define LJH__fastfail __fastfail
#elif __has_builtin(__builtin_arm_trap)
#define LJH__fastfail(...) __builtin_arm_trap(__VA_ARGS__)
#elif __has_builtin(__builtin_trap)
#define LJH__fastfail(...) __builtin_trap()
#else
#error "No fastfail intrisic"
#endif
#endif

#define LJH_CHECKED_MATH(...)                                                                                                                                  \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (__VA_ARGS__) [[unlikely]]                                                                                                                          \
            LJH__fastfail(8 /* FAST_FAIL_RANGE_CHECK_FAILURE */);                                                                                              \
    }                                                                                                                                                          \
    while (false)

#define CHECK_USE_BUILTIN 1
#define CHECK_USE_X86INTRIN 2
#define CHECK_USE_EXTERN_ASM 0

#if __has_builtin(__builtin_add_overflow)
#define CHECK_USE CHECK_USE_BUILTIN
#elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC))
#define CHECK_USE CHECK_USE_EXTERN_ASM
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#define CHECK_USE CHECK_USE_X86INTRIN
#endif

namespace ljh::ckd
{
#if CHECK_USE == CHECK_USE_BUILTIN
    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool add(T& res, U lhs, V rhs) noexcept
    {
        return __builtin_add_overflow(lhs, rhs, __builtin_addressof(res));
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool sub(T& res, U lhs, V rhs) noexcept
    {
        return __builtin_sub_overflow(lhs, rhs, __builtin_addressof(res));
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool mul(T& res, U lhs, V rhs) noexcept
    {
        return __builtin_mul_overflow(lhs, rhs, __builtin_addressof(res));
    }
#elif CHECK_USE == CHECK_USE_X86INTRIN
#if defined(_M_IX86)
    bool __forceinline _add_overflow_i64(unsigned char, signed __int64 lhs, signed __int64 rhs, signed __int64* res) noexcept
    {
        __asm
        {
            mov     eax, DWORD PTR lhs[0]
            mov     edx, DWORD PTR lhs[4]
            add     eax, DWORD PTR rhs[0]
            adc     edx, DWORD PTR rhs[4]
            mov     ecx, DWORD PTR res
            mov     DWORD PTR [ecx], eax
            mov     DWORD PTR [ecx+4], edx
            seto    al
        }
    }
    bool __forceinline _addcarry_u64(unsigned char, unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64* res) noexcept
    {
        __asm
        {
            mov     eax, DWORD PTR lhs[0]
            mov     edx, DWORD PTR lhs[4]
            add     eax, DWORD PTR rhs[0]
            adc     edx, DWORD PTR rhs[4]
            mov     ecx, DWORD PTR res
            mov     DWORD PTR [ecx], eax
            mov     DWORD PTR [ecx+4], edx
            setc    al
        }
    }
    bool __forceinline _sub_overflow_i64(unsigned char, signed __int64 lhs, signed __int64 rhs, signed __int64* res) noexcept
    {
        __asm
        {
            mov     eax, DWORD PTR lhs[0]
            mov     edx, DWORD PTR lhs[4]
            sub     eax, DWORD PTR rhs[0]
            sbb     edx, DWORD PTR rhs[4]
            mov     ecx, DWORD PTR res
            mov     DWORD PTR [ecx], eax
            mov     DWORD PTR [ecx+4], edx
            seto    al
        }
    }
    bool __forceinline _subborrow_u64(unsigned char, unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64* res) noexcept
    {
        __asm
        {
            mov     ecx, DWORD PTR lhs[0]
            mov     ebx, DWORD PTR lhs[4]
            mov     eax, ecx
            sub     eax, DWORD PTR rhs[0]
            mov     edx, ebx
            sbb     edx, DWORD PTR rhs[4]
            cmp     ecx, eax
            sbb     ebx, edx
            mov     ecx, DWORD PTR res
            mov     DWORD PTR [ecx], eax
            mov     DWORD PTR [ecx+4], edx
            setc    al
        }
    }
    bool __forceinline _mul_full_overflow_i64(signed __int64 lhs, signed __int64 rhs, signed __int64* res, signed __int64*) noexcept
    {
        __asm
        {
            mov     eax, DWORD PTR lhs[0]
            mov     edx, DWORD PTR rhs[0]
            mov     ecx, DWORD PTR lhs[4]
            mov     esi, DWORD PTR rhs[4]
            imul    ecx, edx
            seto    bl
            imul    esi, eax
            seto    bh
            mul     edx
            add     ecx, esi
            add     edx, ecx
            mov     ecx, DWORD PTR res
            mov     DWORD PTR[ecx], eax
            mov     DWORD PTR[ecx + 4], edx
            or      bh, bl
            mov     al, bh
        }
    }
    bool __forceinline _mul_full_overflow_u64(unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64* res, unsigned __int64*) noexcept
    {
        __asm
        {
            mov     eax, DWORD PTR lhs[4]
            mul     DWORD PTR rhs[0]
            mov     ebx, eax
            mov     ecx, edx
            mov     eax, DWORD PTR lhs[0]
            mul     DWORD PTR rhs[4]
            add     ebx, eax
            or      ecx, edx
            setnz   cl
            mov     eax, DWORD PTR lhs[0]
            mul     DWORD PTR rhs[0]
            add     edx, ebx
            mov     ebx, DWORD PTR res
            mov     DWORD PTR[ebx], eax
            mov     DWORD PTR[ebx + 4], edx
            mov     al, cl
        }
    }
#endif
    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool add(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1 && std::is_signed_v<T>)
            return _add_overflow_i8(0, lhs, rhs, reinterpret_cast<signed char*>(__builtin_addressof(res))) != 0;
        else if constexpr (sizeof(T) == 1 && std::is_unsigned_v<T>)
            return _addcarry_u8(0, lhs, rhs, reinterpret_cast<unsigned char*>(__builtin_addressof(res))) != 0;
        else if constexpr (std::same_as<T, signed short>)
            return _add_overflow_i16(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned short>)
            return _addcarry_u16(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, signed int>)
            return _add_overflow_i32(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned int>)
            return _addcarry_u32(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, signed __int64>)
            return _add_overflow_i64(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned __int64>)
            return _addcarry_u64(0, lhs, rhs, __builtin_addressof(res));
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool sub(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1 && std::is_signed_v<T>)
            return _sub_overflow_i8(0, lhs, rhs, reinterpret_cast<signed char*>(__builtin_addressof(res)));
        else if constexpr (sizeof(T) == 1 && std::is_unsigned_v<T>)
            return _subborrow_u8(0, lhs, rhs, reinterpret_cast<unsigned char*>(__builtin_addressof(res)));
        else if constexpr (std::same_as<T, signed short>)
            return _sub_overflow_i16(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned short>)
            return _subborrow_u16(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, signed int>)
            return _sub_overflow_i32(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned int>)
            return _subborrow_u32(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, signed __int64>)
            return _sub_overflow_i64(0, lhs, rhs, __builtin_addressof(res));
        else if constexpr (std::same_as<T, unsigned __int64>)
            return _subborrow_u64(0, lhs, rhs, __builtin_addressof(res));
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool mul(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1)
        {
#if defined(_M_IX86)
            // Workaround for bad MSVC Codegen
            // https://developercommunity.visualstudio.com/t/Bad-codegen-for-x86-_mul_full_overflow_i/10668881?sort=newest
#define __cast reinterpret_cast<std::conditional_t<std::is_signed_v<T>, signed short, unsigned short>*>
            using expanded_type = std::conditional_t<std::is_signed_v<T>, signed int, unsigned int>;
#else
#define __cast(...) __VA_ARGS__
            using expanded_type = std::conditional_t<std::is_signed_v<T>, signed short, unsigned short>;
#endif
            expanded_type hig = res;
            bool          out = false;
            if constexpr (std::is_signed_v<T>)
                out = _mul_full_overflow_i8(lhs, rhs, __cast(__builtin_addressof(hig)));
            else if constexpr (std::is_unsigned_v<T>)
                out = _mul_full_overflow_u8(lhs, rhs, __cast(__builtin_addressof(hig)));
            res = static_cast<T>(hig);
            return out;
#undef __cast
        }
        else
        {
            T hig;
            if constexpr (std::same_as<T, signed short>)
                return _mul_full_overflow_i16(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
            else if constexpr (std::same_as<T, unsigned short>)
                return _mul_full_overflow_u16(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
            else if constexpr (std::same_as<T, signed int>)
                return _mul_full_overflow_i32(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
            else if constexpr (std::same_as<T, unsigned int>)
                return _mul_full_overflow_u32(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
            else if constexpr (std::same_as<T, signed __int64>)
                return _mul_full_overflow_i64(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
            else if constexpr (std::same_as<T, unsigned __int64>)
                return _mul_full_overflow_u64(lhs, rhs, __builtin_addressof(res), __builtin_addressof(hig));
        }
    }
#elif CHECK_USE == CHECK_USE_EXTERN_ASM
    extern "C"
    {
        bool _ljh_asm_add_i8(signed char*, signed char, signed char) noexcept;
        bool _ljh_asm_add_u8(unsigned char*, unsigned char, unsigned char) noexcept;
        bool _ljh_asm_add_i16(signed short*, signed short, signed short) noexcept;
        bool _ljh_asm_add_u16(unsigned short*, unsigned short, unsigned short) noexcept;
        bool _ljh_asm_add_i32(signed int*, signed int, signed int) noexcept;
        bool _ljh_asm_add_u32(unsigned int*, unsigned int, unsigned int) noexcept;
        bool _ljh_asm_add_i64(signed __int64*, signed __int64, signed __int64) noexcept;
        bool _ljh_asm_add_u64(unsigned __int64*, unsigned __int64, unsigned __int64) noexcept;

        bool _ljh_asm_sub_i8(signed char*, signed char, signed char) noexcept;
        bool _ljh_asm_sub_u8(unsigned char*, unsigned char, unsigned char) noexcept;
        bool _ljh_asm_sub_i16(signed short*, signed short, signed short) noexcept;
        bool _ljh_asm_sub_u16(unsigned short*, unsigned short, unsigned short) noexcept;
        bool _ljh_asm_sub_i32(signed int*, signed int, signed int) noexcept;
        bool _ljh_asm_sub_u32(unsigned int*, unsigned int, unsigned int) noexcept;
        bool _ljh_asm_sub_i64(signed __int64*, signed __int64, signed __int64) noexcept;
        bool _ljh_asm_sub_u64(unsigned __int64*, unsigned __int64, unsigned __int64) noexcept;

        bool _ljh_asm_mul_i8(signed char*, signed char, signed char) noexcept;
        bool _ljh_asm_mul_u8(unsigned char*, unsigned char, unsigned char) noexcept;
        bool _ljh_asm_mul_i16(signed short*, signed short, signed short) noexcept;
        bool _ljh_asm_mul_u16(unsigned short*, unsigned short, unsigned short) noexcept;
        bool _ljh_asm_mul_i32(signed int*, signed int, signed int) noexcept;
        bool _ljh_asm_mul_u32(unsigned int*, unsigned int, unsigned int) noexcept;
        bool _ljh_asm_mul_i64(signed __int64*, signed __int64, signed __int64) noexcept;
        bool _ljh_asm_mul_u64(unsigned __int64*, unsigned __int64, unsigned __int64) noexcept;
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool add(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1 && std::is_signed_v<T>)
            return _ljh_asm_add_i8(reinterpret_cast<signed char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (sizeof(T) == 1 && std::is_unsigned_v<T>)
            return _ljh_asm_add_u8(reinterpret_cast<unsigned char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (std::same_as<T, signed short>)
            return _ljh_asm_add_i16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned short>)
            return _ljh_asm_add_u16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed int>)
            return _ljh_asm_add_i32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned int>)
            return _ljh_asm_add_u32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed __int64>)
            return _ljh_asm_add_i64(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned __int64>)
            return _ljh_asm_add_u64(__builtin_addressof(res), lhs, rhs);
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool sub(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1 && std::is_signed_v<T>)
            return _ljh_asm_sub_i8(reinterpret_cast<signed char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (sizeof(T) == 1 && std::is_unsigned_v<T>)
            return _ljh_asm_sub_u8(reinterpret_cast<unsigned char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (std::same_as<T, signed short>)
            return _ljh_asm_sub_i16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned short>)
            return _ljh_asm_sub_u16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed int>)
            return _ljh_asm_sub_i32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned int>)
            return _ljh_asm_sub_u32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed __int64>)
            return _ljh_asm_sub_i64(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned __int64>)
            return _ljh_asm_sub_u64(__builtin_addressof(res), lhs, rhs);
    }

    LJH_MODULE_MATH_EXPORT template<typename T, typename U, typename V>
    [[nodiscard]] inline bool mul(T& res, U lhs, V rhs) noexcept
    {
        if constexpr (sizeof(T) == 1 && std::is_signed_v<T>)
            return _ljh_asm_mul_i8(reinterpret_cast<signed char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (sizeof(T) == 1 && std::is_unsigned_v<T>)
            return _ljh_asm_mul_u8(reinterpret_cast<unsigned char*>(__builtin_addressof(res)), lhs, rhs);
        else if constexpr (std::same_as<T, signed short>)
            return _ljh_asm_mul_i16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned short>)
            return _ljh_asm_mul_u16(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed int>)
            return _ljh_asm_mul_i32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned int>)
            return _ljh_asm_mul_u32(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, signed __int64>)
            return _ljh_asm_mul_i64(__builtin_addressof(res), lhs, rhs);
        else if constexpr (std::same_as<T, unsigned __int64>)
            return _ljh_asm_mul_u64(__builtin_addressof(res), lhs, rhs);
    }
#endif
} // namespace ljh::ckd

namespace ljh
{
    LJH_MODULE_MATH_EXPORT template<std::integral T>
        requires(!std::same_as<T, bool>)
    struct checked
    {
        [[nodiscard]] constexpr checked() noexcept               = default;
        [[nodiscard]] constexpr checked(checked const&) noexcept = default;
        constexpr checked& operator=(checked const&) noexcept    = default;
        [[nodiscard]] constexpr checked(checked&&) noexcept      = default;
        constexpr checked& operator=(checked&&) noexcept         = default;
        constexpr ~checked() noexcept                            = default;

        [[nodiscard]] constexpr checked(T value) noexcept
            : _value(value)
        {
            static_assert(sizeof(checked) == sizeof(T));
            static_assert(alignof(checked) == alignof(T));
            static_assert(std::is_trivially_constructible_v<checked> == std::is_trivially_constructible_v<T>);
            static_assert(std::is_trivially_copy_constructible_v<checked> == std::is_trivially_copy_constructible_v<T>);
            static_assert(std::is_trivially_move_constructible_v<checked> == std::is_trivially_move_constructible_v<T>);
            static_assert(std::is_trivially_copy_assignable_v<checked> == std::is_trivially_copy_assignable_v<T>);
            static_assert(std::is_trivially_move_assignable_v<checked> == std::is_trivially_move_assignable_v<T>);
            static_assert(std::is_nothrow_constructible_v<checked> == std::is_nothrow_constructible_v<T>);
            static_assert(std::is_nothrow_copy_constructible_v<checked> == std::is_nothrow_copy_constructible_v<T>);
            static_assert(std::is_nothrow_move_constructible_v<checked> == std::is_nothrow_move_constructible_v<T>);
            static_assert(std::is_nothrow_copy_assignable_v<checked> == std::is_nothrow_copy_assignable_v<T>);
            static_assert(std::is_nothrow_move_assignable_v<checked> == std::is_nothrow_move_assignable_v<T>);
        }

        explicit operator T() const noexcept
        {
            return _value;
        }

        [[nodiscard]] friend constexpr bool operator==(checked lhs, checked rhs) noexcept  = default;
        [[nodiscard]] friend constexpr auto operator<=>(checked lhs, checked rhs) noexcept = default;

        checked& operator++()
        {
            LJH_CHECKED_MATH(ljh::ckd::add(_value, _value, 1));
            return *this;
        }

        checked operator++(int)
        {
            auto old = *this;
            LJH_CHECKED_MATH(ljh::ckd::add(_value, _value, 1));
            return old;
        }

        checked& operator--()
        {
            LJH_CHECKED_MATH(ljh::ckd::sub(_value, _value, 1));
            return *this;
        }

        checked operator--(int)
        {
            auto old = *this;
            LJH_CHECKED_MATH(ljh::ckd::sub(_value, _value, 1));
            return old;
        }

        friend checked operator+(checked lhs, checked rhs)
        {
            checked output;
            LJH_CHECKED_MATH(ljh::ckd::add(output._value, lhs._value, rhs._value));
            return output;
        }

        friend checked operator+=(checked& lhs, checked rhs)
        {
            LJH_CHECKED_MATH(ljh::ckd::add(lhs._value, lhs._value, rhs._value));
            return lhs;
        }

        friend checked operator-(checked lhs, checked rhs)
        {
            checked output;
            LJH_CHECKED_MATH(ljh::ckd::sub(output._value, lhs._value, rhs._value));
            return output;
        }

        friend checked operator-=(checked& lhs, checked rhs)
        {
            LJH_CHECKED_MATH(ljh::ckd::sub(lhs._value, lhs._value, rhs._value));
            return lhs;
        }

        friend checked operator*(checked lhs, checked rhs)
        {
            checked output;
            LJH_CHECKED_MATH(ljh::ckd::mul(output._value, lhs._value, rhs._value));
            return output;
        }

        friend checked operator*=(checked& lhs, checked rhs)
        {
            LJH_CHECKED_MATH(ljh::ckd::mul(lhs._value, lhs._value, rhs._value));
            return lhs;
        }

    private:
        T _value;
    };

    LJH_MODULE_MATH_EXPORT using checked_char               = checked<char>;
    LJH_MODULE_MATH_EXPORT using checked_signed_char        = checked<signed char>;
    LJH_MODULE_MATH_EXPORT using checked_unsigned_char      = checked<unsigned char>;
    LJH_MODULE_MATH_EXPORT using checked_short              = checked<short>;
    LJH_MODULE_MATH_EXPORT using checked_signed_short       = checked<signed short>;
    LJH_MODULE_MATH_EXPORT using checked_unsigned_short     = checked<unsigned short>;
    LJH_MODULE_MATH_EXPORT using checked_int                = checked<int>;
    LJH_MODULE_MATH_EXPORT using checked_signed_int         = checked<signed int>;
    LJH_MODULE_MATH_EXPORT using checked_unsigned_int       = checked<unsigned int>;
    LJH_MODULE_MATH_EXPORT using checked_long_long          = checked<long long>;
    LJH_MODULE_MATH_EXPORT using checked_signed_long_long   = checked<signed long long>;
    LJH_MODULE_MATH_EXPORT using checked_unsigned_long_long = checked<unsigned long long>;
} // namespace ljh

namespace std
{
    template<typename T>
    struct numeric_limits<ljh::checked<T>>
    {
    public:
        [[nodiscard]] static constexpr ljh::checked<T>(min)() noexcept
        {
            return std::numeric_limits<T>::min();
        }

        [[nodiscard]] static constexpr ljh::checked<T>(max)() noexcept
        {
            return std::numeric_limits<T>::max();
        }

        [[nodiscard]] static constexpr ljh::checked<T> lowest() noexcept
        {
            return (min)();
        }

        [[nodiscard]] static constexpr ljh::checked<T> epsilon() noexcept
        {
            return std::numeric_limits<T>::epsilon();
        }

        [[nodiscard]] static constexpr ljh::checked<T> round_error() noexcept
        {
            return std::numeric_limits<T>::round_error();
        }

        [[nodiscard]] static constexpr ljh::checked<T> denorm_min() noexcept
        {
            return std::numeric_limits<T>::denorm_min();
        }

        [[nodiscard]] static constexpr ljh::checked<T> infinity() noexcept
        {
            return std::numeric_limits<T>::infinity();
        }

        [[nodiscard]] static constexpr ljh::checked<T> quiet_NaN() noexcept
        {
            return std::numeric_limits<T>::quiet_NaN();
        }

        [[nodiscard]] static constexpr ljh::checked<T> signaling_NaN() noexcept
        {
            return std::numeric_limits<T>::signaling_NaN();
        }

        static constexpr bool                   is_specialized    = true;
        static constexpr bool                   has_infinity      = std::numeric_limits<T>::has_infinity;
        static constexpr bool                   has_quiet_NaN     = std::numeric_limits<T>::has_quiet_NaN;
        static constexpr bool                   has_signaling_NaN = std::numeric_limits<T>::has_signaling_NaN;
        static constexpr bool                   is_bounded        = std::numeric_limits<T>::is_bounded;
        static constexpr bool                   is_exact          = std::numeric_limits<T>::is_exact;
        static constexpr bool                   is_iec559         = std::numeric_limits<T>::is_iec559;
        static constexpr bool                   is_integer        = std::numeric_limits<T>::is_integer;
        static constexpr bool                   is_modulo         = std::numeric_limits<T>::is_modulo;
        static constexpr bool                   is_signed         = std::numeric_limits<T>::is_signed;
        static constexpr bool                   tinyness_before   = std::numeric_limits<T>::tinyness_before;
        static constexpr bool                   traps             = std::numeric_limits<T>::traps;
        static constexpr std::float_round_style round_style       = std::numeric_limits<T>::round_style;
        static constexpr int                    digits            = std::numeric_limits<T>::digits;
        static constexpr int                    digits10          = std::numeric_limits<T>::digits10;
        static constexpr int                    max_digits10      = std::numeric_limits<T>::max_digits10;
        static constexpr int                    max_exponent      = std::numeric_limits<T>::max_exponent;
        static constexpr int                    max_exponent10    = std::numeric_limits<T>::max_exponent10;
        static constexpr int                    min_exponent      = std::numeric_limits<T>::min_exponent;
        static constexpr int                    min_exponent10    = std::numeric_limits<T>::min_exponent10;
        static constexpr int                    radix             = std::numeric_limits<T>::radix;
    };

#if __has_include(<format>)
    template<typename T, typename C>
        requires requires { typename std::formatter<T, C>; }
    struct formatter<ljh::checked<T>, C> : std::formatter<T, C>
    {
        template<typename FC>
        FC::iterator format(ljh::checked<T> const& value, FC& ctx) const
        {
            return std::formatter<T, C>::format(static_cast<T>(value), ctx);
        }
    };
#endif
} // namespace std