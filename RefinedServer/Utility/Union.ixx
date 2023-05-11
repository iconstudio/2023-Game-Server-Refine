module;
#include <variant>

export module Utility.Union;
import Utility;
import Utility.Constraints;
import Utility.Meta;

namespace util
{
	template <bool _TrivialDestruction, typename... Ts>
	struct InternalVariant {};

	template <size_t Place, typename... Ts>
	struct VariantStorage {};

	template <size_t Place, typename Fty, typename... Rty>
	struct VariantStorage<Place, Fty, Rty...>
	{
		static inline constexpr size_t mySize = 1 + sizeof...(Rty);
		static inline constexpr size_t myPlace = Place;

		constexpr VariantStorage() noexcept {}

		// ctor (not void)
		template <class... Ts>
			requires (notvoids<Fty>)
		constexpr explicit
			VariantStorage(integral_constant<size_t, 0>, Ts&&... _Args)
			noexcept(nothrow_constructibles<Fty, Ts...>)
			: _Head(static_cast<Ts&&>(_Args)...)
		{}

		template <size_t _Idx, class... Ts>
			requires (0 < _Idx)
		constexpr explicit
			VariantStorage(integral_constant<size_t, _Idx>, Ts&&... _Args)
			noexcept(nothrow_constructibles<VariantStorage<Rty...>, integral_constant<size_t, _Idx - 1>, Ts...>)
			: _Tail(integral_constant<size_t, _Idx - 1>{}, static_cast<Ts&&>(_Args)...)
		{}

		constexpr VariantStorage(const VariantStorage& other) = default;
		constexpr VariantStorage(VariantStorage&& other) = default;
		constexpr VariantStorage& operator=(const VariantStorage& other) = default;
		constexpr VariantStorage& operator=(VariantStorage&& other) = default;

	protected:
		union
		{
			union
			{
				std::monostate voidData = {};
				remove_const_t<Fty> _Head;
			};
			VariantStorage<Place + 1, Rty...> _Tail;
		};
	};

	template <typename... Ts>
	using RouteVariant = InternalVariant<make_conjunction<std::is_trivially_destructible, Ts...>, Ts...>;

	// Storage for variant alternatives (trivially destructible case)
	template <typename Fty, typename... Rty>
	struct InternalVariant<true, Fty, Rty...> : VariantStorage<0, Fty, Rty...>
	{
		using base = VariantStorage<0, Fty, Rty...>;

		constexpr InternalVariant() noexcept {}
		using base::base;
	};

	// Storage for variant alternatives (non-trivially destructible case)
	template <class Fty, class... Rty>
	struct InternalVariant<false, Fty, Rty...> : VariantStorage<0, Fty, Rty...>
	{
		using base = VariantStorage<0, Fty, Rty...>;

		using base::base;

		// explicitly non-trivial destructor (which would otherwise be defined as deleted
		// since the class has a variant member with a non-trivial destructor)
		constexpr ~InternalVariant() noexcept
		{}

		constexpr InternalVariant(const InternalVariant& other) = default;
		constexpr InternalVariant(InternalVariant&& other) = default;
		constexpr InternalVariant& operator=(const InternalVariant& other) = default;
		constexpr InternalVariant& operator=(InternalVariant&& other) = default;
	};
}

export namespace util
{
	template <typename Indexer = integral_constant<size_t, 0>, typename... Ts>
	class PlacedVariant;

	template <size_t Place>
	class PlacedVariant<integral_constant<size_t, Place>>
	{};

