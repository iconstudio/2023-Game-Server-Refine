module;
#include <type_traits>
#include <concepts>
export module Utility.Traits;

export namespace util
{
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

	template<typename T>
	using clean_t = std::remove_cvref_t<T>;

	template <typename T>
	inline constexpr bool is_actual_integral_v = std::_Is_any_of_v<std::remove_cvref_t<T>, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template <typename Special, template <typename...> typename Template>
	struct is_specialization : std::false_type {};

	template<template<typename...> typename Template, typename... Args>
	struct is_specialization<Template<Args...>, typename Template> : std::true_type {};

	template<typename Special, template<typename...> typename Template>
	inline constexpr bool is_specialization_v = is_specialization<Special, Template>::value;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_conjunction = std::conjunction_v<MetaFn<Ts>...>;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_disjunction = std::disjunction_v<MetaFn<Ts>...>;
}
