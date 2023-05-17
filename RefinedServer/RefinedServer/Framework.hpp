#pragma once
import Utility;
import Utility.Traits;
import Utility.Singleton;
import Utility.Array;
import Net;
import Net.EndPoint;
import Net.Context;
import Net.Socket;
import Net.Worker;
import Net.CompletionPort;
import Net.Intrinsics;
import Core.User;
import Core.Service.UserManager;
import Core.Service.WorkerManager;

class [[nodiscard]] Framework : util::Singleton<Framework>
{
private:
	static inline constexpr unsigned concurrentHint = 6;
	static inline constexpr unsigned long long serverTcpID = net::constants::SERVER_ID;
	static inline constexpr unsigned long long serverUdpID = serverTcpID + 1;

	static inline constexpr unsigned short tcpPort = 54000;
	static inline constexpr unsigned short udpPort = 54001;

public:
	Framework() noexcept;
	~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update() noexcept;
	void UpdateOnce(const float& delta_time) noexcept;

	net::ioError BeginAccept(const core::userid_t& start);
	void EndAccept() noexcept;
	void EndFailedAccept() noexcept;

	net::Proxy BeginWelcome();
	void EndWelcome() noexcept;
	void EndFailedWelcome() noexcept;

	net::Proxy BeginDispose();
	void EndDispose() noexcept;
	void EndFailedDispose() noexcept;

	net::Proxy Dispose(core::User* const& user);

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

	net::Socket gameSocket;
	net::EndPoint gameEndPoint;

	core::service::UserManager userManager;
	core::service::WorkerManager workerManager;
};
