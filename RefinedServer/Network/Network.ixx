module;
#include <WS2tcpip.h>
#include <MSWSock.h>
export module Network;

export namespace net
{
	using ::WSAOVERLAPPED;
	using ::WSABUF;
	using ::LPWSABUF;

	class CompletionPort;
	class Address;
	class EndPoint;
	class Socket;
	class Context;
	class SendContext;
	class BasicUser;
	class User;
	class Room;
	template<size_t Size>
	class SocketStream;

	namespace abi
	{}

	namespace service
	{
		class WorkerManager;
		class TimerManager;
		class SenderPool;
		class UserManager;
		class RoomManager;
	}

	namespace settings
	{
	}

	using ::LPFN_DISCONNECTEX;
	inline constexpr ::GUID DisconnectEx = WSAID_DISCONNECTEX;

	[[nodiscard]]
	inline int Startup() noexcept
	{
		WSADATA data{};

		constexpr unsigned short version = MAKEWORD(2, 2);
		return ::WSAStartup(version, &data);
	}
}
