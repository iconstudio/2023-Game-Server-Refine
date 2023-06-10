module;
#include <WS2tcpip.h>
#include <MSWSock.h>
export module Net.Socket.ABI;

export namespace net
{
	using ::UINT_PTR;
	using ::SOCKET;
	using ::LPWSAOVERLAPPED_COMPLETION_ROUTINE;
	using SoLinger = ::linger;
	using CompletionRoutine = ::LPWSAOVERLAPPED_COMPLETION_ROUTINE;

	enum SocketDatagram : int
	{
		Datagram = SOCK_DGRAM,
		Stream = SOCK_STREAM,
	};

	enum SocketType : unsigned long
	{
		SoFlagNone = 0,
		SoFlagOverlapped = WSA_FLAG_OVERLAPPED,
		SoFlagRIO = WSA_FLAG_REGISTERED_IO,
	};

	enum class SocketOptions : int
	{
		Debug = SO_DEBUG,
		Recyclable = SO_REUSEADDR,
		DontRoute = SO_DONTROUTE,
		Broadcast = SO_BROADCAST,
		UseLoopback = SO_USELOOPBACK,
		Linger = SO_LINGER,
		NoDelay = TCP_NODELAY,
		DontLinger = SO_DONTLINGER,
		KeepAlive = SO_KEEPALIVE,
		Update = SO_UPDATE_ACCEPT_CONTEXT // MSWSock.h
	};

	enum SocketIos : unsigned long
	{
		SocketIoForGettingFunction = SIO_GET_EXTENSION_FUNCTION_POINTER,
	};

	enum class SocketTasks : unsigned long
	{
		Disconnect = TF_DISCONNECT,
		Reuse = TF_REUSE_SOCKET,
	};

	namespace abi
	{
		inline constexpr unsigned long DEFAULT_ACCEPT_SIZE = sizeof(SOCKADDR_IN) + 16UL;
		inline constexpr ::SOCKET InvalidSocket = INVALID_SOCKET;

		[[nodiscard]]
		inline SOCKET CreateRawTCP(const SocketType& type) noexcept
		{
			return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, type);
		}

		[[nodiscard]]
		inline SOCKET CreateRawUDP(const SocketType& type) noexcept
		{
			return ::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, nullptr, NULL, type);
		}
	}
}