	template <size_t Place, typename Fty, typename... Rty>
	class PlacedVariant<integral_constant<size_t, Place>, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, nullopt_t>, "Fty must not be nullopt_t.");
		static_assert(!same_as<Fty, in_place_t>, "Fty must not be in_place_t.");
		static_assert(!is_specialization_v<Fty, in_place_type_t>, "Fty must not be in_place_type_t.");
		static_assert(!is_indexed_v<Fty, in_place_index_t>, "Fty must not be in_place_index_t.");

		using type = remove_const_t<Fty>;
		using node_type = PlacedVariant<integral_constant<size_t, Place + 1>, Rty...>;

		static inline constexpr size_t mySize = 1 + sizeof...(Rty);
		static inline constexpr size_t myPlace = Place;

		template <size_t Index>
		static inline constexpr size_t relativeIndex = Index - Place;

		template <size_t Index>
		using element_type = meta::at<meta::MetaList<Fty, Rty...>, relativeIndex<Index>>;
		template <size_t Index>
		using value_type = element_type<Index>;
		template <size_t Index>
		using const_value_type = add_const_t<value_type<Index>>;
		template <size_t Index>
		using reference_type = add_lvalue_reference_t<value_type<Index>>;
		template <size_t Index>
		using const_reference_type = add_lvalue_reference_t<value_type<Index>>;
		template <size_t Index>
		using rvalue_type = add_rvalue_reference_t<value_type<Index>>;
		template <size_t Index>
		using const_rvalue_type = add_rvalue_reference_t<value_type<Index>>;

		template <size_t Index, template<size_t> typename Indexer>
		static inline constexpr bool nothrowCopyPursuer = (Index == myPlace && nothrow_copy_constructibles<Fty>)
			|| (Index != myPlace && 1 < mySize && nothrow_copy_constructibles<Indexer<Index>>);

		template <size_t Index, template<size_t> typename Indexer>
		static inline constexpr bool nothrowMovePursuer = (Index == myPlace && nothrow_move_constructibles<Fty>)
			|| (Index != myPlace && 1 < mySize && nothrow_move_constructibles<Indexer<Index>>);

		// no initialization (no active member)
		constexpr PlacedVariant() noexcept
		{}

		// no initialization (no active member)
		constexpr PlacedVariant(nullopt_t) noexcept
		{}

		// Initialize my value with Args (not void)
		template <typename... Args>
			requires (notvoids<Fty>)
		constexpr PlacedVariant(in_place_t, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: myValue(static_cast<Args&&>(args)...)
			, hasValue(true)
		{}

		// Initialize my value with Args (void)
		template <typename... Args>
			requires (!notvoids<Fty>)
		constexpr PlacedVariant(in_place_t, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: voidData()
			, hasValue(true)
		{}

		// Initialize my value with Args by a Index (not void)
		template <typename... Args>
			requires (notvoids<Fty>)
		constexpr PlacedVariant(in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: PlacedVariant(in_place, static_cast<Args&&>(args)...)
		{}

		// Initialize my value with Args by a Index (void)
		template <typename... Args>
			requires (!notvoids<Fty>)
		constexpr PlacedVariant(in_place_index_t<Place>, Args&&... args)
			noexcept
			: PlacedVariant(in_place, nullopt)
		{}

		// Initialize my value with Args by a Type (not void)
		template <typename T, size_t Hint, typename... Args>
			requires (same_as<clean_t<T>, Fty>&& notvoids<Fty>)
		explicit(is_explicit_constructible_v<T>)
			constexpr
			PlacedVariant(in_place_type_t<T>, integral_constant<size_t, Hint>, Args&&... args)
			noexcept(nothrow_constructibles<Fty, Args...>)
			: PlacedVariant(in_place, static_cast<Args&&>(args)...)
		{}

		// Initialize my value with Args by a Type (void)
		// T = void
		template <typename T, size_t Hint, typename... Args>
			requires (same_as<clean_t<T>, Fty> && !notvoids<Fty>)
		explicit
			constexpr
			PlacedVariant(in_place_type_t<T>, integral_constant<size_t, Hint>, Args&&... args)
			noexcept
			: PlacedVariant(in_place, nullopt)
		{}

		// Recursively find the place onto Tail
		template <size_t Target, typename... Args>
			requires (Place < Target&& Target < Place + 1 + sizeof...(Rty))
		constexpr PlacedVariant(in_place_index_t<Target>, Args&&... args)
			noexcept(nothrow_constructibles<node_type, in_place_index_t<Target>, Args...>)
			: _Tail(in_place_index<Target>, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		// When cannot find the place onto Tail
		template <size_t Target, typename... Args>
			requires (Place + 1 + sizeof...(Rty) <= Target)
		explicit constexpr PlacedVariant(in_place_index_t<Target>, Args&&... args)
		{
			static_assert(always_false<in_place_index_t<Target>>, "Target index is out of range.");
		}

		// Place the specified type
		template <typename T, typename... Args>
			requires (!same_as<clean_t<T>, Fty>)
		explicit(is_explicit_constructible_v<T>)
			constexpr
			PlacedVariant(in_place_type_t<T>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: PlacedVariant(in_place_type<T>, integral_constant<size_t, 0>{}, static_cast<Args&&>(args)...)
		{}

		// Place the specified type from hint
		template <typename T, size_t Guard, typename... Args>
			requires (!same_as<clean_t<T>, Fty>&& Guard <= 1 + sizeof...(Rty))
		explicit(is_explicit_constructible_v<T>)
			constexpr
			PlacedVariant(in_place_type_t<T>, integral_constant<size_t, Guard>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: _Tail(in_place_type<T>, integral_constant<size_t, Guard + 1>{}, static_cast<Args&&>(args)...)
			, isExtended(true)
		{}

		constexpr ~PlacedVariant()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires(!trivially_destructibles<Fty, Rty...>)
		{}

		constexpr ~PlacedVariant()
			noexcept(nothrow_destructibles<Fty, Rty...>)
			requires(trivially_destructibles<Fty, Rty...>)
		= default;

		constexpr PlacedVariant& operator=(nullopt_t) noexcept
		{
			hasValue = false;
			return *this;
		}

		// getter (not void)
		template<typename W = Fty>
			requires (notvoids<W>)
		[[nodiscard]]
		constexpr W& value() &
			noexcept(nothrow_copy_constructibles<W>)
		{
			return myValue;
		}

		// getter (not void)
		template<typename W = Fty>
			requires (notvoids<W>)
		[[nodiscard]]
		constexpr const W& value() const&
			noexcept(nothrow_copy_constructibles<const W>)
		{
			return myValue;
		}

		// getter (not void)
		template<typename W = Fty>
			requires (notvoids<W>)
		[[nodiscard]]
		constexpr W&& value() &&
			noexcept(nothrow_move_constructibles<W>)
		{
			return move(myValue);
		}

		// getter (not void)
		template<typename W = Fty>
			requires (notvoids<W>)
		[[nodiscard]]
		constexpr const W&& value() const&&
			noexcept(nothrow_move_constructibles<const W>)
		{
			return move(myValue);
		}

		// getter (void)
		template<typename W = Fty>
			requires (!notvoids<W>)
		constexpr void value() const noexcept
		{
			static_assert(always_false<W>, "Cannot get void type.");
		}

		// index getter
		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() &
			noexcept(nothrowCopyPursuer<Index, value_type>)
		{
			if constexpr (Index == Place)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				if (isExtended)
				{
					return _Tail.template get<Index>();
				}
				else
				{
					throw std::bad_variant_access{};
				}
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// index getter
		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
			noexcept(nothrowCopyPursuer<Index, const_value_type>)
		{
			if constexpr (Index == Place)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				return _Tail.template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// index getter
		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() &&
			noexcept(nothrowMovePursuer<Index, value_type>)
		{
			if constexpr (Index == Place)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// index getter
		template <size_t Index>
			requires (Index <= 1 + Place + sizeof...(Rty))
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
			noexcept(nothrowMovePursuer<Index, const_value_type>)
		{
			if constexpr (Index == Place)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<Index>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (not void)
		template <typename T>
			requires (notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			if constexpr (same_as<T, Fty>)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				return _Tail.template get<T>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (not void)
		template <typename T>
			requires (notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			if constexpr (same_as<T, Fty>)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				return _Tail.template get<T>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (not void)
		template <typename T>
			requires (notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (not void)
		template <typename T>
			requires (notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				static_assert(always_false<Fty>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (included void)
		template <typename T>
			requires (!notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&
		{
			if constexpr (same_as<T, Fty>)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				return _Tail.template get<T>();
			}
			else
			{
				static_assert(always_false<T>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (included void)
		template <typename T>
			requires (!notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&
		{
			if constexpr (same_as<T, Fty>)
			{
				return value();
			}
			else if constexpr (1 < mySize)
			{
				return _Tail.template get<T>();
			}
			else
			{
				static_assert(always_false<T>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (included void)
		template <typename T>
			requires (!notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get()&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				static_assert(always_false<T>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (included void)
		template <typename T>
			requires (!notvoids<T>&& meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr decltype(auto)
			get() const&&
		{
			if constexpr (same_as<T, Fty>)
			{
				return move(*this).value();
			}
			else if constexpr (1 < mySize)
			{
				return move(_Tail).template get<T>();
			}
			else
			{
				static_assert(always_false<T>, "This Monad does not have the indexed type.");
			}
		}

		// type getter (not included void)
		template <typename T>
			requires (!notvoids<T> && !meta::included_v<T, Fty, Rty...>)
		[[nodiscard]]
		constexpr void get() const
		{
			static_assert(always_false<T>, "Cannot get the not included void type.");
		}

		template<typename W = Fty>
			requires (notvoids<W>)
		constexpr PlacedVariant& set(const W& value) &
			noexcept(nothrow_copy_constructibles<W>)
		{
			myValue = value;
			hasValue = true;

			return *this;
		}

		template<typename W = Fty>
			requires (notvoids<W>)
		constexpr PlacedVariant& set(W&& value) &
			noexcept(nothrow_move_constructibles<W>)
		{
			myValue = move(value);
			hasValue = true;

			return *this;
		}

		template<typename W = Fty>
			requires (notvoids<W>)
		constexpr PlacedVariant&& set(const W& value) &&
			noexcept(nothrow_copy_constructibles<W>)
		{
			myValue = value;
			hasValue = true;

			return move(*this);
		}

		template<typename W = Fty>
			requires (notvoids<W>)
		constexpr PlacedVariant&& set(W&& value) &&
			noexcept(nothrow_move_constructibles<W>)
		{
			myValue = move(value);
			hasValue = true;

			return move(*this);
		}

		template <typename W = Fty, typename... Args>
			requires (notvoids<W>)
		constexpr W& emplace(Args&&... args) &
			noexcept(nothrow_constructibles<W, Args&&...>)
		{
			myValue = W{ forward<Args>(args)... };
			hasValue = true;

			return myValue;
		}

		template <typename W = Fty, typename... Args>
			requires (notvoids<W>)
		constexpr W&& emplace(Args&&... args) &&
			noexcept(nothrow_constructibles<W, Args&&...>)
		{
			myValue = W{ forward<Args>(args)... };
			hasValue = true;

			return move(myValue);
		}

		template <size_t Index, typename Uty>
			requires (notvoids<Fty>&& Index == Place)
		constexpr PlacedVariant& try_set(Uty&& value)&
		{
			return set(forward<Uty>(value));
		}

		template <size_t Index, typename Uty>
			requires (notvoids<Fty>&& Index == Place)
		constexpr PlacedVariant&& try_set(Uty&& value)&&
		{
			return set(forward<Uty>(value));
		}

		template <size_t Index, typename Uty>
			requires (!notvoids<Fty>&& Index == Place)
		constexpr void try_set(Uty&& value) const
		{
			static_assert(always_false<Uty>, "Cannot set the value of a void type.");
		}

		template <size_t Index>
			requires (!notvoids<Fty>&& Index == Place)
		constexpr void try_set() const
		{
			static_assert(always_false<integral_constant<size_t, Index>>, "Cannot set the value of a void type.");
		}

		template <size_t Index, typename Uty>
			requires (Index != Place && Index <= 1 + Place + sizeof...(Rty))
		constexpr decltype(auto) try_set(Uty&& value)&
		{
			if constexpr (1 < mySize)
			{
				return _Tail.template try_set<Index>(forward<Uty>(value));
			}
			else
			{
				static_assert(always_false<Uty>, "This Monad does not have the indexed type.");
			}
		}

		template <size_t Index, typename Uty>
			requires (Index != Place && Index <= 1 + Place + sizeof...(Rty))
		constexpr decltype(auto) try_set(Uty&& value)&&
		{
			if constexpr (1 < mySize)
			{
				return move(_Tail).template try_set<Index>(forward<Uty>(value));
			}
			else
			{
				static_assert(always_false<Uty>, "This Monad does not have the indexed type.");
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
			if constexpr (1 < mySize)
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
			if constexpr (1 < mySize)
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
			if constexpr (1 < mySize)
			{
				if (isExtended)
				{
					try
					{
						return _Tail.template is_valueless<Index>();
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
			else
			{
				return true;
			}
		}

		constexpr PlacedVariant(const PlacedVariant&) noexcept = default;
		constexpr PlacedVariant(PlacedVariant&&) noexcept = default;

		constexpr PlacedVariant& operator=(const PlacedVariant& other) & noexcept
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
				_Tail.operator=(other._Tail);
			}

			return *this;
		}

		constexpr PlacedVariant& operator=(PlacedVariant&& other) & noexcept
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
				_Tail.operator=(move(other._Tail));
			}

			return *this;
		}

		constexpr PlacedVariant&& operator=(const PlacedVariant& other) && noexcept
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
				_Tail.operator=(other._Tail);
			}

			return move(*this);
		}

		constexpr PlacedVariant&& operator=(PlacedVariant&& other) && noexcept
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
				_Tail.operator=(move(other._Tail));
			}

			return move(*this);
		}

	private:
		friend class node_type;

		struct void_guard {};

		union
		{
			union
			{
				std::monostate voidData;
				conditional_t<notvoids<type>, type, void_guard> myValue;
			};
			node_type _Tail;
		};

		bool hasValue = false;
		bool isExtended = false;
	};

	using ::std::integral_constant;
	using FirstIndexer = integral_constant<size_t, 0>;

	template <typename... Ts>
	using Union = PlacedVariant<FirstIndexer, Ts...>;
}

export namespace std
{
	template<size_t Index, size_t Place, typename... Ts>
	struct variant_alternative<Index, util::PlacedVariant<integral_constant<size_t, Place>, Ts...>>
	{
		using type = meta::at<meta::MetaList<Ts...>, Index>;
	};

	template<size_t Place>
	struct variant_size<util::PlacedVariant<integral_constant<size_t, Place>>>
		: integral_constant<size_t, 1>
	{};

	template<size_t Place, typename... Ts>
	struct variant_size<util::PlacedVariant<integral_constant<size_t, Place>, Ts...>>
		: integral_constant<size_t, 1 + sizeof...(Ts)>
	{};

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::PlacedVariant<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::PlacedVariant<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<Index>()))
	{
		return _Val.template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::PlacedVariant<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<size_t Index, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::PlacedVariant<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<Index>()))
	{
		return move(_Val).template get<Index>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::PlacedVariant<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::PlacedVariant<integral_constant<size_t, Place>, Ts...>& _Val)
		noexcept(noexcept(_Val.template get<T>()))
	{
		return _Val.template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(util::PlacedVariant<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}

	template<typename T, size_t Place, typename... Ts>
	constexpr decltype(auto)
		get(const util::PlacedVariant<integral_constant<size_t, Place>, Ts...>&& _Val)
		noexcept(noexcept(move(_Val).template get<T>()))
	{
		return move(_Val).template get<T>();
	}
}

#pragma warning(push, 0)
namespace util::test
{
	void test_union() noexcept
	{
		using aa_t = Union<int, void, unsigned long, float>;

		constexpr aa_t aa{};
		using aa_0_t = aa_t::element_type<0>;
		static_assert(util::is_same_v<aa_0_t, int>, "int");
		using aa_1_t = aa_t::element_type<1>;
		//static_assert(is_same_v<aa_1_t, int>, "unsigned long");
		using aa_2_t = aa_t::element_type<2>;
		//static_assert(is_same_v<aa_2_t, int>, "float");
		//using aa_3_t = aa_t::element_type<3>;

		constexpr aa_0_t aa_ty_0_v = 0;
		//constexpr aa_1_t aa_ty_1_v;
		constexpr aa_2_t aa_ty_2_v = 0;

		aa.myPlace;
		aa.mySize;
		aa.relativeIndex<0>;
		aa.relativeIndex<1>;
		aa.relativeIndex<2>;
		aa.relativeIndex<3>;

		constexpr bool a_has_0 = aa.has_value<0>();
		constexpr bool a_has_1 = aa.has_value<1>();
		constexpr bool a_has_2 = aa.has_value<2>();

		using bb_t = Union<int, unsigned long, float, double>;
		bb_t bb0{};
		//bb_t bb1{};
		bb_t bb1(in_place_type<float>, integral_constant<size_t, 0>{}, 4000.034124f);
		bb0.set(0);

		bb0 = bb1;
		bb0.reset();

		// PlacedVariant<integral_constant<size_t, 0>, int, unsigned long, float, double>
		auto& rb0_tset_0 = bb0.try_set<0>(0);
		// PlacedVariant<integral_constant<size_t, 1>, unsigned long, float, double>
		auto& rb0_tset_1 = bb0.try_set<1>(0UL);

		constexpr Union<int, int, int> cc{};
		constexpr Union<bool, int, long> dd{};
		constexpr Union<float, unsigned long long, char> ee{};
		constexpr Union<double, unsigned char, short> ff{};
	}
}
#pragma warning(pop)
