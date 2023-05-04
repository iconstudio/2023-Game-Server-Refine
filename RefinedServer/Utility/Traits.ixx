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
	using ::std::constructible_from;
	using ::std::convertible_to;

	template<typename T>
	using clean_t = std::remove_cvref_t<T>;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_conjunction = std::conjunction_v<MetaFn<Ts>...>;

	template<template<typename> typename MetaFn, typename... Ts>
	inline constexpr bool make_disjunction = std::disjunction_v<MetaFn<Ts>...>;
}
