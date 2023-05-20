module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket;
import Utility;
import Utility.Option;
import Net;
import Net.Addressing;
import Net.EndPoint;
import Net.Context;
import Net.Promise;

export namespace net
{
	using ::UINT_PTR;
	using ::SOCKET;
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
		Update = SO_UPDATE_ACCEPT_CONTEXT
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

	class [[nodiscard]] Socket
	{
		struct init_t {};

		constexpr Socket(init_t) noexcept
		{
		}

	public:
		constexpr Socket() noexcept
			: Socket(init_t{})
		{}

		constexpr Socket(Socket&& other) noexcept
			: Socket(init_t{})
		{
			myHandle = static_cast<::SOCKET&&>(other.myHandle);
			myEndPoint = static_cast<EndPoint&&>(other.myEndPoint);
			other.isOut = true;
		}

		explicit constexpr Socket(const SOCKET& handle) noexcept
			: isOut(false), myHandle(handle)
			, myEndPoint()
		{}

		explicit constexpr Socket(SOCKET&& handle) noexcept
			: isOut(false), myHandle(static_cast<SOCKET&&>(handle))
			, myEndPoint()
		{}

		constexpr Socket& operator=(Socket&& other) noexcept
		{
			myHandle = static_cast<SOCKET&&>(other.myHandle);
			other.isOut = true;

			return *this;
		}

		~Socket() noexcept
		{
			if (!isOut && IsValid())
			{
				::closesocket(myHandle);
				myHandle = abi::InvalidSocket;
			}
		}

		[[nodiscard]]
		constexpr const SOCKET& Handle() const& noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr const volatile SOCKET& Handle() const volatile& noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr SOCKET&& Handle() && noexcept
		{
			return static_cast<SOCKET&&>(myHandle);
		}

		[[nodiscard]]
		constexpr volatile SOCKET&& Handle() volatile&& noexcept
		{
			return static_cast<volatile SOCKET&&>(myHandle);
		}

		[[nodiscard]]
		constexpr bool IsValid() const noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

		[[nodiscard]]
		constexpr bool IsValid() const volatile noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

		[[nodiscard]]
		static inline
			Promise<Socket, int>
			CreateTCP(SocketType type = SocketType::SoFlagOverlapped)
		{
#if _DEBUG
			SOCKET socket = abi::CreateRawTCP(type);
			if (socket == abi::InvalidSocket)
			{
				return WSAGetLastError();
			}

			BOOL option = TRUE;
			::setsockopt(socket, SOL_SOCKET, SO_DEBUG, reinterpret_cast<char*>(&option), sizeof(option));

			return Socket{ socket };
#else // _DEBUG
			return Socket{ abi::CreateRawTCP(type) };
#endif // !_DEBUG
		}

		[[nodiscard]]
		static inline
			Promise<Socket, int>
			CreateUDP(SocketType type = SocketType::SoFlagOverlapped)
		{
#if _DEBUG
			SOCKET socket = abi::CreateRawUDP(type);
			if (socket == abi::InvalidSocket)
			{
				return WSAGetLastError();
			}

			BOOL option = TRUE;
			::setsockopt(socket, SOL_SOCKET, SO_DEBUG, reinterpret_cast<char*>(&option), sizeof(option));

			return Socket{ socket };
#else // _DEBUG
			return Socket{ abi::CreateRawUDP(type) };
#endif // !_DEBUG
		}

		Socket(const Socket& other) = delete;
		Socket(const volatile Socket& other) = delete;
		Socket& operator=(const Socket& other) = delete;
		Socket& operator=(const volatile Socket& other) = delete;
		Socket& operator=(const Socket& other) volatile = delete;
		Socket& operator=(const volatile Socket& other) volatile = delete;

	private:
		[[nodiscard]]
		inline constexpr bool IsOut() const noexcept
		{
			return isOut;
		}

		[[nodiscard]]
		inline constexpr bool IsOut() const volatile noexcept
		{
			return isOut;
		}

		volatile bool isOut{ false };
		SOCKET myHandle{ abi::InvalidSocket };
		EndPoint myEndPoint{};
	};
}
