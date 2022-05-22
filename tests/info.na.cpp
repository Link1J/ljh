
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <ljh/cpp_version.hpp>
#include <ljh/system_info.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#define COMPILER_FEATURE_VALUE(value) #value
#define COMPILER_FEATURE_ENTRY(name) CompilerFeature{ #name, COMPILER_FEATURE_VALUE(name) },

#define COMPILER_ATTRIBUTE_VALUE_AS_STRING(s) #s
#define COMPILER_ATTRIBUTE_AS_NUMBER(x) COMPILER_ATTRIBUTE_VALUE_AS_STRING(x)
#define COMPILER_ATTRIBUTE_ENTRY(attr) CompilerFeature{ #attr, COMPILER_ATTRIBUTE_AS_NUMBER(__has_cpp_attribute(attr)) },

struct CompilerFeature
{
	CompilerFeature(const char* name = nullptr, const char* value = nullptr)
		: name(name), value(value) {}
	const char* name; const char* value;
};

static CompilerFeature cxx[] =
{
COMPILER_FEATURE_ENTRY(__cplusplus)
COMPILER_FEATURE_ENTRY(__cpp_exceptions)
COMPILER_FEATURE_ENTRY(__cpp_rtti)
#if defined(LJH_COMPILER_MSVC)
COMPILER_FEATURE_ENTRY(_MSVC_LANG)
#endif
};
static CompilerFeature cxx11[] =
{
COMPILER_FEATURE_ENTRY(__cpp_alias_templates)
COMPILER_FEATURE_ENTRY(__cpp_attributes)
COMPILER_FEATURE_ENTRY(__cpp_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_decltype)
COMPILER_FEATURE_ENTRY(__cpp_delegating_constructors)
COMPILER_FEATURE_ENTRY(__cpp_inheriting_constructors)
COMPILER_FEATURE_ENTRY(__cpp_initializer_lists)
COMPILER_FEATURE_ENTRY(__cpp_lambdas)
COMPILER_FEATURE_ENTRY(__cpp_nsdmi)
COMPILER_FEATURE_ENTRY(__cpp_range_based_for)
COMPILER_FEATURE_ENTRY(__cpp_raw_strings)
COMPILER_FEATURE_ENTRY(__cpp_ref_qualifiers)
COMPILER_FEATURE_ENTRY(__cpp_rvalue_references)
COMPILER_FEATURE_ENTRY(__cpp_static_assert)
COMPILER_FEATURE_ENTRY(__cpp_threadsafe_static_init)
COMPILER_FEATURE_ENTRY(__cpp_unicode_characters)
COMPILER_FEATURE_ENTRY(__cpp_unicode_literals)
COMPILER_FEATURE_ENTRY(__cpp_user_defined_literals)
COMPILER_FEATURE_ENTRY(__cpp_variadic_templates)
};
static CompilerFeature cxx14[] =
{
COMPILER_FEATURE_ENTRY(__cpp_aggregate_nsdmi)
COMPILER_FEATURE_ENTRY(__cpp_binary_literals)
COMPILER_FEATURE_ENTRY(__cpp_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_decltype_auto)
COMPILER_FEATURE_ENTRY(__cpp_generic_lambdas)
COMPILER_FEATURE_ENTRY(__cpp_init_captures)
COMPILER_FEATURE_ENTRY(__cpp_return_type_deduction)
COMPILER_FEATURE_ENTRY(__cpp_sized_deallocation)
COMPILER_FEATURE_ENTRY(__cpp_variable_templates)
};
static CompilerFeature cxx14lib[] =
{
COMPILER_FEATURE_ENTRY(__cpp_lib_chrono_udls)
COMPILER_FEATURE_ENTRY(__cpp_lib_complex_udls)
COMPILER_FEATURE_ENTRY(__cpp_lib_exchange_function)
COMPILER_FEATURE_ENTRY(__cpp_lib_generic_associative_lookup)
COMPILER_FEATURE_ENTRY(__cpp_lib_integer_sequence)
COMPILER_FEATURE_ENTRY(__cpp_lib_integral_constant_callable)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_final)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_null_pointer)
COMPILER_FEATURE_ENTRY(__cpp_lib_make_reverse_iterator)
COMPILER_FEATURE_ENTRY(__cpp_lib_make_unique)
COMPILER_FEATURE_ENTRY(__cpp_lib_null_iterators)
COMPILER_FEATURE_ENTRY(__cpp_lib_quoted_string_io)
COMPILER_FEATURE_ENTRY(__cpp_lib_result_of_sfinae)
COMPILER_FEATURE_ENTRY(__cpp_lib_robust_nonmodifying_seq_ops)
COMPILER_FEATURE_ENTRY(__cpp_lib_shared_timed_mutex)
COMPILER_FEATURE_ENTRY(__cpp_lib_string_udls)
COMPILER_FEATURE_ENTRY(__cpp_lib_transformation_trait_aliases)
COMPILER_FEATURE_ENTRY(__cpp_lib_transparent_operators)
COMPILER_FEATURE_ENTRY(__cpp_lib_tuple_element_t)
COMPILER_FEATURE_ENTRY(__cpp_lib_tuples_by_type)
};

