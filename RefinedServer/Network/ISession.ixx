module;
#include <type_traits>
#include <concepts>

export module Net.Session;
import Utility;
import Utility.Constraints;
import Utility.Atomic;

namespace net
{
	namespace detail
	{
		template<typename T, typename Ty = std::remove_cvref_t<T>>
		using SelectEn = typename std::conditional_t
			<std::is_enum_v<std::remove_cvref_t<T>>, std::underlying_type<T>, std::type_identity<T>>::type;

		template<typename T, typename Ty = std::remove_cvref_t<T>>
		using SelectIn = typename std::enable_if_t<std::is_integral_v<std::remove_cvref_t<T>>, std::type_identity<T>>::type;

		template<typename T, typename Ty = std::remove_cvref_t<T>>
		using SelectUT = typename std::conditional_t<std::is_integral_v<Ty>, std::type_identity<Ty>, std::underlying_type<Ty>>::type;

		// undefined
		template<typename T>
		inline SelectEn<T> declenum(T&& value) noexcept
		{
			static_assert(util::always_false<T>, "declenum is undefined for non-enums");
		}

		// undefined
		template<typename T>
		inline SelectIn<T> declintg(T&& value) noexcept
		{
			static_assert(util::always_false<T>, "declintg is undefined for non-integrals");
		}

		// undefined
		template<typename T>
		inline SelectUT<T> declitem(T&& value) noexcept
		{
			static_assert(util::always_false<T>, "declitem is undefined for non-qualifying identifiers");
		}
	}

	export template<typename T>
		concept managed_session_items = std::copyable<T>
		&& std::equality_comparable<T>
		&& std::is_trivial_v<T>
		&& requires (const T id)
	{
		detail::declitem(id);
	};
}

export namespace net
{
	template<typename ID>
	concept session_identifiers = managed_session_items<ID>;

	template<typename T, typename ID>
	concept qualified_sessions = session_identifiers<ID>;

	template<typename T, session_identifiers ID>
	struct Session
	{
	public:
		using base = void;
		using crtp_type = T;
		using id_type = ID;

		Session(Session&& other) noexcept
			: myID(static_cast<Session&&>(other).MyID())
			, isTaken(static_cast<Session&&>(other).IsTaken())
		{}

		explicit constexpr Session(const ID& id) noexcept
			: myID(id)
			, isTaken(false)
		{}

		explicit constexpr Session(ID&& id) noexcept
			: myID(static_cast<ID&&>(id))
			, isTaken(false)
		{}

		inline Session& operator=(Session&& other) noexcept
		{
			myID = static_cast<Session&&>(other).MyID();
			isTaken = static_cast<Session&&>(other).IsTaken();
		}

		constexpr ~Session() noexcept = default;

		inline bool Take() & noexcept
		{
			return util::CompareAndSet(isTaken, false, true);
		}

		inline void Return() & noexcept
		{
			while (!util::CompareAndSet(isTaken, true, false));
		}

		inline bool IsTaken() const noexcept
		{
			return isTaken.load(util::memory_order_relaxed);
		}

		[[nodiscard]]
		inline constexpr const ID MyID() const& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		inline constexpr ID&& MyID() && noexcept
		{
			return static_cast<ID&&>(myID);
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() const& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() && noexcept
		{
			return static_cast<ID&&>(myID);
		}

		[[nodiscard]]
		inline constexpr bool operator==(const Session& other) const noexcept
		{
			return myID == other.myID;
		}

		template<typename U, session_identifiers UID>
		[[nodiscard]]
		inline constexpr bool operator==(const Session<U, UID>& other) const noexcept
		{
			if constexpr (util::same_as<UID, ID>)
			{
				if constexpr (util::same_as<U, T>)
				{
					return myID == other.myID;
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

		[[nodiscard]]
		inline constexpr util::strong_ordering operator<=>(const Session& other) const noexcept
			requires util::integrals<ID>
		{
			return myID <=> other.myID;
		}

		[[nodiscard]]
		inline constexpr util::strong_ordering operator<=>(const Session& other) const noexcept
			requires util::enumerations<ID>
		{
			return util::to_underlying(myID) <=> util::to_underlying(other.myID);
		}

	protected:
		ID myID;

		volatile util::atomic_bool isTaken;
	};
}

namespace net
{
	enum TestIDEN1
	{
		asdasf, sfaf, sadf, asdf
	};

	enum class TestIDEN2 : unsigned long long
	{
		aaaa, bbbb, cccc
	};

	enum TestIDEN3 {};
	enum TestIDEN4 {};

	class TestCL
	{};

	static void test_session() noexcept
	{
		//auto a = detail::declenum(bool{});
		//auto b = detail::declenum(int{});
		//auto c = detail::declenum(long long{});
		//auto d = detail::declenum(TestIDEN1{});
		//auto ed = detail::declenum(TestIDEN2{});

		//auto f = detail::declintg(bool{});
		//auto g = detail::declintg(int{});
		//auto h = detail::declintg(long long{});
		//auto i = detail::declintg(TestIDEN1{});
		//auto j = detail::declintg(TestIDEN2{});

		//auto k = detail::declintg(declenum(bool{}));
		//auto l = detail::declintg(declenum(int{}));
		//auto m = detail::declintg(declenum(long long{}));
		//auto n = detail::declintg(detail::declenum(TestIDEN1{}));
		//auto o = detail::declintg(detail::declenum(TestIDEN2{}));

		//auto p = detail::declenum(declintg(bool{}));
		//auto q = detail::declenum(declintg(int{}));
		//auto r = detail::declenum(declintg(long long{}));
		//auto s = detail::declenum(declintg(TestIDEN1{}));
		//auto t = detail::declenum(declintg(TestIDEN2{}));

		//const auto v = detail::declitem<bool>(bool{});
		//const auto w = detail::declitem(int{});
		//const auto x = detail::declitem(long long{});
		//const auto y = detail::declitem(TestIDEN1{});
		//const auto z = detail::declitem(TestIDEN2{});

		constexpr Session<TestCL, bool> id0{ bool{} };
		constexpr Session<int, int> id1{ int {} };
		constexpr Session<long long, long long> id2{ long long{} };
		constexpr Session<TestIDEN1, TestIDEN1> id3{ TestIDEN1 {} };
		constexpr Session<TestIDEN2, TestIDEN2> id4{ TestIDEN2 {} };
		constexpr Session<TestIDEN3, int> id5{ 0 };
		//constexpr Session<TestIDEN3, float> id6{ 20.010523029f };
	}
}
