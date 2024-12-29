
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define LJH__fastfail(...) throw __VA_ARGS__;

#include "ljh/checked_math.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_translate_exception.hpp>

#include <bit>

CATCH_TRANSLATE_EXCEPTION(int const&)
{
    return "__fastfail(FAST_FAIL_RANGE_CHECK_FAILURE)";
}

namespace Catch
{
    template<std::integral T>
    struct StringMaker<ljh::checked<T>>
    {
        static std::string convert(ljh::checked<T> const& value)
        {
            return StringMaker<T>::convert(std::bit_cast<T>(value));
        }
    };
} // namespace Catch

template<typename T>
struct numeric_limits
{
    using U = std::make_unsigned_t<T>;

    static constexpr T add_overflow = std::numeric_limits<T>::min() + 1;
    static constexpr T sub_overflow = std::numeric_limits<T>::max() - 1;
    static constexpr T mul_overflow = std::bit_cast<T>(U(std::numeric_limits<U>::max() - 1));
    static constexpr T inc_overflow = std::numeric_limits<T>::min();
    static constexpr T dec_overflow = std::numeric_limits<T>::max();
};

template<typename T>
struct numeric_limits<ljh::checked<T>>
{
    static constexpr ljh::checked<T> add_overflow = numeric_limits<T>::add_overflow;
    static constexpr ljh::checked<T> sub_overflow = numeric_limits<T>::sub_overflow;
    static constexpr ljh::checked<T> mul_overflow = numeric_limits<T>::mul_overflow;
    static constexpr ljh::checked<T> inc_overflow = numeric_limits<T>::inc_overflow;
    static constexpr ljh::checked<T> dec_overflow = numeric_limits<T>::dec_overflow;
};

TEMPLATE_TEST_CASE("checked_math - function - add", "[test_20][checked_math][c_like]", char, signed char, unsigned char, short, signed short, unsigned short,
                   int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    SECTION("No Overflow")
    {
        TestType a = 0;
        REQUIRE_FALSE(ljh::ckd::add(a, 4, 2));
        CHECK(a == 6);
    }
    SECTION("Overflow")
    {
        TestType a = 0;
        REQUIRE(ljh::ckd::add(a, std::numeric_limits<TestType>::max(), 2));
        CHECK(a == numeric_limits<TestType>::add_overflow);
    }
}

TEMPLATE_TEST_CASE("checked_math - function - sub", "[test_20][checked_math][c_like]", char, signed char, unsigned char, short, signed short, unsigned short,
                   int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    SECTION("No Overflow")
    {
        TestType a = 0;
        REQUIRE_FALSE(ljh::ckd::sub(a, 4, 2));
        CHECK(a == 2);
    }
    SECTION("Overflow")
    {
        TestType a = 0;
        REQUIRE(ljh::ckd::sub(a, std::numeric_limits<TestType>::min(), 2));
        CHECK(a == numeric_limits<TestType>::sub_overflow);
    }
}

TEMPLATE_TEST_CASE("checked_math - function - mul", "[test_20][checked_math][c_like]", char, signed char, unsigned char, short, signed short, unsigned short,
                   int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    SECTION("No Overflow")
    {
        TestType a = 0;
        REQUIRE_FALSE(ljh::ckd::mul(a, 4, 2));
        CHECK(a == 8);
    }
    SECTION("Overflow")
    {
        TestType a = 0;
        REQUIRE(ljh::ckd::mul(a, std::numeric_limits<TestType>::max(), 2));
        CHECK(a == numeric_limits<TestType>::mul_overflow);
    }
}

TEST_CASE("checked_math - function - infinite precision", "[test_20][checked_math][c_like]")
{
    SECTION("add")
    {
        uint32_t c;
        int32_t  a = 0x7FFFFFFF;
        int32_t  b = 2;
        REQUIRE_FALSE(ljh::ckd::add(c, a, b));
        REQUIRE(c == 0x80000001u);
    }
    //! This one is currently broken, and not fixable.
    // SECTION("sub")
    // {
    //     int32_t  c;
    //     uint32_t a = 0x7FFFFFFF;
    //     uint32_t b = 0x80000000;
    //     REQUIRE_FALSE(ljh::ckd::sub(c, a, b));
    //     REQUIRE(c == -1);
    // }
}