static CompilerFeature cxx17[] =
{
COMPILER_FEATURE_ENTRY(__cpp_aggregate_bases)
COMPILER_FEATURE_ENTRY(__cpp_aligned_new)
COMPILER_FEATURE_ENTRY(__cpp_capture_star_this)
COMPILER_FEATURE_ENTRY(__cpp_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_deduction_guides)
COMPILER_FEATURE_ENTRY(__cpp_enumerator_attributes)
COMPILER_FEATURE_ENTRY(__cpp_fold_expressions)
COMPILER_FEATURE_ENTRY(__cpp_guaranteed_copy_elision)
COMPILER_FEATURE_ENTRY(__cpp_hex_float)
COMPILER_FEATURE_ENTRY(__cpp_if_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_inheriting_constructors)
COMPILER_FEATURE_ENTRY(__cpp_inline_variables)
COMPILER_FEATURE_ENTRY(__cpp_namespace_attributes)
COMPILER_FEATURE_ENTRY(__cpp_noexcept_function_type)
COMPILER_FEATURE_ENTRY(__cpp_nontype_template_args)
COMPILER_FEATURE_ENTRY(__cpp_nontype_template_parameter_auto)
COMPILER_FEATURE_ENTRY(__cpp_range_based_for)
COMPILER_FEATURE_ENTRY(__cpp_static_assert)
COMPILER_FEATURE_ENTRY(__cpp_structured_bindings)
COMPILER_FEATURE_ENTRY(__cpp_template_template_args)
COMPILER_FEATURE_ENTRY(__cpp_variadic_using)
};
static CompilerFeature cxx17lib[] =
{
COMPILER_FEATURE_ENTRY(__cpp_lib_addressof_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_lib_allocator_traits_is_always_equal)
COMPILER_FEATURE_ENTRY(__cpp_lib_any)
COMPILER_FEATURE_ENTRY(__cpp_lib_apply)
COMPILER_FEATURE_ENTRY(__cpp_lib_array_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_lib_as_const)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_is_always_lock_free)
COMPILER_FEATURE_ENTRY(__cpp_lib_bool_constant)
COMPILER_FEATURE_ENTRY(__cpp_lib_boyer_moore_searcher)
COMPILER_FEATURE_ENTRY(__cpp_lib_byte)
COMPILER_FEATURE_ENTRY(__cpp_lib_chrono)
COMPILER_FEATURE_ENTRY(__cpp_lib_clamp)
COMPILER_FEATURE_ENTRY(__cpp_lib_enable_shared_from_this)
COMPILER_FEATURE_ENTRY(__cpp_lib_execution)
COMPILER_FEATURE_ENTRY(__cpp_lib_filesystem)
COMPILER_FEATURE_ENTRY(__cpp_lib_gcd_lcm)
COMPILER_FEATURE_ENTRY(__cpp_lib_hardware_interference_size)
COMPILER_FEATURE_ENTRY(__cpp_lib_has_unique_object_representations)
COMPILER_FEATURE_ENTRY(__cpp_lib_hypot)
COMPILER_FEATURE_ENTRY(__cpp_lib_incomplete_container_elements)
COMPILER_FEATURE_ENTRY(__cpp_lib_invoke)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_aggregate)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_invocable)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_swappable)
COMPILER_FEATURE_ENTRY(__cpp_lib_launder)
COMPILER_FEATURE_ENTRY(__cpp_lib_logical_traits)
COMPILER_FEATURE_ENTRY(__cpp_lib_make_from_tuple)
COMPILER_FEATURE_ENTRY(__cpp_lib_map_try_emplace)
COMPILER_FEATURE_ENTRY(__cpp_lib_math_special_functions)
COMPILER_FEATURE_ENTRY(__cpp_lib_memory_resource)
COMPILER_FEATURE_ENTRY(__cpp_lib_node_extract)
COMPILER_FEATURE_ENTRY(__cpp_lib_nonmember_container_access)
COMPILER_FEATURE_ENTRY(__cpp_lib_not_fn)
COMPILER_FEATURE_ENTRY(__cpp_lib_optional)
COMPILER_FEATURE_ENTRY(__cpp_lib_parallel_algorithm)
COMPILER_FEATURE_ENTRY(__cpp_lib_raw_memory_algorithms)
COMPILER_FEATURE_ENTRY(__cpp_lib_sample)
COMPILER_FEATURE_ENTRY(__cpp_lib_scoped_lock)
COMPILER_FEATURE_ENTRY(__cpp_lib_shared_mutex)
COMPILER_FEATURE_ENTRY(__cpp_lib_shared_ptr_arrays)
COMPILER_FEATURE_ENTRY(__cpp_lib_shared_ptr_weak_type)
COMPILER_FEATURE_ENTRY(__cpp_lib_string_view)
COMPILER_FEATURE_ENTRY(__cpp_lib_to_chars)
COMPILER_FEATURE_ENTRY(__cpp_lib_transparent_operators)
COMPILER_FEATURE_ENTRY(__cpp_lib_type_trait_variable_templates)
COMPILER_FEATURE_ENTRY(__cpp_lib_uncaught_exceptions)
COMPILER_FEATURE_ENTRY(__cpp_lib_unordered_map_try_emplace)
COMPILER_FEATURE_ENTRY(__cpp_lib_variant)
COMPILER_FEATURE_ENTRY(__cpp_lib_void_t)
};

