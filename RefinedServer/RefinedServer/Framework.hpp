#pragma once
import Utility;
import Utility.Traits;
import Utility.Singleton;
import Utility.Array;
import Net;
import Net.EndPoint;
import Net.Context;
import Net.Socket;
import Net.CompletionPort;
import Net.Promise;

class [[nodiscard]] Framework : util::Singleton<Framework>
{
private:
	static inline constexpr unsigned concurrentHint = 6;
	static inline constexpr unsigned short serverPort = 54000;

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

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

	Framework(Framework& other) = delete;
	Framework& operator=(Framework& other) = delete;
	Framework(Framework&& other) = delete;
	Framework& operator=(Framework&& other) = delete;

private:
	net::Proxy Close(net::Socket& socket) noexcept;

	net::CompletionPort ioPort;
	net::Socket nameSocket;
	net::EndPoint nameEndPoint;

	net::Context acceptContext;
	char acceptBuffer[64];
	unsigned long accceptResultSize;

	net::Socket gameSocket;
	net::EndPoint gameEndPoint;

	util::Array<net::BasicSession*, maxNPCs> everySession;
	util::Array<net::User*, maxUsers> everyUser;
};
