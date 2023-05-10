module;
#include <utility>
export module Utility.Traits;

export namespace util
{
	using ::std::add_const;
	using ::std::add_const_t;
	using ::std::add_volatile;
	using ::std::add_volatile_t;
	using ::std::add_cv;
	using ::std::add_cv_t;
	using ::std::add_pointer;
	using ::std::add_pointer_t;
	using ::std::add_lvalue_reference;
	using ::std::add_lvalue_reference_t;
	using ::std::add_rvalue_reference;
	using ::std::add_rvalue_reference_t;

	using ::std::remove_const;
	using ::std::remove_const_t;
	using ::std::remove_volatile;
	using ::std::remove_volatile_t;
	using ::std::remove_cv;
	using ::std::remove_cv_t;
	using ::std::remove_pointer;
	using ::std::remove_pointer_t;
	using ::std::remove_extent;
	using ::std::remove_extent_t;
	using ::std::remove_all_extents;
	using ::std::remove_all_extents_t;
	using ::std::remove_reference;
	using ::std::remove_reference_t;
	using ::std::remove_cvref;
	using ::std::remove_cvref_t;

	using ::std::negation;
	using ::std::negation_v;
	using ::std::conditional;
	using ::std::conditional_t;
	using ::std::enable_if;
	using ::std::enable_if_t;
	using ::std::void_t;

	using ::std::integral_constant;
	using ::std::true_type;
	using ::std::false_type;

	using ::std::is_same;
	using ::std::is_same_v;

	using ::std::same_as;
	using ::std::copyable;
	using ::std::movable;
	using ::std::default_initializable;
	using ::std::copy_constructible;
	using ::std::move_constructible;
	using ::std::is_copy_assignable_v;
	using ::std::is_move_assignable_v;
	using ::std::constructible_from;
	using ::std::convertible_to;

	using ::std::in_place_t;
	using ::std::in_place;
	using ::std::in_place_type_t;
	using ::std::in_place_type;
	using ::std::in_place_index_t;
	using ::std::in_place_index;

	template<typename T>
	inline constexpr bool always_false = false;

	template <size_t... Indices>
	struct in_place_indices_t
	{
		explicit constexpr in_place_indices_t() noexcept = default;
		constexpr ~in_place_indices_t() noexcept = default;

		explicit constexpr in_place_indices_t(std::index_sequence<Indices...>) noexcept
		{}
	};

	template <size_t... Indices>
	inline constexpr in_place_indices_t<Indices...> in_place_indices{};

	template<typename T>
	using clean_t = std::remove_cvref_t<std::remove_all_extents_t<T>>;

	template <typename T>
	inline constexpr bool is_actual_integral_v = std::_Is_any_of_v<std::remove_cvref_t<T>, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template <typename Special, template <typename...> typename Template>
	struct is_specialization : std::false_type
	{};

	template<template<typename...> typename Template, typename... Args>
	struct is_specialization<Template<Args...>, typename Template> : std::true_type
	{};

	template<typename Special, template<typename...> typename Template>
	inline constexpr bool is_specialization_v = is_specialization<Special, Template>::value;

	template <typename Special, template <size_t...> typename Template>
	struct is_indexed : std::false_type
	{};

	template<template<size_t...> typename Template, size_t... Indices>
	struct is_indexed<Template<Indices...>, typename Template> : std::true_type
	{};

	template<typename Special, template<size_t...> typename Template>
	inline constexpr bool is_indexed_v = is_indexed<Special, Template>::value;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_conjunction = std::conjunction_v<MetaFn<Ts>...>;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_disjunction = std::disjunction_v<MetaFn<Ts>...>;

	template<typename Fn, typename... Args>
	using monad_result_t = clean_t<std::invoke_result_t<Fn, Args...>>;

	template<typename T>
	using make_lvalue_t = conditional_t<same_as<clean_t<T>, void>, void, add_lvalue_reference_t<remove_const_t<T>>>;

	template<typename T>
	using make_clvalue_t = conditional_t<same_as<clean_t<T>, void>, void, add_lvalue_reference_t<add_const_t<T>>>;

	template<typename T>
	using make_rvalue_t = conditional_t<same_as<clean_t<T>, void>, void, add_rvalue_reference_t<remove_const_t<T>>>;

	template<typename T>
	using make_crvalue_t = conditional_t<same_as<clean_t<T>, void>, void, add_rvalue_reference_t<add_const_t<T>>>;
}
