module;
#include <variant>

export module Utility.Union;
import Utility;
import Utility.Constraints;
import Utility.Meta;

export namespace util
{
	using ::std::in_place_t;
	using ::std::in_place;
	using ::std::in_place_type_t;
	using ::std::in_place_type;
	using ::std::in_place_index_t;
	using ::std::in_place_index;

	template <typename... Ts>
	class StaticUnion;

	template <size_t Place>
	class StaticUnion<std::integral_constant<size_t, Place>>
	{};

	template <size_t Place, typename Fty, typename... Rty>
	class StaticUnion<std::integral_constant<size_t, Place>, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, nullopt_t>, "Fty must not be nullopt_t.");
		static_assert(!same_as<Fty, std::in_place_t>, "Fty must not be std::in_place_t.");
		static_assert(!is_specialization_v<Fty, std::in_place_type_t>, "Fty must not be std::in_place_type_t.");
		static_assert(!is_indexed_v<Fty, std::in_place_index_t>, "Fty must not be std::in_place_type_t<.");

		using type = StaticUnion<std::integral_constant<size_t, Place>, Fty, Rty...>;
		using value_type = std::remove_const_t<Fty>;
		using under_type = StaticUnion<std::integral_constant<size_t, Place + 1>, Rty...>;

		static inline constexpr size_t mySize = 1 + sizeof...(Rty);
		static inline constexpr size_t myPlace = Place;

		// no initialization (no active member)
		constexpr StaticUnion() noexcept
		{}

		// no initialization (no active member)
		constexpr StaticUnion(nullopt_t) noexcept
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr StaticUnion(std::in_place_t, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr StaticUnion(std::in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: StaticUnion(std::in_place, static_cast<Args&&>(args)...)
		{}

		// Recursively find the place onto Tail
		template <size_t Target, typename... Args>
			requires (Target != Place)
		constexpr StaticUnion(std::in_place_index_t<Target>, Args&&... args)
			noexcept(nothrow_constructibles<under_type, std::in_place_index_t<Target>, Args...>)
			: _Tail(std::in_place_index<Target>, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename T, size_t Index, typename... Args>
			requires (same_as<clean_t<T>, Fty>)
		constexpr StaticUnion(std::in_place_type_t<T>, std::integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: StaticUnion(std::in_place, static_cast<Args&&>(args)...)
		{}

		// Find the specified type
		template <typename T, size_t Index, typename... Args>
			requires (!same_as<clean_t<T>, Fty>&& Index <= 1 + sizeof...(Rty))
		constexpr StaticUnion(std::in_place_type_t<T>, std::integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: _Tail(std::in_place_type<T>, std::integral_constant<size_t, Index + 1>{}, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr explicit StaticUnion(std::integral_constant<size_t, 0>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Recursively seek the index within Tail
		template <size_t Index, typename... Args>
			requires (Index != Place)
		constexpr explicit StaticUnion(std::integral_constant<size_t, Index>, Args&&... _Args)
			noexcept(nothrow_constructibles<under_type, std::integral_constant<size_t, Index - 1>, Args...>)
			: _Tail(std::integral_constant<size_t, Index - 1>{}, static_cast<Args&&>(_Args)...)
			, isExtended(true)
		{}

		constexpr ~StaticUnion()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires (!make_disjunction<std::is_trivially_destructible, Fty, Rty...>)
		{}

		constexpr ~StaticUnion()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires (make_conjunction<std::is_trivially_destructible, Fty, Rty...>)
		{}

		constexpr StaticUnion& operator=(nullopt_t) noexcept
		{
			hasValue = false;
			return *this;
		}

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
			return move(myValue);
		}

		[[nodiscard]]
		constexpr const Fty&& get() const&&
			noexcept(nothrow_move_constructibles<const Fty>)
		{
			return move(myValue);
		}

		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			if constexpr (Index == Place)
			{
				return get();
			}
			else if (isExtended)
			{
				return _Tail.template get<Index>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			if constexpr (Index == Place)
			{
				return get();
			}
			else if (isExtended)
			{
				return _Tail.template get<Index>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			if constexpr (Index == Place)
			{
				return move(get());
			}
			else if (isExtended)
			{
				return move(_Tail).template get<Index>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			if constexpr (Index == Place)
			{
				return move(get());
			}
			else if (isExtended)
			{
				return move(_Tail).template get<Index>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <typename T>
			requires meta::included_v<T, Fty, Rty...>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			if constexpr (same_as<T, Fty>)
			{
				return get();
			}
			else if (isExtended)
			{
				return _Tail.template get<T>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <typename T>
			requires meta::included_v<T, Fty, Rty...>
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			if constexpr (same_as<T, Fty>)
			{
				return get();
			}
			else if (isExtended)
			{
				return _Tail.template get<T>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <typename T>
			requires meta::included_v<T, Fty, Rty...>
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).get();
			}
			else if (isExtended)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
				throw std::bad_variant_access{};
			}
		}

		template <typename T>
			requires meta::included_v<T, Fty, Rty...>
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).get();
			}
			else if (isExtended)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				//static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
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

		StaticUnion(StaticUnion&&) = default;
		StaticUnion(const StaticUnion&) = default;
		StaticUnion& operator=(StaticUnion&&) = default;
		StaticUnion& operator=(const StaticUnion&) = default;

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

export namespace std
{
	template<size_t Index, size_t Place, typename... Ts>
	struct variant_alternative<Index, util::StaticUnion<integral_constant<size_t, Place>, Ts...>>
	{
		using type = meta::at<meta::MetaList<Ts...>, Index>;
	};

	template<size_t Place>
	struct variant_size<util::StaticUnion<integral_constant<size_t, Place>>>
		: integral_constant<size_t, 1>
	{};

	template<size_t Place, typename... Ts>
	struct variant_size<util::StaticUnion<integral_constant<size_t, Place>, Ts...>>
		: integral_constant<size_t, 1 + sizeof...(Ts)>
	{};

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::StaticUnion<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::StaticUnion<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::StaticUnion<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::StaticUnion<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::StaticUnion<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::StaticUnion<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::StaticUnion<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::StaticUnion<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}
}