static CompilerFeature cxx20[] =
{
COMPILER_FEATURE_ENTRY(__cpp_aggregate_paren_init)
COMPILER_FEATURE_ENTRY(__cpp_char8_t)
COMPILER_FEATURE_ENTRY(__cpp_concepts)
COMPILER_FEATURE_ENTRY(__cpp_conditional_explicit)
COMPILER_FEATURE_ENTRY(__cpp_consteval)
COMPILER_FEATURE_ENTRY(__cpp_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_constexpr_dynamic_alloc)
COMPILER_FEATURE_ENTRY(__cpp_constexpr_in_decltype)
COMPILER_FEATURE_ENTRY(__cpp_constinit)
COMPILER_FEATURE_ENTRY(__cpp_deduction_guides)
COMPILER_FEATURE_ENTRY(__cpp_designated_initializers)
COMPILER_FEATURE_ENTRY(__cpp_generic_lambdas)
COMPILER_FEATURE_ENTRY(__cpp_impl_coroutine)
COMPILER_FEATURE_ENTRY(__cpp_impl_destroying_delete)
COMPILER_FEATURE_ENTRY(__cpp_impl_three_way_comparison)
COMPILER_FEATURE_ENTRY(__cpp_init_captures)
COMPILER_FEATURE_ENTRY(__cpp_modules)
COMPILER_FEATURE_ENTRY(__cpp_nontype_template_args)
COMPILER_FEATURE_ENTRY(__cpp_using_enum)
};
static CompilerFeature cxx20lib[] =
{
COMPILER_FEATURE_ENTRY(__cpp_lib_array_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_lib_assume_aligned)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_flag_test)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_float)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_lock_free_type_aliases)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_ref)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_shared_ptr)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_value_initialization)
COMPILER_FEATURE_ENTRY(__cpp_lib_atomic_wait)
COMPILER_FEATURE_ENTRY(__cpp_lib_barrier)
COMPILER_FEATURE_ENTRY(__cpp_lib_bind_front)
COMPILER_FEATURE_ENTRY(__cpp_lib_bit_cast)
COMPILER_FEATURE_ENTRY(__cpp_lib_bitops)
COMPILER_FEATURE_ENTRY(__cpp_lib_bounded_array_traits)
COMPILER_FEATURE_ENTRY(__cpp_lib_char8_t)
COMPILER_FEATURE_ENTRY(__cpp_lib_chrono)
COMPILER_FEATURE_ENTRY(__cpp_lib_concepts)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_algorithms)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_complex)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_dynamic_alloc)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_functional)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_iterator)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_memory)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_numeric)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_string)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_string_view)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_tuple)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_utility)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_vector)
COMPILER_FEATURE_ENTRY(__cpp_lib_coroutine)
COMPILER_FEATURE_ENTRY(__cpp_lib_destroying_delete)
COMPILER_FEATURE_ENTRY(__cpp_lib_endian)
COMPILER_FEATURE_ENTRY(__cpp_lib_erase_if)
COMPILER_FEATURE_ENTRY(__cpp_lib_execution)
COMPILER_FEATURE_ENTRY(__cpp_lib_format)
COMPILER_FEATURE_ENTRY(__cpp_lib_generic_unordered_lookup)
COMPILER_FEATURE_ENTRY(__cpp_lib_int_pow2)
COMPILER_FEATURE_ENTRY(__cpp_lib_integer_comparison_functions)
COMPILER_FEATURE_ENTRY(__cpp_lib_interpolate)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_constant_evaluated)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_layout_compatible)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_nothrow_convertible)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_pointer_interconvertible)
COMPILER_FEATURE_ENTRY(__cpp_lib_jthread)
COMPILER_FEATURE_ENTRY(__cpp_lib_latch)
COMPILER_FEATURE_ENTRY(__cpp_lib_list_remove_return_type)
COMPILER_FEATURE_ENTRY(__cpp_lib_math_constants)
COMPILER_FEATURE_ENTRY(__cpp_lib_polymorphic_allocator)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges)
COMPILER_FEATURE_ENTRY(__cpp_lib_remove_cvref)
COMPILER_FEATURE_ENTRY(__cpp_lib_semaphore)
COMPILER_FEATURE_ENTRY(__cpp_lib_shared_ptr_arrays)
COMPILER_FEATURE_ENTRY(__cpp_lib_shift)
COMPILER_FEATURE_ENTRY(__cpp_lib_smart_ptr_for_overwrite)
COMPILER_FEATURE_ENTRY(__cpp_lib_source_location)
COMPILER_FEATURE_ENTRY(__cpp_lib_span)
COMPILER_FEATURE_ENTRY(__cpp_lib_ssize)
COMPILER_FEATURE_ENTRY(__cpp_lib_starts_ends_with)
COMPILER_FEATURE_ENTRY(__cpp_lib_string_view)
COMPILER_FEATURE_ENTRY(__cpp_lib_syncbuf)
COMPILER_FEATURE_ENTRY(__cpp_lib_three_way_comparison)
COMPILER_FEATURE_ENTRY(__cpp_lib_to_address)
COMPILER_FEATURE_ENTRY(__cpp_lib_to_array)
COMPILER_FEATURE_ENTRY(__cpp_lib_type_identity)
COMPILER_FEATURE_ENTRY(__cpp_lib_unwrap_ref)
};

