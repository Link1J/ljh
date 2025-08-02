
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#include "ljh/area/point.hpp"
#include "ljh/area/rect.hpp"
#include "ljh/area/region.hpp"
#include "ljh/area/size.hpp"
#include "ljh/area/tagged_rect.hpp"
#include "ljh/checked_math.hpp"
#include "ljh/color.hpp"
#include "ljh/fixed_point.hpp"
export module ljh.math;
export namespace ljh {
	namespace ckd {
		using ljh::ckd::add;
		using ljh::ckd::sub;
		using ljh::ckd::mul;
	}
	using ljh::checked;
	using ljh::checked_char;
	using ljh::checked_signed_char;
	using ljh::checked_unsigned_char;
	using ljh::checked_short;
	using ljh::checked_signed_short;
	using ljh::checked_unsigned_short;
	using ljh::checked_int;
	using ljh::checked_signed_int;
	using ljh::checked_unsigned_int;
	using ljh::checked_long_long;
	using ljh::checked_signed_long_long;
	using ljh::checked_unsigned_long_long;
	using ljh::fixed_point;
	using ljh::basic_point;
	using ljh::basic_size;
	using ljh::basic_rect;
	using ljh::basic_tagged_rect;
	using ljh::basic_region;
	using ljh::point;
	using ljh::size;
	using ljh::rect;
	using ljh::region;
	using ljh::tagged_region;
	using ljh::ipoint;
	using ljh::isize;
	using ljh::irect;
	using ljh::iregion;
	using ljh::tagged_iregion;
}