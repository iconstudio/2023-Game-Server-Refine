export module Utility.Reflection;
import <compare>;
import <string>;
import <string_view>;
import Utility.Constraints;
import Utility.Meta;
import Utility.String.Literal;

export namespace util
{
	template <typename Char, size_t N>
	struct basic_fixed_string
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval basic_fixed_string(const Char* const& buffer) noexcept
		{
			for (size_t i = 0; i < N - 1; ++i)
			{
				const Char& elem = buffer[i];
				intBuffer[i] = elem;
			}
		}

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(intBuffer, N - 1);
		}

		Char intBuffer[N - 1];
	};

	template <typename Char>
	struct basic_fixed_string<Char, 1>
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval basic_fixed_string(const Char* const& buffer) noexcept
		{
			intBuffer = buffer[0];
		}

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(intBuffer, 1);
		}

		Char intBuffer;
	};

	template <typename Char>
	struct basic_fixed_string<Char, 0>
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(nullptr, 0);
		}
	};

	template <typename Char, size_t N>
	basic_fixed_string(const Char(&str)[N]) -> basic_fixed_string<Char, N>;

	template <size_t N>
	using fixed_string = basic_fixed_string<char, N>;

	template <size_t N>
	using fixed_u8string = basic_fixed_string<char8_t, N>;

	template <size_t N>
	using fixed_u16string = basic_fixed_string<char16_t, N>;

	template <size_t N>
	using fixed_u32string = basic_fixed_string<char32_t, N>;

	template <size_t N>
	using fixed_wstring = basic_fixed_string<wchar_t, N>;
}

namespace reflex
{
	export using ::util::basic_fixed_string;
	export using ::util::fixed_string;

	export template<typename... Ts> using tag_types = meta::MetaList<Ts...>;

	template<typename Tag>
	struct tag_traits
	{
		using type = meta::MetaList<void>;
	};

	template<typename... Ts>
	struct tag_traits<meta::MetaList<Ts...>>
	{
		using type = meta::MetaList<Ts...>;
	};

	template<typename Tag>
	using tag_traits_t = typename tag_traits<Tag>::type;

	template<typename T
		, size_t ID, fixed_string Name
		, typename Parents = void
		, typename Children = void>
	struct make_reflection
	{
		consteval make_reflection() noexcept = default;
		constexpr ~make_reflection() noexcept = default;

		using type = T;
		using parents = tag_traits_t<Parents>;
		using children = tag_traits_t<Children>;

		static constexpr size_t type_id = ID;
		static constexpr size_t size = sizeof(T);
		static constexpr std::string_view name = std::string_view(Name);
	};

	template<size_t ID, fixed_string Name
		, typename Parents
		, typename Children>
	struct make_reflection<void, ID, Name, Parents, Children>
	{
		consteval make_reflection() noexcept = default;
		constexpr ~make_reflection() noexcept = default;

		using type = void;
		using parents = tag_traits_t<Parents>;
		using children = tag_traits_t<Children>;

		static constexpr size_t type_id = ID;
		static constexpr size_t size = 0;
		static constexpr std::string_view name = std::string_view(Name);
	};

	template<typename T>
	struct reflect_traits;

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
	struct reflect_traits<T[]> : reflect_traits<T*>
	{};

	export constexpr size_t LastTypeIndex = 27;

#define MAKE_PARENTS(...) \
	::reflex::tag_types<__VA_ARGS__>

#define MAKE_CHILDREN(...) \
	::reflex::tag_types<__VA_ARGS__>

#define MAKE_REFLECTION(type, id, name, parents, children) \
	template<> \
	struct ::reflex::reflect_traits<type> final \
		: ::reflex::make_reflection<type, id, name, parents, children> \
	{}

#define MAKE_REFLECTION_NO_CHILDREN(type, id, name, parents) \
	MAKE_REFLECTION(type, id, name, parents, void)

#define MAKE_REFLECTION_NO_PARENTS(type, id, name, children) \
	MAKE_REFLECTION(type, id, name, void, children)

#define MAKE_REFLECTION_NO_HIERACHY(type, id, name) \
	MAKE_REFLECTION(type, id, name, void, void)
}

