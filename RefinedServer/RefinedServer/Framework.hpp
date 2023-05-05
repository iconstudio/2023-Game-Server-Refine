#pragma once
import Utility;
import Utility.Array;
import Utility.Monad;

class NODISCARD Framework
{
public:
	Framework() noexcept = default;
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
	util::Monad<int> Dispose(User* const& user);
	util::Monad<int> Close(Socket& socket);

	bool Poll() noexcept;
	bool Post() noexcept;

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

	CompletionPort ioService;
	Socket ioSocket;

	util::Array<BasicUser, maxNPCs> everySession;
	util::Array<User, maxUsers> everyUser;
};
