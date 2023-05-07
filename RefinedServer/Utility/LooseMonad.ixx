module;
#include <variant>
#include <expected>

export module Utility.Monad.Loosen;
import Utility;
import Utility.Constraints;
import Utility.Meta;

namespace util
{
	template <size_t Place, typename... Ts>
	class __variant_storage
	{
		constexpr __variant_storage() noexcept {}
		constexpr ~__variant_storage() noexcept {}
	};

	template <size_t Place, typename Fty, typename... Rty>
	class __variant_storage<Place, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, std::in_place_t>, "Fty must not be std::in_place_t.");
		static_assert(!is_specialization_v<Fty, std::in_place_type_t>, "Fty must not be std::in_place_type_t.");
		static_assert(!is_indexed_v<Fty, std::in_place_index_t>, "Fty must not be std::in_place_type_t<.");

		using type = __variant_storage<Place, Fty, Rty...>;
		using value_type = std::remove_const_t<Fty>;
		using under_type = __variant_storage<Place + 1, Rty...>;

		static inline constexpr size_t mySize = 1 + sizeof...(Rty);
		static inline constexpr size_t myPlace = Place;

		// no initialization (no active member)
		constexpr __variant_storage() noexcept
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr __variant_storage(std::in_place_t, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr __variant_storage(std::in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: __variant_storage(std::in_place, static_cast<Args&&>(args)...)
		{}

		// Recursively find the place onto Tail
		template <size_t Target, typename... Args>
			requires (Target != Place)
		constexpr __variant_storage(std::in_place_index_t<Target>, Args&&... args)
			noexcept(nothrow_constructibles<under_type, std::in_place_index_t<Target>, Args...>)
			: _Tail(std::in_place_index<Target>, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename T, size_t Index, typename... Args>
			requires (same_as<clean_t<T>, Fty>)
		constexpr __variant_storage(std::in_place_type_t<T>, std::integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: __variant_storage(std::in_place, static_cast<Args&&>(args)...)
		{}

		// Find the specified type
		template <typename T, size_t Index, typename... Args>
			requires (!same_as<clean_t<T>, Fty>&& Index <= mySize)
		constexpr __variant_storage(std::in_place_type_t<T>, std::integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: _Tail(std::in_place_type<T>, std::integral_constant<size_t, Index + 1>{}, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr explicit __variant_storage(std::integral_constant<size_t, 0>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Recursively seek the index within Tail
		template <size_t Index, typename... Args>
			requires (Index != Place)
		constexpr explicit __variant_storage(std::integral_constant<size_t, Index>, Args&&... _Args)
			noexcept(nothrow_constructibles<under_type, std::integral_constant<size_t, Index - 1>, Args...>)
			: _Tail(std::integral_constant<size_t, Index - 1>{}, static_cast<Args&&>(_Args)...)
			, isExtended(false)
		{}

		constexpr ~__variant_storage()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires (!make_disjunction<std::is_trivially_destructible, Fty, Rty...>)
		{}

		constexpr ~__variant_storage()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires (make_conjunction<std::is_trivially_destructible, Fty, Rty...>)
		{}

		[[nodiscard]]
		constexpr Fty& get() &
			noexcept(nothrow_copy_constructibles<Fty>)
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr const Fty& get() const&
			noexcept(nothrow_copy_constructibles<const Fty>)
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr Fty&& get() &&
			noexcept(nothrow_move_constructibles<Fty>)
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr const Fty&& get() const&&
			noexcept(nothrow_move_constructibles<const Fty>)
		{
			return std::move(myValue);
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr
			Fty&
			get() &
			noexcept(noexcept((*this).get()))
		{
			return get();
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr
			const Fty&
			get() const&
			noexcept(noexcept((*this).get()))
		{
			return get();
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr
			Fty&&
			get() &&
			noexcept(noexcept(static_cast<__variant_storage&&>(*this).get()))
		{
			return static_cast<Fty&&>(get());
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr
			const Fty&&
			get() const&&
			noexcept(noexcept(static_cast<const __variant_storage&&>(*this).get()))
		{
			return static_cast<const Fty&&>(get());
		}

		template <size_t Index>
			requires (Place < Index)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			if (isExtended)
			{
				return _Tail.template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index != Place)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			if (isExtended)
			{
				return _Tail.template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index != Place)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			if (isExtended)
			{
				return static_cast<under_type&&>(_Tail).template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index != Place)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			if (isExtended)
			{
				return static_cast<const under_type&&>(_Tail).template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		[[nodiscard]]
		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		template <size_t Index>
			requires (Place < Index)
		[[nodiscard]]
		constexpr bool has_value() const noexcept
		{
			if (isExtended)
			{
				try
				{
					return _Tail.template has_value<Index>();
				}
				catch (...)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		template <size_t Index>
			requires (Index == Place)
		[[nodiscard]]
		constexpr bool is_valueless() const noexcept
		{
			return !hasValue;
		}

		template <size_t Index>
			requires (Place < Index)
		[[nodiscard]]
		constexpr bool is_valueless() const noexcept
		{
			if (isExtended)
			{
				try
				{
					return _Tail.template is_valueless<Index>();
				}
				catch (...)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}

		__variant_storage(__variant_storage&&) = default;
		__variant_storage(const __variant_storage&) = default;
		__variant_storage& operator=(__variant_storage&&) = default;
		__variant_storage& operator=(const __variant_storage&) = default;

	private:
		friend class under_type;

		union
		{
			value_type myValue;
			under_type _Tail;
		};
		bool hasValue = false;
		bool isExtended = false;
	};
}

export namespace util
{
	using ::std::in_place_t;
	using ::std::in_place;
	using ::std::in_place_type_t;
	using ::std::in_place_type;
	using ::std::in_place_index_t;
	using ::std::in_place_index;

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
		using value_type = __variant_storage<0, Ts...>;

		constexpr LooseMonad() noexcept
		{}

		template <size_t Place, typename... Args>
		constexpr explicit
			LooseMonad(in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<value_type, std::in_place_index_t<Place>, Args...>)
			: myStorage(in_place_index<Place>, static_cast<Args&&>(args)...)
		{}

		template <typename T, typename... Args>
		constexpr explicit
			LooseMonad(in_place_type_t<T>, Args&&... args)
			noexcept(nothrow_constructibles<value_type, std::in_place_type_t<T>, std::integral_constant<size_t, 0>, Args...>)
			: myStorage(in_place_type<T>, std::integral_constant<size_t, 0>{}, static_cast<Args&&>(args)...)
		{}

		constexpr ~LooseMonad() noexcept(nothrow_destructibles<Ts...>) = default;

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() &
			noexcept(noexcept(myStorage.template get<Index>()))
		{
			return myStorage.template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
			noexcept(noexcept(myStorage.template get<Index>()))
		{
			return myStorage.template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() &&
			noexcept(noexcept(move(myStorage).template get<Index>()))
		{
			return move(myStorage).template get<Index>();
		}

		template <size_t Index>
			requires (Index < sizeof...(Ts))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
			noexcept(noexcept(move(myStorage).template get<Index>()))
		{
			return move(myStorage).template get<Index>();
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
		value_type myStorage;
	};
}

namespace util
{
	void test_loose() noexcept
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
		constexpr LooseMonad<int, float, float> f3{ std::in_place_type<float>, 500.0f };
		constexpr auto get_f3_0 = f3.get<0>();
		constexpr auto get_f3_1 = f3.get<1>();
		constexpr auto get_f3_2 = f3.get<2>();
		constexpr LooseMonad<int, float, int> g3{ std::in_place_type<float>, 500.0f };

		constexpr size_t sz0 = sizeof(LooseMonad<int>);
		constexpr size_t sz1 = sizeof(LooseMonad<int, float>);
		constexpr size_t sz2 = sizeof(LooseMonad<int, float>);
		constexpr size_t sz3 = sizeof(LooseMonad<int, float, float>);
		constexpr size_t sz4 = sizeof(LooseMonad<int, float, int>);

	}
}