static CompilerFeature cxx23[] = {
//< Continue to Populate
COMPILER_FEATURE_ENTRY(__cpp_constexpr)
COMPILER_FEATURE_ENTRY(__cpp_explicit_this_parameter)
COMPILER_FEATURE_ENTRY(__cpp_if_consteval)
COMPILER_FEATURE_ENTRY(__cpp_multidimensional_subscript)
COMPILER_FEATURE_ENTRY(__cpp_size_t_suffix)
};
static CompilerFeature cxx23lib[] = {
//< Continue to Populate
COMPILER_FEATURE_ENTRY(__cpp_lib_adaptor_iterator_pair_constructor)
COMPILER_FEATURE_ENTRY(__cpp_lib_allocate_at_least)
COMPILER_FEATURE_ENTRY(__cpp_lib_associative_heterogeneous_erasure)
COMPILER_FEATURE_ENTRY(__cpp_lib_bind_back)
COMPILER_FEATURE_ENTRY(__cpp_lib_byteswap)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_cmath)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_memory)
COMPILER_FEATURE_ENTRY(__cpp_lib_constexpr_typeinfo)
COMPILER_FEATURE_ENTRY(__cpp_lib_containers_ranges)
COMPILER_FEATURE_ENTRY(__cpp_lib_expected)
COMPILER_FEATURE_ENTRY(__cpp_lib_format)
COMPILER_FEATURE_ENTRY(__cpp_lib_invoke_r)
COMPILER_FEATURE_ENTRY(__cpp_lib_is_scoped_enum)
COMPILER_FEATURE_ENTRY(__cpp_lib_move_only_function)
COMPILER_FEATURE_ENTRY(__cpp_lib_optional)
COMPILER_FEATURE_ENTRY(__cpp_lib_out_ptr)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_chunk)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_chunk_by)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_iota)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_join_with)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_slide)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_starts_ends_with)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_to_container)
COMPILER_FEATURE_ENTRY(__cpp_lib_ranges_zip)
COMPILER_FEATURE_ENTRY(__cpp_lib_reference_from_temporary)
COMPILER_FEATURE_ENTRY(__cpp_lib_shift)
COMPILER_FEATURE_ENTRY(__cpp_lib_spanstream)
COMPILER_FEATURE_ENTRY(__cpp_lib_stacktrace)
COMPILER_FEATURE_ENTRY(__cpp_lib_stdatomic_h)
COMPILER_FEATURE_ENTRY(__cpp_lib_string_contains)
COMPILER_FEATURE_ENTRY(__cpp_lib_string_resize_and_overwrite)
COMPILER_FEATURE_ENTRY(__cpp_lib_to_underlying)
COMPILER_FEATURE_ENTRY(__cpp_lib_unreachable)
COMPILER_FEATURE_ENTRY(__cpp_lib_variant)
};

