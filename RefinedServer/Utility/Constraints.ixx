module;
#include <concepts>

export module Utility.Constraints;
import Utility.Traits;

export namespace util
{
	template<typename Derived>
	concept crtp = std::is_class_v<Derived> && std::same_as<Derived, std::remove_cv_t<Derived>>;

	template<typename T>
	concept notvoids = !std::same_as<std::decay_t<clean_t<T>>, void>;

	template<typename... Ts>
	concept integrals = make_conjunction<std::is_integral, clean_t<Ts>...>;

	template<typename... Ts>
	concept floats = make_conjunction<std::is_floating_point, clean_t<Ts>...>;

	template<typename... Ts>
	concept enumerations = make_conjunction<std::is_enum, clean_t<Ts>...>;

	template<typename... Ts>
	concept classes = make_conjunction<std::is_class, clean_t<Ts>...>;

	template<typename T, typename... Args>
	concept invocables = !std::is_abstract_v<clean_t<T>> && std::invocable<T, Args...>;

	template<typename T, typename Result, typename... Args>
	concept r_invocables = !std::is_abstract_v<clean_t<T>> && std::invocable<T, Args...>&& std::is_invocable_r_v<Result, T, Args...>;

	template<typename T>
	concept aggregate_classes = classes<T> && std::is_aggregate_v<clean_t<T>>;

	template<typename T>
	concept nonaggregate_classes = classes<T> && std::is_aggregate_v<clean_t<T>>;

	template<typename T>
	concept nothrow_default_constructibles = std::is_nothrow_default_constructible_v<T>;

	template<typename T, typename... Args>
	concept nothrow_constructibles = std::is_nothrow_constructible_v<T, Args...>;

	template<typename T, typename To>
	concept nothrow_convertibles = std::is_nothrow_convertible_v<T, To>;

	template<typename... Ts>
	concept nothrow_copy_constructibles = std::conjunction_v<std::is_nothrow_copy_constructible<Ts>...>;

	template<typename... Ts>
	concept nothrow_move_constructibles = std::conjunction_v<std::is_nothrow_move_constructible<Ts>...>;

	template<typename... Ts>
	concept nothrow_copy_assignables = std::conjunction_v<std::is_nothrow_copy_assignable<Ts>...>;

	template<typename... Ts>
	concept nothrow_move_assignables = std::conjunction_v<std::is_nothrow_move_assignable<Ts>...>;

	template<typename... Ts>
	concept nothrow_destructibles = std::conjunction_v<std::is_nothrow_destructible<Ts>...>;

	template<typename T, typename... Args>
	concept nothrow_invocables = std::is_nothrow_invocable_v<T, Args...>;

	template<typename T, typename Result, typename... Args>
	concept nothrow_r_invocables = std::is_nothrow_invocable_r_v<Result, T, Args...>;

	template<typename Lhs, typename Rhs>
	concept nothrow_assignables = std::is_nothrow_assignable_v<Lhs, Rhs>;

	template<typename T>
	concept basic_arithmeticals = std::is_arithmetic_v<clean_t<T>>;

	template<typename... Args>
	concept argument_available = 0 < sizeof...(Args);

	template<size_t... Indices>
	concept index_available = 0 < sizeof...(Indices);

	template<typename Ty1, typename Ty2>
	concept similar = !std::same_as<clean_t<Ty1>, clean_t<Ty2>>&& std::common_with<clean_t<Ty1>, clean_t<Ty2>>;

	template <typename T>
	inline constexpr bool is_actual_integral_v = std::_Is_any_of_v<std::remove_cvref_t<T>, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template <typename T>
	concept actual_integral = is_actual_integral_v<T>;

	template <typename Special, template <typename...> typename Template>
	struct is_specialization : std::false_type {};

	template<template<typename...> typename Template, typename... Args>
	struct is_specialization<Template<Args...>, typename Template> : std::true_type {};

	template<typename Special, template<typename...> typename Template>
	inline constexpr bool is_specialization_v = is_specialization<Special, Template>::value;

	//template <typename T, template <typename...> typename>
	//inline constexpr bool is_specialization_v = false;

	//template <template <typename...> typename T, typename... Args>
	//inline constexpr bool is_specialization_v<T<Args...>, T> = true;

	template<typename... Ts>
	concept specializations = is_specialization_v<Ts...>;
}
