module;
#include <variant>

export module Utility.Monad.Loosen;
import Utility;
import Utility.Constraints;
import Utility.Meta;
export import Utility.Union;

export namespace util
{
	template<typename... Ts>
	class [[nodiscard]] LooseMonad
	{
	public:
		using base_type = StaticUnion<std::integral_constant<size_t, 0>, Ts...>;

		template<size_t Index>
		using element_type = std::tuple_element_t<Index, std::tuple<Ts...>>;
		template<size_t Index>
		using const_element_type = const element_type<Index>;
		template<size_t Index>
		using reference_type = element_type<Index>&;
		template<size_t Index>
		using const_reference_type = const element_type<Index>&;
		template<size_t Index>
		using rvalue_type = element_type<Index>&&;
		template<size_t Index>
		using const_rvalue_type = const element_type<Index>&&;

		constexpr LooseMonad() noexcept
		{}

		constexpr LooseMonad(nullopt_t) noexcept
		{}

		template <typename T>
			requires (meta::included_v<clean_t<T>, Ts...> && !same_as<clean_t<T>, in_place_t> && !same_as<clean_t<T>, in_place_type_t> && !same_as<clean_t<T>, in_place_index_t>)
		explicit constexpr LooseMonad(T&& object) noexcept
			: myStorage(in_place_type<clean_t<T>>, static_cast<T&&>(object))
		{}

		template <size_t Index, typename... Args>
			requires (Index < sizeof...(Ts))
		explicit constexpr
			LooseMonad(in_place_index_t<Index>, Args&&... args)
			noexcept(nothrow_constructibles<base_type, in_place_index_t<Index>, Args...>)
			: myStorage(in_place_index<Index>, static_cast<Args&&>(args)...)
		{}

		template <typename T, typename... Args>
			requires (meta::included_v<T, Ts...>)
		explicit constexpr
			LooseMonad(in_place_type_t<T>, Args&&... args)
			noexcept(nothrow_constructibles<base_type, in_place_type_t<T>, std::integral_constant<size_t, 0>, Args...>)
			: myStorage(in_place_type<T>, std::integral_constant<size_t, 0>{}, static_cast<Args&&>(args)...)
		{}

		constexpr ~LooseMonad() noexcept(nothrow_destructibles<Ts...>) = default;

		template<size_t Index, invocables<reference_type<Index>> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)(declval<reference_type<Index>>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(myStorage.template get<Index>());
			}

