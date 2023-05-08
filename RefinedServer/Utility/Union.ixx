module;
#include <variant>

export module Utility.Union;
import Utility;
import Utility.Constraints;
import Utility.Meta;

export namespace util
{
	template <typename Indexer = integral_constant<size_t, 0>, typename... Ts>
	class StaticUnion;

	template <size_t Place>
	class StaticUnion<integral_constant<size_t, Place>>
	{};

	template <size_t Place, notvoids Fty, notvoids... Rty>
	class StaticUnion<integral_constant<size_t, Place>, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, nullopt_t>, "Fty must not be nullopt_t.");
		static_assert(!same_as<Fty, in_place_t>, "Fty must not be in_place_t.");
		static_assert(!is_specialization_v<Fty, in_place_type_t>, "Fty must not be in_place_type_t.");
		static_assert(!is_indexed_v<Fty, in_place_index_t>, "Fty must not be in_place_index_t.");

		using type = StaticUnion<integral_constant<size_t, Place>, Fty, Rty...>;
		using value_type = remove_const_t<Fty>;
		using under_type = StaticUnion<integral_constant<size_t, Place + 1>, Rty...>;

		static inline constexpr size_t mySize = 1 + sizeof...(Rty);
		static inline constexpr size_t myPlace = Place;

		template <size_t Index>
		static inline constexpr size_t relativeIndex = Index - Place;

		template <size_t Index>
		using element_type = meta::at<meta::MetaList<Fty, Rty...>, relativeIndex<Index>>;

		template <size_t Index, template<typename> typename Predicate>
		static inline constexpr bool nothrowGetter = (Index == myPlace && Predicate<Fty>)
			|| (Index != myPlace && Predicate<element_type<Index>>);

		// no initialization (no active member)
		constexpr StaticUnion() noexcept
		{}

		// no initialization (no active member)
		constexpr StaticUnion(nullopt_t) noexcept
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr StaticUnion(in_place_t, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr StaticUnion(in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: StaticUnion(in_place, static_cast<Args&&>(args)...)
		{}

		// Recursively find the place onto Tail
		template <size_t Target, typename... Args>
			requires (Target != Place)
		constexpr StaticUnion(in_place_index_t<Target>, Args&&... args)
			noexcept(nothrow_constructibles<under_type, in_place_index_t<Target>, Args...>)
			: _Tail(in_place_index<Target>, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename T, size_t Index, typename... Args>
			requires (same_as<clean_t<T>, Fty>)
		constexpr StaticUnion(in_place_type_t<T>, integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: StaticUnion(in_place, static_cast<Args&&>(args)...)
		{}

		// Find the specified type
		template <typename T, size_t Index, typename... Args>
			requires (!same_as<clean_t<T>, Fty>&& Index <= 1 + sizeof...(Rty))
		constexpr StaticUnion(in_place_type_t<T>, integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: _Tail(in_place_type<T>, integral_constant<size_t, Index + 1>{}, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// Initialize my value with Args
		template <typename... Args>
		constexpr explicit StaticUnion(integral_constant<size_t, 0>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Recursively seek the index within Tail
		template <size_t Index, typename... Args>
			requires (Index != Place)
		constexpr explicit StaticUnion(integral_constant<size_t, Index>, Args&&... _Args)
			noexcept(nothrow_constructibles<under_type, integral_constant<size_t, Index - 1>, Args...>)
			: _Tail(integral_constant<size_t, Index - 1>{}, static_cast<Args&&>(_Args)...)
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

		constexpr void reset() noexcept
		{
			if constexpr (1 < mySize)
			{
				if (isExtended)
				{
					_Tail.reset();
				}
			}

			//myData = Data{};
			hasValue = false;
			isExtended = false;
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

		template <typename T>
			requires (same_as<clean_t<T>, Fty>)
		[[nodiscard]]
		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		template <typename T>
			requires (!same_as<clean_t<T>, Fty>)
		[[nodiscard]]
		constexpr bool has_value() const noexcept
		{
			if (isExtended)
			{
				try
				{
					return _Tail.template has_value<T>();
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

		constexpr StaticUnion(const StaticUnion&) noexcept = default;
		constexpr StaticUnion(StaticUnion&&) noexcept = default;

		constexpr StaticUnion& operator=(const StaticUnion& other) & noexcept
		{
			// make empty itself
			reset();

			if (other.hasValue)
			{
				myValue = other.myValue;
				hasValue = true;
			}
			else if (other.isExtended)
			{
				if (isExtended)
				{
					_Tail.operator=(other._Tail);
				}
			}

			return *this;
		}

		constexpr StaticUnion& operator=(StaticUnion&& other) & noexcept
		{
			// make empty itself
			reset();

			if (other.hasValue)
			{
				myValue = move(other.myValue);
				hasValue = true;
			}
			else if (other.isExtended)
			{
				if (isExtended)
				{
					_Tail.operator=(move(other._Tail));
				}
			}

			return *this;
		}

		constexpr StaticUnion&& operator=(const StaticUnion& other) && noexcept
		{
			// make empty itself
			reset();

			if (other.hasValue)
			{
				myValue = other.myValue;
				hasValue = true;
			}
			else if (other.isExtended)
			{
				if (isExtended)
				{
					_Tail.operator=(other._Tail);
				}
			}

			return move(*this);
		}

		constexpr StaticUnion&& operator=(StaticUnion&& other) && noexcept
		{
			// make empty itself
			reset();

			if (other.hasValue)
			{
				myValue = move(other.myValue);
				hasValue = true;
			}
			else if (other.isExtended)
			{
				if (isExtended)
				{
					_Tail.operator=(move(other._Tail));
				}
			}

			return move(*this);
		}

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

namespace util
{
	constexpr void test_union()
	{
		const StaticUnion<int, int, int> aa{};
		const StaticUnion<bool, int, long> bb{};
		const StaticUnion<float, unsigned long long, char> cc{};
		const StaticUnion<double, unsigned char, short> dd{};
	}
}
