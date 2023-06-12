export module Utility.Reflection;
import <string>;
import <string_view>;
import Utility.Constraints;
import Utility.Meta;

export namespace reflex
{
	struct tag_parent { constexpr tag_parent() noexcept = default; };
	struct tag_child { constexpr tag_parent() noexcept = default; };

	template<typename T, size_t ID, std::string_view Name>
	struct make_reflection
	{

	};

	template<typename T>
	struct reflect_traits
	{
		using type = T;

		using parents = meta::MetaList<>;
		using children = meta::MetaList<>;

		static constexpr size_t type_id = 0;
		static constexpr std::string_view name = "Type";
	};

	template<typename T>
	struct reflect_traits<const T> : reflect_traits<T>
	{};

	template<typename T>
	struct reflect_traits<volatile T> : reflect_traits<T>
	{};

	template<typename T>
	struct reflect_traits<const volatile T> : reflect_traits<T>
	{};

	template<typename T>
	using parents_t = typename reflect_traits<T>::template parents;

	template<typename T>
	using children_t = typename reflect_traits<T>::template children;

	template<typename T, size_t I>
	using parent_t = meta::at<parents_t<T>, I>;

	template<typename T, size_t I>
	using child_t = meta::at<children_t<T>, I>;

	template<typename T>
	inline constexpr size_t type_id = reflect_traits<T>::template type_id;

	template<typename T>
	inline constexpr std::string_view name = reflect_traits<T>::template name;

	template<typename T, typename U>
	consteval bool operator==(reflect_traits<T>, reflect_traits<U>) noexcept
	{
		return type_id<T> == type_id<U>;
	}

	void test()
	{
		using ::std::literals;

		constexpr make_reflection<int, 0, "int"sv> aa();
	}
}
