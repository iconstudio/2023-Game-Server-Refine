export module Core.Session;
import Utility;
import Utility.Constraints;
import Utility.Atomic;

export namespace core
{
	template<typename T>
	concept session_identifiers = util::copyable<T> && util::equality_comparable<T>;

	template<typename T, session_identifiers ID>
	struct Session
	{
	public:
		using base = void;
		using crtp_type = T;
		using id_type = ID;

		Session(Session&& other) noexcept
			: isTaken(static_cast<Session&&>(other).IsTaken())
			, myID(static_cast<Session&&>(other).MyID())
		{}

		Session(volatile Session&& other) noexcept
			: isTaken(static_cast<volatile Session&&>(other).IsTaken())
			, myID(static_cast<volatile Session&&>(other).MyID())
		{}

		explicit constexpr Session(const ID& id) noexcept
			: isTaken(false)
			, myID(id)
		{}

		explicit constexpr Session(ID&& id) noexcept
			: isTaken(false)
			, myID(static_cast<ID&&>(id))
		{}

		inline Session& operator=(Session&& other) noexcept
		{
			myID = static_cast<Session&&>(other).MyID();
			isTaken = static_cast<Session&&>(other).IsTaken();

			return *this;
		}

		inline volatile Session& operator=(Session&& other) volatile noexcept
		{
			myID = static_cast<Session&&>(other).MyID();
			isTaken = static_cast<Session&&>(other).IsTaken();

			return *this;
		}

		inline Session& operator=(volatile Session&& other) noexcept
		{
			myID = static_cast<volatile Session&&>(other).MyID();
			isTaken = static_cast<volatile Session&&>(other).IsTaken();

			return *this;
		}

		inline volatile Session& operator=(volatile Session&& other) volatile noexcept
		{
			myID = static_cast<volatile Session&&>(other).MyID();
			isTaken = static_cast<volatile Session&&>(other).IsTaken();

			return *this;
		}

		constexpr ~Session() noexcept
		{}

		inline bool Take() & noexcept
		{
			return util::CompareAndSet(isTaken, false, true);
		}

		inline bool Take() volatile& noexcept
		{
			return util::CompareAndSet(isTaken, false, true);
		}

		inline void Return() & noexcept
		{
			while (!util::CompareAndSet(isTaken, true, false));
		}

		inline void Return() volatile& noexcept
		{
			while (!util::CompareAndSet(isTaken, true, false));
		}

		inline bool IsTaken() const noexcept
		{
			return isTaken.load(util::memory_order_relaxed);
		}

		inline bool IsTaken() const volatile noexcept
		{
			return isTaken.load(util::memory_order_relaxed);
		}

		[[nodiscard]]
		inline constexpr const ID MyID() const& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		inline constexpr const ID MyID() const volatile& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		inline constexpr ID&& MyID() && noexcept
		{
			return static_cast<ID&&>(myID);
		}

		[[nodiscard]]
		inline constexpr volatile ID&& MyID() volatile&& noexcept
		{
			return static_cast<volatile ID&&>(myID);
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() const& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() const volatile& noexcept
		{
			return myID;
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() && noexcept
		{
			return static_cast<ID&&>(myID);
		}

		[[nodiscard]]
		explicit inline constexpr operator ID() volatile&& noexcept
		{
			return static_cast<volatile ID&&>(myID);
		}

		[[nodiscard]]
		inline constexpr bool operator==(const Session& other) const noexcept
		{
			return myID == other.myID;
		}

		template<typename U, typename UID>
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

		Session(const Session& other) = delete;
		Session& operator=(const Session& other) = delete;

	protected:
		util::atomic_bool isTaken;
		ID myID;
	};
}

#pragma warning(push, 1)
namespace core::test
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

	void test_session() noexcept
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
#pragma warning(pop)
