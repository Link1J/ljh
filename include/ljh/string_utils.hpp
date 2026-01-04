
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// string_utils.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
//
// ABOUT
//     String utils
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "cpp_version.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cwchar>

#if __cpp_lib_string_view >= 201606L
#include <string_view>
#include "get_index.hpp"
#endif

namespace ljh
{
    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    void ltrim(std::basic_string<C, T, A>& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](C ch) { return !std::isspace(ch); }));
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    void rtrim(std::basic_string<C, T, A>& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](C ch) { return !std::isspace(ch); }).base(), s.end());
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    void trim(std::basic_string<C, T, A>& s)
    {
        ltrim(s);
        rtrim(s);
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    std::basic_string<C, T, A> ltrim_copy(std::basic_string<C, T, A> s)
    {
        ltrim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    std::basic_string<C, T, A> rtrim_copy(std::basic_string<C, T, A> s)
    {
        rtrim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    std::basic_string<C, T, A> trim_copy(std::basic_string<C, T, A> s)
    {
        trim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    std::vector<std::basic_string<C, T, A>> split(std::basic_string<C, T, A> const& s, C seperator, std::size_t max_elements = 0)
    {
        using size_type = typename std::basic_string<C, T, A>::size_type;

        std::vector<std::basic_string<C, T, A>> output;
        size_type                               prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::basic_string<C, T, A>::npos)
        {
            output.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = ++pos;

            if (output.size() == max_elements - 1)
                break;
        }

        output.push_back(s.substr(prev_pos));
        return output;
    }

    LJH_MODULE_STRING_EXPORT template<size_t S, typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    std::vector<std::basic_string<C, T, A>> split(std::basic_string<C, T, A> const& s, C const (&seperator)[S], std::size_t max_elements = 0)
    {
        using size_type = typename std::basic_string<C, T, A>::size_type;

        std::vector<std::basic_string<C, T, A>> output;
        size_type                               prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::basic_string<C, T, A>::npos)
        {
            output.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = pos += S - 1;

            if (output.size() == max_elements - 1)
                break;
        }

        output.push_back(s.substr(prev_pos));
        return output;
    }

#if __cpp_lib_string_view >= 201606L
    LJH_MODULE_STRING_EXPORT inline std::wstring convert_string(std::string_view str)
    {
        auto op = [state = std::mbstate_t(), from = str.data()](wchar_t* buf, size_t len) mutable noexcept { return std::mbsrtowcs(buf, &from, len, &state); };
        std::wstring output;
#if __cpp_lib_string_resize_and_overwrite >= 202110L
        output.resize_and_overwrite(op(nullptr, 0), op);
#else
        output.resize(op(nullptr, 0));
        output.resize(op(output.data(), output.size()));
#endif
        return output;
    }

    LJH_MODULE_STRING_EXPORT inline std::string convert_string(std::wstring_view wstr)
    {
        auto op = [state = std::mbstate_t(), from = wstr.data()](char* buf, size_t len) mutable noexcept { return std::wcsrtombs(buf, &from, len, &state); };
        std::string output;
#if __cpp_lib_string_resize_and_overwrite >= 202110L
        output.resize_and_overwrite(op(nullptr, 0), op);
#else
        output.resize(op(nullptr, 0));
        output.resize(op(output.data(), output.size()));
#endif
        return output;
    }
#else
    LJH_MODULE_STRING_EXPORT inline std::wstring convert_string(const std::string& str)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(str);
    }

    inline std::string convert_string(std::wstring const& wstr)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(wstr);
    }
#endif

#if __cpp_lib_string_view >= 201606L
    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    void ltrim(std::basic_string_view<C, T>& s)
    {
        s.remove_prefix(get_index_if(s.begin(), s.end(), [](C ch) { return !std::isspace(ch); }));
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    void rtrim(std::basic_string_view<C, T>& s)
    {
        s.remove_suffix(get_index_if(s.rbegin(), s.rend(), [](C ch) { return !std::isspace(ch); }));
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    void trim(std::basic_string_view<C, T>& s)
    {
        ltrim(s);
        rtrim(s);
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    std::basic_string_view<C, T> ltrim_copy(std::basic_string_view<C, T> s)
    {
        ltrim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    std::basic_string_view<C, T> rtrim_copy(std::basic_string_view<C, T> s)
    {
        rtrim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    std::basic_string_view<C, T> trim_copy(std::basic_string_view<C, T> s)
    {
        trim(s);
        return s;
    }

    LJH_MODULE_STRING_EXPORT template<typename C, typename T = std::char_traits<C>>
    std::vector<std::basic_string_view<C, T>> split(std::basic_string_view<C, T> const& s, C seperator, std::size_t max_elements = 0)
    {
        using size_type = typename std::basic_string_view<C, T>::size_type;

        std::vector<std::basic_string_view<C, T>> output;
        size_type                                 prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::basic_string_view<C, T>::npos)
        {
            output.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = ++pos;

            if (output.size() == max_elements - 1)
                break;
        }

        output.push_back(s.substr(prev_pos));
        return output;
    }

    LJH_MODULE_STRING_EXPORT template<size_t S, typename C, typename T = std::char_traits<C>>
    std::vector<std::basic_string_view<C, T>> split(std::basic_string_view<C, T> const& s, C const (&seperator)[S], std::size_t max_elements = 0)
    {
        using size_type = typename std::basic_string_view<C, T>::size_type;

        std::vector<std::basic_string_view<C, T>> output;
        size_type                                 prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::basic_string_view<C, T>::npos)
        {
            output.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = pos += S - 1;

            if (output.size() == max_elements - 1)
                break;
        }

        output.push_back(s.substr(prev_pos));
        return output;
    }
#endif
} // namespace ljh