			return *this;
		}

		template<size_t Index, invocables<const_reference_type<Index>> Fn>
		inline constexpr
			const LooseMonad&
			if_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const_reference_type<Index>>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(myStorage.template get<Index>());
			}

			return *this;
		}

		template<size_t Index, invocables<rvalue_type<Index>> Fn>
		inline constexpr
			LooseMonad&&
			if_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)(declval<rvalue_type<Index>>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(move(myStorage).template get<Index>());
			}

			return move(*this);
		}

		template<size_t Index, invocables<const_rvalue_type<Index>> Fn>
		inline constexpr
			const LooseMonad&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const_rvalue_type<Index>>())))
		{
			if (myStorage.template has_value<Index>())
			{
				forward<Fn>(action)(move(myStorage).template get<Index>());
			}

			return move(*this);
		}

		template<typename T, invocables<T&> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>())))
		{
			if (myStorage.template has_value<T>())
			{
				forward<Fn>(action)(myStorage.template get<T>());
			}

			return *this;
		}

		template<typename T, invocables<const T&> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>())))
		{
			if (myStorage.template has_value<T>())
			{
				forward<Fn>(action)(myStorage.template get<T>());
			}

			return *this;
		}

		template<typename T, invocables<T&&> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>())))
		{
			if (myStorage.template has_value<T>())
			{
				forward<Fn>(action)(move(myStorage).template get<T>());
			}

			return move(*this);
		}

		template<typename T, invocables<const T&&> Fn>
		inline constexpr
			LooseMonad&
			if_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>())))
		{
			if (myStorage.template has_value<T>())
			{
				forward<Fn>(action)(move(myStorage).template get<T>());
			}

			return move(*this);
		}

		template<size_t Index, invocables<reference_type<Index>> Fn>
		inline constexpr
			monad_result_t<Fn, reference_type<Index>>
			and_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)(declval<reference_type<Index>>())))
		{
			static_assert(!same_as<monad_result_t<Fn, reference_type<Index>>, void>, "Monadic result cannot be void.");

			if (myStorage.template has_value<Index>())
			{
				return forward<Fn>(action)(myStorage.template get<Index>());
			}
			else
			{
				return monad_result_t<Fn, reference_type<Index>>{ nullopt };
			}
		}

		template<size_t Index, invocables<const_reference_type<Index>> Fn>
		inline constexpr
			monad_result_t<Fn, const_reference_type<Index>>
			and_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const_reference_type<Index>>())))
		{
			static_assert(!same_as<monad_result_t<Fn, const_reference_type<Index>>, void>, "Monadic result cannot be void.");

			if (myStorage.template has_value<Index>())
			{
				return forward<Fn>(action)(myStorage.template get<Index>());
			}
			else
			{
				return monad_result_t<Fn, const_reference_type<Index>>{ nullopt };
			}
		}

		template<size_t Index, invocables<rvalue_type<Index>> Fn>
		inline constexpr
			monad_result_t<Fn, rvalue_type<Index>>
			and_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)(declval<rvalue_type<Index>>())))
		{
			static_assert(!same_as<monad_result_t<Fn, rvalue_type<Index>>, void>, "Monadic result cannot be void.");

			if (myStorage.template has_value<Index>())
			{
				return forward<Fn>(action)(move(myStorage).template get<Index>());
			}
			else
			{
				return monad_result_t<Fn, rvalue_type<Index>>{ nullopt };
			}
		}

		template<size_t Index, invocables<const_rvalue_type<Index>> Fn>
		inline constexpr
			monad_result_t<Fn, const_rvalue_type<Index>>
			and_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const_rvalue_type<Index>>())))
		{
			static_assert(!same_as<monad_result_t<Fn, const_rvalue_type<Index>>, void>, "Monadic result cannot be void.");

			if (myStorage.template has_value<Index>())
			{
				return forward<Fn>(action)(move(myStorage).template get<Index>());
			}
			else
			{
				return monad_result_t<Fn, const_rvalue_type<Index>>{ nullopt };
			}
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			LooseMonad&
			else_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!has_value<Index>())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			const LooseMonad&
			else_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!has_value<Index>())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			LooseMonad&&
			else_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!has_value<Index>())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			const LooseMonad&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!has_value<Index>())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!is_same_v<fwd_result_t, void>, "Result cannot be void.");

			if (!has_value<Index>())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!is_same_v<fwd_result_t, void>, "Result cannot be void.");

			if (!has_value<Index>())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!is_same_v<fwd_result_t, void>, "Result cannot be void.");

			if (!has_value<Index>())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<size_t Index, invocables Fn>
		inline constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!is_same_v<fwd_result_t, void>, "Result cannot be void.");

			if (!has_value<Index>())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
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
			requires meta::included_range_v<T, base_type>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			return myStorage.template get<T>();
		}

		template <typename T>
			requires meta::included_range_v<T, base_type>
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			return myStorage.template get<T>();
		}

		template <typename T>
			requires meta::included_range_v<T, base_type>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			return move(myStorage).template get<T>();
		}

		template <typename T>
			requires meta::included_range_v<T, base_type>
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

		template <typename T>
			requires meta::included_range_v<T, base_type>
		constexpr bool has_value() const noexcept
		{
			return myStorage.template has_value<T>();
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

	template<typename T, typename E>
	using Expected = LooseMonad<T, E>;
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
		: integral_constant<size_t, 1 + sizeof...(Ts)>
	{};

	template<size_t Index, typename... Ts>
	constexpr decltype(auto)
		get(util::LooseMonad<Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, typename... Ts>
	constexpr decltype(auto)
		get(const util::LooseMonad<Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, typename... Ts>
	constexpr decltype(auto)
		get(util::LooseMonad<Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<size_t Index, typename... Ts>
	constexpr decltype(auto)
		get(const util::LooseMonad<Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<typename T, typename... Ts>
	constexpr decltype(auto)
		get(util::LooseMonad<Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, typename... Ts>
	constexpr decltype(auto)
		get(const util::LooseMonad<Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, typename... Ts>
	constexpr decltype(auto)
		get(util::LooseMonad<Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}

	template<typename T, typename... Ts>
	constexpr decltype(auto)
		get(const util::LooseMonad<Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}
}

namespace util
{
#if false
	constexpr void do_something() noexcept {}

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
		//constexpr int get_d3_i32 = d3.get<int>();
		constexpr float get_d3_f32 = d3.get<float>();
		//constexpr double get_d3_f64 = d3.get<double>();
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

		constexpr auto& fna1 = a1.if_then<0>([](const int& v) {
			do_something();
		});

		constexpr auto& fnb1 = b1.if_then<0>([](const int& v) {
			do_something();
		});
	}
#endif
}