static CompilerFeature attributes[] =
{
COMPILER_ATTRIBUTE_ENTRY(carries_dependency)
COMPILER_ATTRIBUTE_ENTRY(deprecated)
COMPILER_ATTRIBUTE_ENTRY(fallthrough)
COMPILER_ATTRIBUTE_ENTRY(likely)
COMPILER_ATTRIBUTE_ENTRY(maybe_unused)
COMPILER_ATTRIBUTE_ENTRY(nodiscard)
COMPILER_ATTRIBUTE_ENTRY(noreturn)
COMPILER_ATTRIBUTE_ENTRY(no_unique_address)
COMPILER_ATTRIBUTE_ENTRY(unlikely)
};

template<size_t N>
inline void show(char const* title, CompilerFeature (&features)[N])
{
	constexpr static int max_name_length = 44; //< Update if necessary
	std::cout << '\n' << std::left << title << '\n';
	for (const CompilerFeature& x : features)
	{
		std::string value{ (x.value[0] != '_' && x.value[0] != '0') ? x.value : "------_" };
		value.pop_back();
		value.insert(4, 1, '-');
		std::cout << std::left << std::setw(max_name_length) << x.name << " " << value << '\n';
	}
}

int main()
{
	std::cout << "COMPILER ";
#if defined(LJH_COMPILER_MSVC)
	std::cout << "(MSVC " << _MSC_FULL_VER / 10000000 << '.' << _MSC_FULL_VER / 100000 % 100 << '.' << _MSC_FULL_VER % 100000 << ")\n";
#elif defined(LJH_COMPILER_GCC)
	std::cout << "(GCC " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << ")\n";
#elif defined(LJH_COMPILER_APPLE_CLANG)
	std::cout << "(AppleClang " << __clang_major__ << '.' << __clang_minor__ << '.' << __clang_patchlevel__ << ")\n";
#elif defined(LJH_COMPILER_CLANG)
	std::cout << "(Clang " << __clang_major__ << '.' << __clang_minor__ << '.' << __clang_patchlevel__ << ")\n";
#endif

	std::cout << "SYSTEM (" << *ljh::system_info::get_string() << ")\n";

	show("C++ GENERAL", cxx       );
	show("C++11 CORE" , cxx11     );
	show("C++14 CORE" , cxx14     );
	show("C++14 LIB"  , cxx14lib  );
	show("C++17 CORE" , cxx17     );
	show("C++17 LIB"  , cxx17lib  );
	show("C++20 CORE" , cxx20     );
	show("C++20 LIB"  , cxx20lib  );
	show("C++23 CORE" , cxx23     );
	show("C++23 LIB"  , cxx23lib  );
	show("ATTRIBUTES" , attributes);
}