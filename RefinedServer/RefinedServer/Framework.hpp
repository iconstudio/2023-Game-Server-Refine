#pragma once

template<typename T, typename ID>
class Session;
class BasicUser;
class User;
class CompletionPort;
class Socket;
template<typename T>
class Monad;

namespace net
{
	namespace abi
	{}

	class NODISCARD [[msvc::no_unique_address]] Functions final
	{};

	namespace service
	{
	}

	namespace settings
	{
	}
}

class NODISCARD Framework
{
public:
	Framework() noexcept = default;
	~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update() noexcept;
	void UpdateOnce(const float& delta_time) noexcept;

	Monad<int> BeginAccept();
	void EndAccept() noexcept;
	void EndFailedAccept() noexcept;

	Monad<int> BeginWelcome();
	void EndWelcome() noexcept;
	void EndFailedWelcome() noexcept;

	Monad<int> BeginDispose();
	void EndDispose() noexcept;
	void EndFailedDispose() noexcept;

private:
	Monad<int> Dispose(User* const& user);
	Monad<int> Close(Socket& socket);

	bool Poll() noexcept;
	bool Post() noexcept;

	static inline constexpr size_t maxNPCs = 1000;
	static inline constexpr size_t maxUsers = 1000;

	CompletionPort ioService;
	Socket ioSocket;

	std::array<BasicUser, maxNPCs> everySession;
	std::array<User, maxUsers> everyUser;
};
