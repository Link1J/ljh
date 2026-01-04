
//          Copyright Jared Irwin 2025-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_MATH 1
#define LJH_MODULE_MATH_EXPORT export
export module ljh.math;
extern "C++"
{
#include "ljh/area/point.hpp"
#include "ljh/area/rect.hpp"
#include "ljh/area/region.hpp"
#include "ljh/area/size.hpp"
#include "ljh/area/tagged_rect.hpp"
#include "ljh/checked_math.hpp"
#include "ljh/color.hpp"
#include "ljh/fixed_point.hpp"
}