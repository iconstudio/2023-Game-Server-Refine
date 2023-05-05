#pragma once
import Utility;
import Utility.Singleton;
import Utility.Array;
import Utility.Monad;
import Net;

class NODISCARD Framework : util::Singleton<Framework>
{
public:
	Framework() noexcept;
	~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update() noexcept;
	void UpdateOnce(const float& delta_time) noexcept;

	util::Monad<int> BeginAccept();
	void EndAccept() noexcept;
	void EndFailedAccept() noexcept;

	util::Monad<int> BeginWelcome();
	void EndWelcome() noexcept;
	void EndFailedWelcome() noexcept;

	util::Monad<int> BeginDispose();
	void EndDispose() noexcept;
	void EndFailedDispose() noexcept;

private:
	util::Monad<int> Dispose(net::User* const& user);
	util::Monad<int> Close(net::Socket& socket);

	bool Poll() noexcept;
	bool Post() noexcept;

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

	net::CompletionPort ioService;
	net::Socket ioSocket;

	util::Array<net::BasicUser, maxNPCs> everySession;
	util::Array<net::User, maxUsers> everyUser;
};