export MAKE_REFLECTION_NO_HIERACHY(nullptr_t, 0, "std::nullptr_t");
export MAKE_REFLECTION_NO_HIERACHY(void, 1, "void");
export MAKE_REFLECTION_NO_HIERACHY(bool, 2, "bool");
export MAKE_REFLECTION_NO_HIERACHY(char, 3, "char");
export MAKE_REFLECTION_NO_HIERACHY(signed char, 4, "signed char");
export MAKE_REFLECTION_NO_HIERACHY(unsigned char, 5, "unsigned char");
export MAKE_REFLECTION_NO_HIERACHY(wchar_t, 6, "wide char");
export MAKE_REFLECTION_NO_HIERACHY(char8_t, 7, "utf-8");
export MAKE_REFLECTION_NO_HIERACHY(char16_t, 8, "utf-16");
export MAKE_REFLECTION_NO_HIERACHY(char32_t, 9, "utf-32");
export MAKE_REFLECTION_NO_HIERACHY(short, 10, "short");
export MAKE_REFLECTION_NO_HIERACHY(unsigned short, 11, "unsigned short");
export MAKE_REFLECTION_NO_HIERACHY(int, 12, "int");
export MAKE_REFLECTION_NO_HIERACHY(unsigned int, 13, "unsigned int");
export MAKE_REFLECTION_NO_HIERACHY(long, 14, "long");
export MAKE_REFLECTION_NO_HIERACHY(unsigned long, 15, "unsigned long");
export MAKE_REFLECTION_NO_HIERACHY(long long, 16, "long long");
export MAKE_REFLECTION_NO_HIERACHY(unsigned long long, 17, "unsigned long long");
export MAKE_REFLECTION_NO_HIERACHY(float, 18, "float");
export MAKE_REFLECTION_NO_HIERACHY(double, 19, "double");
export MAKE_REFLECTION_NO_HIERACHY(long double, 20, "long double");
export MAKE_REFLECTION_NO_HIERACHY(void*, 21, "void*");
export MAKE_REFLECTION_NO_HIERACHY(std::string, 22, "string");
export MAKE_REFLECTION_NO_HIERACHY(std::wstring, 23, "wide string");
export MAKE_REFLECTION_NO_HIERACHY(std::u8string, 24, "utf-8 string");
export MAKE_REFLECTION_NO_HIERACHY(std::u16string, 25, "utf-16 string");
export MAKE_REFLECTION_NO_HIERACHY(std::u32string, 26, "utf-32 string");
export MAKE_REFLECTION_NO_HIERACHY(std::string_view, ::reflex::LastTypeIndex, "string_view");

export namespace reflex
{
	template<typename T>
	using reflect_t = typename reflect_traits<T>::template type;

	template<typename T>
	using parents = typename reflect_traits<T>::template parents;

	template<typename T>
	using children = typename reflect_traits<T>::template children;

	template<typename T, size_t I>
	using parent = meta::at<parents<T>, I>;

	template<typename T, size_t I>
	using child = meta::at<children<T>, I>;

	template<typename T>
	inline constexpr size_t type_id = reflect_traits<T>::template type_id;

	template<typename T>
	inline constexpr std::string_view name = reflect_traits<T>::template name;

	template<typename T, typename U>
	consteval bool operator==(reflect_traits<T>, reflect_traits<U>) noexcept
	{
		return type_id<T> == type_id<U>;
	}
}

#pragma warning(push, 1)
namespace reflex::test
{
#if false
{
	void test()
	{
		using namespace ::std::literals;

		constexpr fixed_string aaa{ "342" };
		const fixed_string bbb{ "342" };
		fixed_string ccc{ "342" };

		using test_refl_int = make_reflection<int, 0, "int", tag_types<bool, long, double>>;
		constexpr test_refl_int::type a = 0;
		constexpr test_refl_int::parents b{};
		constexpr test_refl_int::children c{};

		using par1 = typename meta::at<test_refl_int::parents, 0>;
		static_assert(util::same_as<par1, bool>);

		using par2 = typename meta::at<test_refl_int::parents, 1>;
		static_assert(util::same_as<par2, long>);

		using par3 = typename meta::at<test_refl_int::parents, 2>;
		static_assert(util::same_as<par3, double>);

		constexpr auto namei = test_refl_int::name;
		constexpr char name_1 = namei[0];
		constexpr auto id = test_refl_int::type_id;

		constexpr reflect_t<int> aaa_2 = 0;
		constexpr parents<int> bbb_2{};
		constexpr children<int> ccc_2{};
		constexpr auto nameii = name<int>;
		constexpr char name_2 = nameii[0];
		constexpr auto id_2 = type_id<int>;
	}
#endif
}
#pragma warning(pop)
