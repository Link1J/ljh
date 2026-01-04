
//          Copyright Jared Irwin 2025-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_MAIN 1
#define LJH_MODULE_MAIN_EXPORT export
export module ljh;
extern "C++"
{
#include "ljh/bitmask_operators.hpp"
#include "ljh/casting.hpp"
#include "ljh/concepts.hpp"
#include "ljh/defer.hpp"
#include "ljh/enum_array.hpp"
#include "ljh/expected.hpp"
#include "ljh/function_traits.hpp"
#include "ljh/function_pointer.hpp"
#include "ljh/get_index.hpp"
#include "ljh/int_types.hpp"
#include "ljh/overload.hpp"
#include "ljh/version.hpp"
#include "ljh/typename.hpp"
#include "ljh/type_traits.hpp"
#include "ljh/unreachable.hpp"
#include "ljh/format/ranges.hpp"
#include "ljh/ranges/actions.hpp"
#include "ljh/ranges/bind_back.hpp"
#include "ljh/ranges/elements_of.hpp"
#include "ljh/ranges/range_adaptor_closure.hpp"
#include "ljh/ranges/terminators.hpp"
#include "ljh/ranges/utility.hpp"
#include "ljh/ranges/views.hpp"
}