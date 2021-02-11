
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <ljh/string.hpp>
#include <cstdio>
#include <iostream>

static_assert(ljh::string("ABCDEFGHIJKLMNOPQRSTUVW"             ).size    () == 23);
static_assert(ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ"          ).size    () == 26);
static_assert(ljh::string(                                      ).capacity() == 23);
static_assert(ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789").capacity() == 63);

template<typename T>
constexpr auto sum(ljh::basic_string<T> string)
{
	typename ljh::basic_string<T>::size_type sum = 0;
	for (auto& letter : string) sum += letter;
	return sum;
}

template<typename T>
constexpr auto test_swap_part(ljh::basic_string<T>& string1)
{
	ljh::basic_string<T> string2 = "Hello world";
	std::swap(string1, string2);
}

constexpr auto test_swap()
{
	ljh::string string1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	test_swap_part(string1);
	return string1;
}

int main()
{
	constexpr ljh::wstring test1 = L"ABC";
	printf("%ls\n", test1.data());
	printf("%zd\n", test1.size());
	constexpr ljh::string test2 = "ABCDEFGHIJKLMNOPQRSTUVW";
	printf("%s\n", test2.data());
	printf("%zd\n", test2.size());
	ljh::string test3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	printf("%s\n", test3.data());
	printf("%zd\n", test3.size());
	constexpr auto test4 = ljh::wstring(L"ABC").size();
	printf("%zd\n", test4);
	constexpr ljh::string test5;
	printf("%s\n", test5.data());
	printf("%zd\n", test5.size());
	constexpr ljh::string test6 = test2;
	printf("%s\n", test6.data());
	printf("%zd\n", test6.size());
	/*constexpr*/ auto test7 = ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ").size();
	printf("%zd\n", test7);
	constexpr auto test8 = sum(ljh::string("ABCDEFGHIJKLMNOPQRSTUV"));
	printf("%zd\n", test8);
	/*constexpr*/ auto test9 = sum(ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	printf("%zd\n", test9);
	constexpr auto test10 = sum(ljh::wstring(L"ABCDEFGHIJ"));
	printf("%zd\n", test10);
	/*constexpr*/ auto test11 = sum(ljh::wstring(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	printf("%zd\n", test11);
	constexpr ljh::string test12 = {'a','b','c'};
	printf("%s\n", test12.data());
	printf("%zd\n", test12.size());
	constexpr ljh::string test13 = test_swap();
	printf("%s\n", test13.data());
	printf("%zd\n", test13.size());
	/*constexpr*/ ljh::string test14 = ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ").erase(0, 3);
	printf("%s\n", test14.data());
	printf("%zd\n", test14.size());
	/*constexpr*/ ljh::string test15 = ljh::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ").erase(0, 2);
	printf("%s\n", test15.data());
	printf("%zd\n", test15.size());

	ljh::string test16 = "ljh::string is awsome";
	std::cout << test16 << '\n';
}