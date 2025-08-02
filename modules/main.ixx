
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
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
export module ljh;
export namespace ljh {
	namespace bitmask_operators {
		using ljh::bitmask_operators::enable;
		using ljh::bitmask_operators::enable_t;
		namespace ops {
			using ljh::bitmask_operators::ops::operator|;
 			using ljh::bitmask_operators::ops::operator&;
 			using ljh::bitmask_operators::ops::operator^;
 			using ljh::bitmask_operators::ops::operator~;
 			using ljh::bitmask_operators::ops::operator|=;
 			using ljh::bitmask_operators::ops::operator&=;
 			using ljh::bitmask_operators::ops::operator^=;
		}
	}
	using ljh::underlying_cast;
	using ljh::pointer_cast;
	using ljh::reinterpret;
	using ljh::dereference_as;
	using ljh::one_of;
	using ljh::char_type;
	using ljh::scoped_enum;
	using ljh::function_type;
	using ljh::either_same_as;
	using ljh::instance;
	using ljh::has_tuple_element;
	using ljh::tuple_like;
	using ljh::pair_like;
	using ljh::scope_exit;
	using ljh::scope_fail;
	using ljh::scope_success;
	using ljh::condition_scope_exit;
	using ljh::condition_scope_fail;
	using ljh::condition_scope_success;
	using ljh::enum_array;
	using ljh::expected;
	using ljh::unexpected;
	using ljh::bad_expected_access;
	using ljh::operator==;
	using ljh::operator!=;
	using ljh::swap;
	using ljh::function_traits;
	using ljh::function_pointer;
	using ljh::funcptr;
	using ljh::matches_return_type;
	using ljh::get_index;
	using ljh::get_index_if;
	inline namespace int_types
	{
		using ljh::int_types::s08;
		using ljh::int_types::s16;
		using ljh::int_types::s32;
		using ljh::int_types::s64;
		using ljh::int_types::u08;
		using ljh::int_types::u16;
		using ljh::int_types::u32;
		using ljh::int_types::u64;
		using ljh::int_types::f32;
		using ljh::int_types::f64;
	}
	using ljh::overload;
	namespace limit
	{
		using ljh::limit::limiting;
		using ljh::limit::max_t;
		using ljh::limit::min_t;
		using ljh::limit::equal_t;
		using ljh::limit::always_t;
		using ljh::limit::never_t;
	}
	using ljh::version;
	using ljh::type_name;
	using ljh::is_scoped_enum;
	using ljh::is_char_type;
	using ljh::is_callable;
	using ljh::is_function_pointer;
	using ljh::is_instance;
	using ljh::is_type_complete;
	using ljh::void_t;
	using ljh::empty;
	using ljh::type_or_empty;
	using ljh::type_if_defined;
	using ljh::is_scoped_enum_v;
	using ljh::is_char_type_v;
	using ljh::is_callable_v;
	using ljh::is_function_pointer_v;
	using ljh::is_instance_v;
	using ljh::is_type_complete_v;
	using ljh::remove_cvref;
	using ljh::remove_cvref_t;
	using ljh::is_bounded_array;
	using ljh::is_bounded_array_v;
	using ljh::type_identity;
	using ljh::type_identity_t;
	using ljh::make_like;
	using ljh::make_like_t;
	using ljh::unreachable;
	namespace fmt {
		using ljh::fmt::join;
	}
	using ljh::bind_back;
	using ljh::constructor;
	namespace ranges {
		using ljh::ranges::elements_of;
		using ljh::ranges::range_adaptor_closure;
		using ljh::ranges::is_range_adaptor_closure;
		using ljh::ranges::can_pipe;
		using ljh::ranges::can_compose;
		using ljh::ranges::closure;
		using ljh::ranges::adaptor;
		using ljh::ranges::first;
		using ljh::ranges::first_or;
		using ljh::ranges::last;
		using ljh::ranges::last_or;
		using ljh::ranges::subspan_const;
		using ljh::ranges::subspan;
		using ljh::ranges::first_n_const;
		using ljh::ranges::first_n;
		using ljh::ranges::last_n_const;
		using ljh::ranges::last_n;
		namespace actions {
			using ljh::ranges::actions::to_array;
		}
		namespace views {
			using ljh::ranges::views::transform_into;
			using ljh::ranges::views::transform_tuple;
#if __cpp_lib_ranges_zip >= 202110L
			using ljh::ranges::views::zip;
#endif
#if __cpp_lib_ranges_fold >= 202207L
			using ljh::ranges::views::fold_left;
#endif
		}
	}
}

export using namespace ljh::bitmask_operators::ops;