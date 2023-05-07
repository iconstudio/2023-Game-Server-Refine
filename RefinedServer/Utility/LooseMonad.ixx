module;
#include <variant>
#include <expected>

export module Utility.Monad.Loosen;
import Utility.Constraints;
import Utility.Meta;

namespace util
{
	template <size_t Place, bool IsTrivialDestruction, typename... Ts>
	class __variant_storage {};

	template <size_t Place, typename... Ts>
	using __variant_route = __variant_storage<Place, util::make_conjunction<std::is_trivially_destructible, Ts...>, Ts...>;

	template <size_t Place, typename Fty, typename... Rty>
	class __variant_storage<Place, true, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, std::in_place_t>, "Fty must not be std::in_place_t.");
		static_assert(!is_specialization_v<Fty, std::in_place_type_t>, "Fty must not be std::in_place_type_t.");
		static_assert(!is_indexed_v<Fty, std::in_place_index_t>, "Fty must not be std::in_place_type_t<.");

		using type = __variant_storage<Place, true, Fty, Rty...>;
		using value_type = std::remove_const_t<Fty>;
		using under_type = __variant_route<Place + 1, Rty...>;

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
		{}

		[[nodiscard]]
		constexpr Fty& get() & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr const Fty& get() const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr Fty&& get() && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr const Fty&& get() const&& noexcept
		{
			return std::move(myValue);
		}

	protected:
		union
		{
			value_type myValue;
			under_type _Tail;
		};
		bool hasValue = false;
	};

	template <size_t Place, typename Fty, typename... Rty>
	class __variant_storage<Place, false, Fty, Rty...>
	{
	public:
		static_assert(!same_as<Fty, std::in_place_t>, "Fty must not be std::in_place_t.");
		static_assert(!is_specialization_v<Fty, std::in_place_type_t>, "Fty must not be std::in_place_type_t.");
		static_assert(!is_indexed_v<Fty, std::in_place_index_t>, "Fty must not be std::in_place_type_t.");

		using type = __variant_storage<Place, true, Fty, Rty...>;
		using value_type = std::remove_const_t<Fty>;
		using under_type = __variant_route<Place + 1, Rty...>;

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
			requires (!same_as<clean_t<T>, Fty> && Index <= mySize)
		constexpr __variant_storage(std::in_place_type_t<T>, std::integral_constant<size_t, Index>, Args&&... args)
			noexcept(nothrow_constructibles<T, Args...>)
			: _Tail(std::in_place_type<T>, std::integral_constant<size_t, Index + 1>{}, static_cast<Args&&>(args)...)
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
		{}

		constexpr ~__variant_storage() noexcept
		{
			// explicitly non-trivial destructor (which would otherwise be defined as deleted
			// since the class has a variant member with a non-trivial destructor)
		}

		[[nodiscard]]
		constexpr Fty& get() & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr const Fty& get() const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr Fty&& get() && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr const Fty&& get() const&& noexcept
		{
			return std::move(myValue);
		}

		__variant_storage(__variant_storage&&) = default;
		__variant_storage(const __variant_storage&) = default;
		__variant_storage& operator=(__variant_storage&&) = default;
		__variant_storage& operator=(const __variant_storage&) = default;

	protected:
		union
		{
			value_type myValue;
			under_type _Tail;
		};
		bool hasValue = false;
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
		constexpr LooseMonad() noexcept
		{}

		template <size_t Place, typename... Args>
		constexpr explicit
			LooseMonad(in_place_index_t<Place>, Args&&... args)
			noexcept(nothrow_constructibles<__variant_route<0, Ts...>, std::in_place_index_t<Place>, Args...>)
			: myStorage(in_place_index<Place>, static_cast<Args&&>(args)...)
		{}

		template <typename T, typename... Args>
		constexpr explicit
			LooseMonad(in_place_type_t<T>, Args&&... args)
			noexcept(nothrow_constructibles<__variant_route<0, Ts...>, std::in_place_type_t<T>, std::integral_constant<size_t, 0>, Args...>)
			: myStorage(in_place_type<T>, std::integral_constant<size_t, 0>{}, static_cast<Args&&>(args)...)
		{}

		__variant_route<0, Ts...> myStorage;
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

		const LooseMonad<int, float, float> d2{ std::in_place_type<float>, 500.0f };
		const LooseMonad<int, float, int> e2{ std::in_place_type<float>, 500.0f };

		constexpr LooseMonad<int, float, float> d3{ std::in_place_type<float>, 500.0f };
		constexpr LooseMonad<int, float, int> e3{ std::in_place_type<float>, 500.0f };
	}
}
