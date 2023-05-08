#pragma once
import Utility;
import Utility.Traits;
import Utility.Singleton;
import Utility.Array;
import Utility.Union;
import Net;
import Net.EndPoint;
import Net.Socket;
import Net.CompletionPort;
import Net.Promise;

class [[nodiscard]] Framework : util::Singleton<Framework>
{
public:
	Framework() noexcept;
	~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update() noexcept;
	void UpdateOnce(const float& delta_time) noexcept;

	net::Proxy BeginAccept();
	void EndAccept() noexcept;
	void EndFailedAccept() noexcept;

	net::Proxy BeginWelcome();
	void EndWelcome() noexcept;
	void EndFailedWelcome() noexcept;

	net::Proxy BeginDispose();
	void EndDispose() noexcept;
	void EndFailedDispose() noexcept;

	net::Proxy Dispose(net::User* const& user);

	bool Poll() noexcept;
	bool Post() noexcept;

private:
	net::Proxy Close(net::Socket& socket);

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

	net::CompletionPort ioService;
	net::Socket nameSocket;
	net::EndPoint nameEndPoint;

	util::Array<net::BasicSession*, maxNPCs> everySession;
	util::Array<net::User*, maxUsers> everyUser;
};

namespace net
{
	static void test_promise3() noexcept
	{
		constexpr auto fnl0 = [](const int& v) noexcept -> int {
			return 300;
		};

		constexpr auto fnr0 = [](const int&) noexcept -> Promise<int, void> {
			return Promise<int, void>::succeed_t{ 300 };
		};

		Promise<int, void> vpromise0{};
		const auto r0 = vpromise0 >> fnl0;
		Promise<long long, void> vpromise1{};

		constexpr Promise<int, void> cvpromise{};

		constexpr const auto& cr0_0 = cvpromise.if_then(
			[](const int& v) -> int {
			return 300;
		});

		constexpr const auto& cr0_1 = cvpromise.and_then(
			[](const int& v) -> Promise<int, void> {
			return Promise<int, void>::succeed_t{ 300 };
		}).if_then(fnl0);

		constexpr auto cr0_2 = cvpromise >> fnr0 >> [](auto&&) noexcept -> int { return 13402; };
	}

	static void test_union() noexcept
	{
		using aa_t = util::Union<int, unsigned long, float>;

		constexpr aa_t aa{};
		using aa_0_t = aa_t::element_type<0>;
		static_assert(util::is_same_v<aa_0_t, int>, "int");
		using aa_1_t = aa_t::element_type<1>;
		//static_assert(is_same_v<aa_1_t, int>, "unsigned long");
		using aa_2_t = aa_t::element_type<2>;
		//static_assert(is_same_v<aa_2_t, int>, "float");
		//using aa_3_t = aa_t::element_type<3>;

		constexpr aa_0_t aa_ty_0_v = 0;
		constexpr aa_1_t aa_ty_1_v = 0;
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

		using bb_t = util::Union<int, unsigned long, float, double>;
		bb_t bb0{};
		//bb_t bb1{};
		bb_t bb1(util::in_place_type<float>, util::integral_constant<size_t, 0>{}, 4000.034124f);
		bb0.set(0);

		bb0 = bb1;
		bb0.reset();

		// PlacedVariant<integral_constant<size_t, 0>, int, unsigned long, float, double>
		auto& rb0_tset_0 = bb0.try_set<0>(0);
		// PlacedVariant<integral_constant<size_t, 1>, unsigned long, float, double>
		auto& rb0_tset_1 = bb0.try_set<1>(0UL);

		constexpr util::Union<int, int, int> cc{};
		constexpr util::Union<bool, int, long> dd{};
		constexpr util::Union<float, unsigned long long, char> ee{};
		constexpr util::Union<double, unsigned char, short> ff{};
	}
}