TEMPLATE_TEST_CASE("checked_math - function - numeric_limits", "[test_20][checked_math][c++_type]", char, signed char, unsigned char, short, signed short,
                   unsigned short, int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    REQUIRE(std::numeric_limits<TestType>::min() == std::numeric_limits<ljh::checked<TestType>>::min());
    REQUIRE(std::numeric_limits<TestType>::max() == std::numeric_limits<ljh::checked<TestType>>::max());
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - add", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a + 2);
        auto b = a + 2;
        REQUIRE(b == 6);
        REQUIRE(a == 4);
    }
    SECTION("Overflow")
    {
        TestType a = std::numeric_limits<TestType>::max();
        REQUIRE_THROWS_AS(a + 2, int);
        REQUIRE(a == std::numeric_limits<TestType>::max());
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - add asssign", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a += 2);
        REQUIRE(a == 6);
    }
    SECTION("Overflow")
    {
        TestType a = std::numeric_limits<TestType>::max();
        REQUIRE_THROWS_AS(a += 2, int);
        REQUIRE(a == numeric_limits<TestType>::add_overflow);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - sub", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a - 2);
        auto b = a - 2;
        REQUIRE(b == 2);
        REQUIRE(a == 4);
    }
    SECTION("Overflow")
    {
        TestType a = std::numeric_limits<TestType>::min();
        REQUIRE_THROWS_AS(a - 2, int);
        REQUIRE(a == std::numeric_limits<TestType>::min());
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - sub assign", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a -= 2);
        REQUIRE(a == 2);
    }
    SECTION("Overflow")
    {
        TestType a = std::numeric_limits<TestType>::min();
        REQUIRE_THROWS_AS(a -= 2, int);
        REQUIRE(a == numeric_limits<TestType>::sub_overflow);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - mul", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a * 2);
        auto b = a * 2;
        REQUIRE(b == 8);
        REQUIRE(a == 4);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - mul assign", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("No Overflow")
    {
        TestType a = 4;
        REQUIRE_NOTHROW(a *= 2);
        REQUIRE(a == 8);
    }
    SECTION("Overflow")
    {
        TestType a = std::numeric_limits<TestType>::max();
        REQUIRE_THROWS_AS(a *= 2, int);
        REQUIRE(a == numeric_limits<TestType>::mul_overflow);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - inc", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("Prefix")
    {
        SECTION("No Overflow")
        {
            TestType a = 4;
            REQUIRE_NOTHROW(++a);
            REQUIRE(a == 5);
            SECTION("Correct Behaviour")
            {
                TestType a = 4;
                auto     b = ++a;
                CHECK(a == 5);
                REQUIRE(b == 5);
            }
        }
        SECTION("Overflow")
        {
            TestType a = std::numeric_limits<TestType>::max();
            REQUIRE_THROWS_AS(++a, int);
            REQUIRE(a == numeric_limits<TestType>::inc_overflow);
        }
    }
    SECTION("Postfix")
    {
        SECTION("No Overflow")
        {
            TestType a = 4;
            REQUIRE_NOTHROW(a++);
            REQUIRE(a == 5);
            SECTION("Correct Behaviour")
            {
                TestType a = 4;
                auto     b = a++;
                CHECK(a == 5);
                REQUIRE(b == 4);
            }
        }
        SECTION("Overflow")
        {
            TestType a = std::numeric_limits<TestType>::max();
            REQUIRE_THROWS_AS(a++, int);
            REQUIRE(a == numeric_limits<TestType>::inc_overflow);
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE("checked_math - type - dec", "[test_20][checked_math][c++_type]", ljh::checked,
                           (char, signed char, unsigned char, short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long,
                            unsigned long long))
{
    SECTION("Prefix")
    {
        SECTION("No Overflow")
        {
            TestType a = 4;
            REQUIRE_NOTHROW(--a);
            REQUIRE(a == 3);
            SECTION("Correct Behaviour")
            {
                TestType a = 4;
                auto     b = --a;
                CHECK(a == 3);
                REQUIRE(b == 3);
            }
        }
        SECTION("Overflow")
        {
            TestType a = std::numeric_limits<TestType>::min();
            REQUIRE_THROWS_AS(--a, int);
            REQUIRE(a == numeric_limits<TestType>::dec_overflow);
        }
    }
    SECTION("Postfix")
    {
        SECTION("No Overflow")
        {
            TestType a = 4;
            REQUIRE_NOTHROW(a--);
            REQUIRE(a == 3);
            SECTION("Correct Behaviour")
            {
                TestType a = 4;
                auto     b = a--;
                CHECK(a == 3);
                REQUIRE(b == 4);
            }
        }
        SECTION("Overflow")
        {
            TestType a = std::numeric_limits<TestType>::min();
            REQUIRE_THROWS_AS(a--, int);
            REQUIRE(a == numeric_limits<TestType>::dec_overflow);
        }
    }
}

#if __has_include(<format>)
TEMPLATE_TEST_CASE("checked_math - type - std::format", "[test_20][checked_math][c++_type]", char, signed char, unsigned char, short, signed short,
                   unsigned short, int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    using namespace std::literals;
    using T = TestType;
    using C = ljh::checked<T>;

    constexpr T t_min = std::numeric_limits<T>::min();
    constexpr T t_max = std::numeric_limits<T>::max();
    constexpr C c_min = std::numeric_limits<C>::min();
    constexpr C c_max = std::numeric_limits<C>::max();

    constexpr bool do_signed_checks = std::is_signed_v<T> && !std::is_same_v<T, char>;

    REQUIRE(std::format("{:d}", T{0}) == "0");
    REQUIRE(std::format("{:d}", T{1}) == "1");
    REQUIRE(std::format("{:d}", T{16}) == "16");
    REQUIRE(std::format("{:b}", T{0}) == "0");
    REQUIRE(std::format("{:b}", T{1}) == "1");
    REQUIRE(std::format("{:b}", T{16}) == "10000");
    REQUIRE(std::format("{:c}", T{'A'}) == "A");
    if constexpr (do_signed_checks)
    {
        REQUIRE(std::format("{:d}", T{-0}) == "0");
        REQUIRE(std::format("{:d}", T{-1}) == "-1");
        REQUIRE(std::format("{:d}", T{-16}) == "-16");
        REQUIRE(std::format("{:b}", T{-0}) == "0");
        REQUIRE(std::format("{:b}", T{-1}) == "-1");
        REQUIRE(std::format("{:b}", T{-16}) == "-10000");
    }

    SECTION("Default")
    {
        if constexpr (std::is_same_v<T, char>)
        {
            CHECK(std::format("{}", C{0}) == "\x0"s);
            CHECK(std::format("{}", C{1}) == "\x1");
            CHECK(std::format("{}", C{16}) == "\x10");
        }
        else
        {
            CHECK(std::format("{}", C{0}) == "0");
            CHECK(std::format("{}", C{1}) == "1");
            CHECK(std::format("{}", C{16}) == "16");
        }
    }
    SECTION("Binary")
    {
        SECTION("Lower Case")
        {
            SECTION("Without Prefix")
            {
                REQUIRE(std::format("{:b}", C{0}) == "0");
                REQUIRE(std::format("{:b}", C{1}) == "1");
                REQUIRE(std::format("{:b}", C{16}) == "10000");
                if constexpr (do_signed_checks)
                {
                    REQUIRE(std::format("{:b}", C{-0}) == "0");
                    REQUIRE(std::format("{:b}", C{-1}) == "-1");
                    REQUIRE(std::format("{:b}", C{-16}) == "-10000");
                }
            }
            SECTION("With Prefix")
            {
                REQUIRE(std::format("{:#b}", C{0}) == "0b0");
                REQUIRE(std::format("{:#b}", C{1}) == "0b1");
                REQUIRE(std::format("{:#b}", C{16}) == "0b10000");
            }
        }
        SECTION("Upper Case")
        {
            SECTION("With Prefix")
            {
                REQUIRE(std::format("{:#B}", C{0}) == "0B0");
                REQUIRE(std::format("{:#B}", C{1}) == "0B1");
                REQUIRE(std::format("{:#B}", C{16}) == "0B10000");
            }
        }
    }
    SECTION("Character")
    {
        REQUIRE(std::format("{:c}", C{'A'}) == "A");
    }
    SECTION("Decimal")
    {
        SECTION("With Default Sign")
        {
            REQUIRE(std::format("{:d}", C{0}) == "0");
            REQUIRE(std::format("{:d}", C{1}) == "1");
            REQUIRE(std::format("{:d}", C{16}) == "16");
            if constexpr (do_signed_checks)
            {
                REQUIRE(std::format("{:d}", C{-0}) == "0");
                REQUIRE(std::format("{:d}", C{-1}) == "-1");
                REQUIRE(std::format("{:d}", C{-16}) == "-16");
            }
        }
        SECTION("With + Sign")
        {
            REQUIRE(std::format("{:+d}", C{0}) == "+0");
            REQUIRE(std::format("{:+d}", C{1}) == "+1");
            REQUIRE(std::format("{:+d}", C{16}) == "+16");
            if constexpr (do_signed_checks)
            {
                REQUIRE(std::format("{:+d}", C{-0}) == "+0");
                REQUIRE(std::format("{:+d}", C{-1}) == "-1");
                REQUIRE(std::format("{:+d}", C{-16}) == "-16");
            }
        }
        SECTION("With - Sign")
        {
            REQUIRE(std::format("{:-d}", C{0}) == "0");
            REQUIRE(std::format("{:-d}", C{1}) == "1");
            REQUIRE(std::format("{:-d}", C{16}) == "16");
            if constexpr (do_signed_checks)
            {
                REQUIRE(std::format("{:-d}", C{-0}) == "0");
                REQUIRE(std::format("{:-d}", C{-1}) == "-1");
                REQUIRE(std::format("{:-d}", C{-16}) == "-16");
            }
        }
        SECTION("With Space Sign")
        {
            REQUIRE(std::format("{: d}", C{0}) == " 0");
            REQUIRE(std::format("{: d}", C{1}) == " 1");
            REQUIRE(std::format("{: d}", C{16}) == " 16");
            if constexpr (do_signed_checks)
            {
                REQUIRE(std::format("{: d}", C{-0}) == " 0");
                REQUIRE(std::format("{: d}", C{-1}) == "-1");
                REQUIRE(std::format("{: d}", C{-16}) == "-16");
            }
        }
    }
}

TEMPLATE_TEST_CASE("checked_math - type - std::format - Matches Basic Type", "[test_20][checked_math][c++_type][!mayfail]", char, signed char, unsigned char,
                   short, signed short, unsigned short, int, signed int, unsigned int, long long, signed long long, unsigned long long)
{
    using T = TestType;
    using C = ljh::checked<T>;

    constexpr T t_min = std::numeric_limits<T>::min();
    constexpr T t_max = std::numeric_limits<T>::max();
    constexpr C c_min = std::numeric_limits<C>::min();
    constexpr C c_max = std::numeric_limits<C>::max();

    SECTION("Default")
    {
        CHECK(std::format("{}", C{0}) == std::format("{}", T{0}));
        CHECK(std::format("{}", C{1}) == std::format("{}", T{1}));
        CHECK(std::format("{}", C{16}) == std::format("{}", T{16}));
        CHECK(std::format("{}", c_min) == std::format("{}", t_min));
        CHECK(std::format("{}", c_max) == std::format("{}", t_max));
    }
    SECTION("Binary")
    {
        SECTION("Lower Case")
        {
            SECTION("Without Prefix")
            {
                CHECK(std::format("{:b}", C{0}) == std::format("{:b}", T{0}));
                CHECK(std::format("{:b}", C{1}) == std::format("{:b}", T{1}));
                CHECK(std::format("{:b}", C{16}) == std::format("{:b}", T{16}));
                CHECK(std::format("{:b}", c_min) == std::format("{:b}", t_min));
                CHECK(std::format("{:b}", c_max) == std::format("{:b}", t_max));
            }
            SECTION("With Prefix")
            {
                CHECK(std::format("{:#b}", C{0}) == std::format("{:#b}", T{0}));
                CHECK(std::format("{:#b}", C{1}) == std::format("{:#b}", T{1}));
                CHECK(std::format("{:#b}", C{16}) == std::format("{:#b}", T{16}));
                CHECK(std::format("{:#b}", c_min) == std::format("{:#b}", t_min));
                CHECK(std::format("{:#b}", c_max) == std::format("{:#b}", t_max));

                REQUIRE(std::format("{:#b}", C{0}) == "0b0");
                REQUIRE(std::format("{:#b}", C{1}) == "0b1");
                REQUIRE(std::format("{:#b}", C{16}) == "0b10000");
            }
        }
        SECTION("Upper Case")
        {
            SECTION("Without Prefix")
            {
                CHECK(std::format("{:B}", C{0}) == std::format("{:B}", T{0}));
                CHECK(std::format("{:B}", C{1}) == std::format("{:B}", T{1}));
                CHECK(std::format("{:B}", C{16}) == std::format("{:B}", T{16}));
                CHECK(std::format("{:B}", c_min) == std::format("{:B}", t_min));
                CHECK(std::format("{:B}", c_max) == std::format("{:B}", t_max));

                CHECK(std::format("{:B}", C{0}) == std::format("{:b}", C{0}));
                CHECK(std::format("{:B}", C{1}) == std::format("{:b}", C{1}));
                CHECK(std::format("{:B}", C{16}) == std::format("{:b}", C{16}));
                CHECK(std::format("{:B}", c_min) == std::format("{:b}", c_min));
                CHECK(std::format("{:B}", c_max) == std::format("{:b}", c_max));
            }
            SECTION("With Prefix")
            {
                CHECK(std::format("{:#B}", C{0}) == std::format("{:#B}", T{0}));
                CHECK(std::format("{:#B}", C{1}) == std::format("{:#B}", T{1}));
                CHECK(std::format("{:#B}", C{16}) == std::format("{:#B}", T{16}));
                CHECK(std::format("{:#B}", c_min) == std::format("{:#B}", t_min));
                CHECK(std::format("{:#B}", c_max) == std::format("{:#B}", t_max));

                REQUIRE(std::format("{:#b}", C{0}) == "0B0");
                REQUIRE(std::format("{:#b}", C{1}) == "0B1");
                REQUIRE(std::format("{:#b}", C{16}) == "0B10000");
            }
        }
    }
    SECTION("Character")
    {
        CHECK(std::format("{:c}", C{0}) == std::format("{:c}", T{0}));
        CHECK(std::format("{:c}", C{1}) == std::format("{:c}", T{1}));
        CHECK(std::format("{:c}", C{16}) == std::format("{:c}", T{16}));
        CHECK(std::format("{:c}", C{' '}) == std::format("{:c}", T{' '}));
        CHECK(std::format("{:c}", C{'A'}) == std::format("{:c}", T{'A'}));
        CHECK(std::format("{:c}", C{'\t'}) == std::format("{:c}", T{'\t'}));
        CHECK(std::format("{:c}", C{'\b'}) == std::format("{:c}", T{'\b'}));

        REQUIRE("A" == std::format("{:c}", C{'A'}));
    }
    SECTION("Decimal")
    {
        SECTION("With Default Sign")
        {
            CHECK(std::format("{:d}", C{0}) == std::format("{:d}", T{0}));
            CHECK(std::format("{:d}", C{1}) == std::format("{:d}", T{1}));
            CHECK(std::format("{:d}", C{16}) == std::format("{:d}", T{16}));
            CHECK(std::format("{:d}", c_min) == std::format("{:d}", t_min));
            CHECK(std::format("{:d}", c_max) == std::format("{:d}", t_max));

            REQUIRE(std::format("{:d}", C{0}) == "0");
            REQUIRE(std::format("{:d}", C{1}) == "1");
            REQUIRE(std::format("{:d}", C{16}) == "16");
            if constexpr (std::is_signed_v<T>)
            {
                REQUIRE(std::format("{:d}", C{-0}) == "0");
                REQUIRE(std::format("{:d}", C{-1}) == "-1");
                REQUIRE(std::format("{:d}", C{-16}) == "-16");
            }
        }
        SECTION("With + Sign")
        {
            CHECK(std::format("{:+d}", C{0}) == std::format("{:+d}", T{0}));
            CHECK(std::format("{:+d}", C{1}) == std::format("{:+d}", T{1}));
            CHECK(std::format("{:+d}", C{16}) == std::format("{:+d}", T{16}));
            CHECK(std::format("{:+d}", c_min) == std::format("{:+d}", t_min));
            CHECK(std::format("{:+d}", c_max) == std::format("{:+d}", t_max));

            REQUIRE(std::format("{:+d}", C{0}) == "+0");
            REQUIRE(std::format("{:+d}", C{1}) == "+1");
            REQUIRE(std::format("{:+d}", C{16}) == "+16");
            if constexpr (std::is_signed_v<T>)
            {
                REQUIRE(std::format("{:+d}", C{-0}) == "0");
                REQUIRE(std::format("{:+d}", C{-1}) == "-1");
                REQUIRE(std::format("{:+d}", C{-16}) == "-16");
            }
        }
        SECTION("With - Sign")
        {
            CHECK(std::format("{:-d}", C{0}) == std::format("{:-d}", T{0}));
            CHECK(std::format("{:-d}", C{1}) == std::format("{:-d}", T{1}));
            CHECK(std::format("{:-d}", C{16}) == std::format("{:-d}", T{16}));
            CHECK(std::format("{:-d}", c_min) == std::format("{:-d}", t_min));
            CHECK(std::format("{:-d}", c_max) == std::format("{:-d}", t_max));

            REQUIRE(std::format("{:-d}", C{0}) == "0");
            REQUIRE(std::format("{:-d}", C{1}) == "1");
            REQUIRE(std::format("{:-d}", C{16}) == "16");
            if constexpr (std::is_signed_v<T>)
            {
                REQUIRE(std::format("{:-d}", C{-0}) == "0");
                REQUIRE(std::format("{:-d}", C{-1}) == "-1");
                REQUIRE(std::format("{:-d}", C{-16}) == "-16");
            }
        }
        SECTION("With Space Sign")
        {
            CHECK(std::format("{: d}", C{0}) == std::format("{: d}", T{0}));
            CHECK(std::format("{: d}", C{1}) == std::format("{: d}", T{1}));
            CHECK(std::format("{: d}", C{16}) == std::format("{: d}", T{16}));
            CHECK(std::format("{: d}", c_min) == std::format("{: d}", t_min));
            CHECK(std::format("{: d}", c_max) == std::format("{: d}", t_max));

            REQUIRE(std::format("{: d}", C{0}) == " 0");
            REQUIRE(std::format("{: d}", C{1}) == " 1");
            REQUIRE(std::format("{: d}", C{16}) == " 16");
            if constexpr (std::is_signed_v<T>)
            {
                REQUIRE(std::format("{: d}", C{-0}) == " 0");
                REQUIRE(std::format("{: d}", C{-1}) == "-1");
                REQUIRE(std::format("{: d}", C{-16}) == "-16");
            }
        }
    }
    SECTION("Octal")
    {
        SECTION("Without Prefix")
        {
            CHECK(std::format("{:o}", C{0}) == std::format("{:o}", T{0}));
            CHECK(std::format("{:o}", C{1}) == std::format("{:o}", T{1}));
            CHECK(std::format("{:o}", C{16}) == std::format("{:o}", T{16}));
            CHECK(std::format("{:o}", c_min) == std::format("{:o}", t_min));
            CHECK(std::format("{:o}", c_max) == std::format("{:o}", t_max));
        }
        SECTION("With Prefix")
        {
            CHECK(std::format("{:#o}", C{0}) == std::format("{:#o}", T{0}));
            CHECK(std::format("{:#o}", C{1}) == std::format("{:#o}", T{1}));
            CHECK(std::format("{:#o}", C{16}) == std::format("{:#o}", T{16}));
            CHECK(std::format("{:#o}", c_min) == std::format("{:#o}", t_min));
            CHECK(std::format("{:#o}", c_max) == std::format("{:#o}", t_max));
        }
    }
    SECTION("Hex")
    {
        SECTION("Lower Case")
        {
            SECTION("Without Prefix")
            {
                CHECK(std::format("{:x}", C{0}) == std::format("{:x}", T{0}));
                CHECK(std::format("{:x}", C{1}) == std::format("{:x}", T{1}));
                CHECK(std::format("{:x}", C{16}) == std::format("{:x}", T{16}));
                CHECK(std::format("{:x}", c_min) == std::format("{:x}", t_min));
                CHECK(std::format("{:x}", c_max) == std::format("{:x}", t_max));
            }
            SECTION("With Prefix")
            {
                CHECK(std::format("{:#x}", C{0}) == std::format("{:#x}", T{0}));
                CHECK(std::format("{:#x}", C{1}) == std::format("{:#x}", T{1}));
                CHECK(std::format("{:#x}", C{16}) == std::format("{:#x}", T{16}));
                CHECK(std::format("{:#x}", c_min) == std::format("{:#x}", t_min));
                CHECK(std::format("{:#x}", c_max) == std::format("{:#x}", t_max));
            }
        }
        SECTION("Upper Case")
        {
            SECTION("Without Prefix")
            {
                CHECK(std::format("{:X}", C{0}) == std::format("{:X}", T{0}));
                CHECK(std::format("{:X}", C{1}) == std::format("{:X}", T{1}));
                CHECK(std::format("{:X}", C{16}) == std::format("{:X}", T{16}));
                CHECK(std::format("{:X}", c_min) == std::format("{:X}", t_min));
                CHECK(std::format("{:X}", c_max) == std::format("{:X}", t_max));
            }
            SECTION("With Prefix")
            {
                CHECK(std::format("{:#X}", C{0}) == std::format("{:#X}", T{0}));
                CHECK(std::format("{:#X}", C{1}) == std::format("{:#X}", T{1}));
                CHECK(std::format("{:#X}", C{16}) == std::format("{:#X}", T{16}));
                CHECK(std::format("{:#X}", c_min) == std::format("{:#X}", t_min));
                CHECK(std::format("{:#X}", c_max) == std::format("{:#X}", t_max));
            }
        }
    }
}
#endif