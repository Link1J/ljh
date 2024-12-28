
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
    SECTION("sub")
    {
        int32_t  c;
        uint32_t a = 0x7FFFFFFF;
        uint32_t b = 0x80000000;
        REQUIRE_FALSE(ljh::ckd::add(c, a, b));
        REQUIRE(c == -1);
    }
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