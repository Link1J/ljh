
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// memory_mapped_file.hpp - v1.1
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
// Requires bitmask_operators.hpp
//
// ABOUT
//     Memory Mapped Files
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include <type_traits>
#include <filesystem>
#include "bitmask_operators.hpp"

namespace ljh::memory_mapped
{
#ifdef _WIN32
    LJH_MODULE_OS_EXPORT using handle = void*;
#else
    LJH_MODULE_OS_EXPORT using handle = int;
#endif

    LJH_MODULE_OS_EXPORT inline const handle invalid_handle = (handle)(-1);

    LJH_MODULE_OS_EXPORT enum class permissions
    {
        read          = 0b0001,
        write         = 0b0010,
        executable    = 0b0100,
        copy_on_write = 0b1000,
        r             = read,
        w             = write,
        x             = executable,
        rw            = read | write,
        rx            = read | executable,
        wx            = write | executable,
        rwx           = read | write | executable,
    };

    LJH_MODULE_OS_EXPORT class view;
    LJH_MODULE_OS_EXPORT class file;
    LJH_MODULE_OS_EXPORT class io_error;

    class file
    {
        friend class view;

    public:
        file() = default;
        file(std::filesystem::path&& filename, permissions permissions);
        ~file();
        file(file const&)            = delete;
        file& operator=(file const&) = delete;
        file(file&&);
        file& operator=(file&&);

        size_t size() const noexcept;
        bool   is_open() const noexcept;

    private:
        handle file_descriptor = invalid_handle;
        handle file_handle     = invalid_handle;
        size_t filesize        = 0;
    };

    class view
    {
    public:
        view() = default;
        view(file& fd, permissions permissions, size_t start, size_t length);
        ~view();
        view(view const&)            = delete;
        view& operator=(view const&) = delete;
        view(view&&);
        view& operator=(view&&);

        void flush() noexcept;
        bool valid() const noexcept;

        template<typename T>
        T* as() noexcept
        {
            if (sizeof(T) > length)
            {
                return nullptr;
            }
            return reinterpret_cast<T*>((char*)data + offset);
        }

        template<typename T>
        T const* as() const noexcept
        {
            if (sizeof(T) > length)
            {
                return nullptr;
            }
            return reinterpret_cast<T const*>((char*)data + offset);
        }

    private:
        void*  data   = nullptr;
        size_t length = 0;
        size_t offset = 0;
    };

    class io_error : public std::exception
    {
        uint32_t _error_code;

    public:
        explicit io_error();
        virtual char const* what() const noexcept override;
        uint32_t const      error_code() const noexcept;
        char const*         error_string() const noexcept;
    };

    LJH_MODULE_OS_EXPORT class invalid_position : public io_error
    {
    public:
        explicit invalid_position() = default;
        virtual char const* what() const noexcept override;
    };

    LJH_MODULE_OS_EXPORT class invalid_file : public io_error
    {
    public:
        explicit invalid_file() = default;
        virtual char const* what() const noexcept override;
    };

    LJH_MODULE_OS_EXPORT class invalid_permissions : public io_error
    {
    public:
        explicit invalid_permissions() = default;
        virtual char const* what() const noexcept override;
    };
} // namespace ljh::memory_mapped

template<>
struct ljh::bitmask_operators::enable<ljh::memory_mapped::permissions> : std::true_type
{};