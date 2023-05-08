#pragma once
import Utility;
import Utility.Traits;
import Utility.Singleton;
import Utility.Array;
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
}
