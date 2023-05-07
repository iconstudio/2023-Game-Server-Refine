#pragma once
import Utility;
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

	net::Promise BeginAccept();
	void EndAccept() noexcept;
	void EndFailedAccept() noexcept;

	net::Promise BeginWelcome();
	void EndWelcome() noexcept;
	void EndFailedWelcome() noexcept;

	net::Promise BeginDispose();
	void EndDispose() noexcept;
	void EndFailedDispose() noexcept;

	net::Promise Dispose(net::User* const& user);

	bool Poll() noexcept;
	bool Post() noexcept;

private:
	net::Promise Close(net::Socket& socket);

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

	net::CompletionPort ioService;
	net::Socket nameSocket;
	net::EndPoint nameEndPoint;

	util::Array<net::BasicSession*, maxNPCs> everySession;
	util::Array<net::User*, maxUsers> everyUser;
};
