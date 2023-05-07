module;
#include <variant>

export module Utility.Monad.Loosen;
import Utility;
import Utility.Constraints;
import Utility.Meta;
export import Utility.Union;

export namespace util
{
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

	template<typename... Ts>
	class [[nodiscard]] LooseMonad
	{
	public:
		using base_type = StaticUnion<0, Ts...>;

		template<size_t Index>
		using element_type = std::variant_alternative_t<Index, base_type>;

		constexpr LooseMonad() noexcept
		{}

		template <size_t Place, typename... Args>
		constexpr
			LooseMonad(in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<base_type, std::in_place_index_t<Place>, Args...>)
			: myStorage(in_place_index<Place>, static_cast<Args&&>(args)...)
		{}

		template <typename T, typename... Args>
		constexpr
			LooseMonad(in_place_type_t<T>, Args&&... args)
			noexcept(nothrow_constructibles<base_type, std::in_place_type_t<T>, std::integral_constant<size_t, 0>, Args...>)
			: myStorage(in_place_type<T>, std::integral_constant<size_t, 0>{}, static_cast<Args&&>(args)...)
		{}

		constexpr ~LooseMonad() noexcept(nothrow_destructibles<Ts...>) = default;

		template<size_t Index, invocables<element_type<Index>&> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)(declval<element_type<Index>&>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(myStorage.template get<Index>());
			}

			return *this;
		}

		template<size_t Index, invocables<const element_type<Index>&> Fn>
		inline constexpr
			const LooseMonad&
			if_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const element_type<Index>&>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(myStorage.template get<Index>());
			}

			return *this;
		}

		template<size_t Index, invocables<element_type<Index>&&> Fn>
		inline constexpr
			LooseMonad&&
			if_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)(declval<element_type<Index>&&>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(move(myStorage).template get<Index>());
			}

			return move(*this);
		}

		template<size_t Index, invocables<const element_type<Index>&&> Fn>
		inline constexpr
			const LooseMonad&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const element_type<Index>&&>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(move(myStorage).template get<Index>());
			}

			return move(*this);
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			return myStorage.template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			return myStorage.template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			return move(myStorage).template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			return move(myStorage).template get<Index>();
		}

		template <typename T>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			return myStorage.template get<T>();
		}

		template <typename T>
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			return myStorage.template get<T>();
		}

		template <typename T>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			return move(myStorage).template get<T>();
		}

		template <typename T>
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			return move(myStorage).template get<T>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		constexpr bool has_value() const noexcept
		{
			return myStorage.template has_value<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		constexpr bool is_valueless() const noexcept
		{
			return myStorage.template is_valueless<Index>();
		}

	private:
		base_type myStorage;
	};

}

export namespace std
{
	template<size_t Index, typename... Ts>
	struct variant_alternative<Index, util::LooseMonad<Ts...>>
	{
		static_assert(Index < sizeof...(Ts), "Loosen index out of bounds.");

		using type = meta::at<util::LooseMonad<Ts...>, Index>;
	};

	template<typename... Ts>
	struct variant_size<util::LooseMonad<Ts...>>
		: integral_constant<size_t, sizeof...(Ts)>
	{};
}

namespace util
{
	void do_something() noexcept {}

	constexpr void test_loose() noexcept
	{
		const LooseMonad<int, float> a0{};
		const LooseMonad<int, float> b0{ std::in_place_index<0>, 1 };
		const LooseMonad<int, float> c0{ std::in_place_index<1>, 1.0f };

		constexpr LooseMonad<int, float> a1{};
		constexpr LooseMonad<int, float> b1{ std::in_place_index<0>, 1 };
		constexpr LooseMonad<int, float> c1{ std::in_place_index<1>, 1.0f };

		const LooseMonad<int, float> b2{ std::in_place_type<int>, 500 };
		const LooseMonad<int, float> c2{ std::in_place_type<float>, 500.0f };

		constexpr LooseMonad<int, float> b3{ std::in_place_type<int>, 500 };
		constexpr LooseMonad<int, float> c3{ std::in_place_type<float>, 500.0f };

		const LooseMonad<int, float, float> d2{ std::in_place_index<1>, 500.0f };
		const LooseMonad<int, float, int> e2{ std::in_place_index<2>, 500 };
		const LooseMonad<int, float, float> f2{ std::in_place_type<float>, 500.0f };
		const LooseMonad<int, float, int> g2{ std::in_place_type<float>, 500.0f };

		constexpr LooseMonad<int, float, float> d3{ std::in_place_index<1>, 500.0f };
		constexpr bool has_d3_0 = d3.has_value<0>();
		constexpr bool has_d3_1 = d3.has_value<1>();
		constexpr bool has_d3_2 = d3.has_value<2>();
		constexpr bool less_d3_0 = d3.is_valueless<0>();
		constexpr bool less_d3_1 = d3.is_valueless<1>();
		constexpr bool less_d3_2 = d3.is_valueless<2>();
		constexpr LooseMonad<int, float, int> e3{ std::in_place_index<2>, 500 };
		constexpr bool has_e3_0 = e3.has_value<0>();
		constexpr bool has_e3_1 = e3.has_value<1>();
		constexpr bool has_e3_2 = e3.has_value<2>();
		constexpr bool less_e3_0 = e3.is_valueless<0>();
		constexpr bool less_e3_1 = e3.is_valueless<1>();
		constexpr bool less_e3_2 = e3.is_valueless<2>();
		constexpr auto get_e3_2 = e3.get<2>();
		constexpr LooseMonad<int, float, float> f3{ std::in_place_type<float>, 500.0f };
		//constexpr auto get_f3_0 = f3.get<0>();
		constexpr auto get_f3_1 = f3.get<1>();
		//constexpr auto get_f3_2 = f3.get<2>();
		constexpr LooseMonad<int, float, int> g3{ std::in_place_type<float>, 500.0f };

		constexpr size_t sz0 = sizeof(LooseMonad<int>);
		constexpr size_t sz1 = sizeof(LooseMonad<int, float>);
		constexpr size_t sz2 = sizeof(LooseMonad<int, float>);
		constexpr size_t sz3 = sizeof(LooseMonad<int, float, float>);
		constexpr size_t sz4 = sizeof(LooseMonad<int, float, int>);

		constexpr auto& fna1 = a1.if_then<0>([](const int&) {
			do_something();
		});

	}
}
