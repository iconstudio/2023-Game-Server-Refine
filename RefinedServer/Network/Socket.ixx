module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket;
import Utility;
import Utility.Error;
import Net;
import Net.EndPoint;
import Net.Context;
import Net.Intrinsics;
import Net.Promise;

export namespace net
{
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
		using ::SOCKET;
		using ::OVERLAPPED;
		using ::OVERLAPPED_ENTRY;
		using ::LPOVERLAPPED_COMPLETION_ROUTINE;
		using ::LPWSAOVERLAPPED_COMPLETION_ROUTINE;

		using ::RIO_NOTIFICATION_COMPLETION_TYPE;
		using ::RIORESULT;
		using ::PRIORESULT;

		using ::RIO_BUF;
		using ::PRIO_BUF;

		using ::RIO_BUFFERID_t;
		using ::RIO_BUFFERID;
		using ::PRIO_BUFFERID;

		using ::RIO_CQ_t;
		using ::RIO_CQ;
		using ::PRIO_CQ;
		using ::RIO_RQ_t;
		using ::RIO_RQ;
		using ::PRIO_RQ;

		inline constexpr unsigned long DEFAULT_ACCEPT_BUFFER_SIZE = sizeof(SOCKADDR_IN) + 16;
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

	using ::SOCKET;
	using CompletionRoutine = ::LPWSAOVERLAPPED_COMPLETION_ROUTINE;

	class [[nodiscard]] Socket
	{
	public:
		constexpr Socket() noexcept
			: isOut(false), myHandle(abi::InvalidSocket)
			, myAddress(), myEndPoint()
		{}

		constexpr Socket(Socket&& other) noexcept
			: isOut(false), myHandle(static_cast<::SOCKET&&>(other.myHandle))
			, myAddress(util::move(other.myAddress)), myEndPoint(util::move(other.myEndPoint))
		{
			other.isOut = true;
		}

		explicit constexpr Socket(const SOCKET& handle) noexcept
			: isOut(false), myHandle(handle)
			, myAddress(), myEndPoint()
		{}

		explicit constexpr Socket(SOCKET&& handle) noexcept
			: isOut(false), myHandle(static_cast<SOCKET&&>(handle))
			, myAddress(), myEndPoint()
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

		inline ioError Bind(const EndPoint& target) noexcept
		{
			return io::Execute(::bind, myHandle, reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize()).if_then([&, this]() {
				myAddress = target;
			});
		}

		inline ioError Connect(const EndPoint& target) noexcept
		{
			return io::Execute(::connect, myHandle, reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize()).else_then([&]() {
				myEndPoint = target;
			});
		}

		inline ioError Connect(const ::SOCKADDR* const& address, const int& addrlen) const noexcept
		{
			return io::Execute(::connect, myHandle, address, addrlen);
		}

		inline ioError Listen(const int& backlog = constants::LISTEN_MAX) noexcept
		{
			return io::Execute(::listen, myHandle, backlog);
		}

		inline ioError BeginAccept(Socket& client)
		{

		}

		inline ioError Recv(WSABUF& buffer, Context* const& context, unsigned long* bytes = nullptr, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return io::Execute(::WSARecv, myHandle, util::addressof(buffer), 1UL, bytes, &flags, context, nullptr);
		}

		inline ioError Send(WSABUF buffer, Context* const& context, unsigned long* bytes = nullptr, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return io::Execute(::WSASend, myHandle, util::addressof(buffer), 1UL, bytes, flags, context, nullptr);
		}

		inline ioError BeginRecv(WSABUF& buffer, Context* const& context, CompletionRoutine routine, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return io::Execute(::WSARecv, myHandle, util::addressof(buffer), 1UL, nullptr, &flags, context, routine);
		}

		inline ioError BeginSend(WSABUF buffer, Context* const& context, CompletionRoutine routine, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return io::Execute(::WSASend, myHandle, util::addressof(buffer), 1UL, nullptr, flags, context, routine);
		}

		inline constexpr bool IsValid() const noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

		inline constexpr bool IsValid() const volatile noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

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

			if (type == SocketType::SoFlagOverlapped)
			{
				//::setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&socket), sizeof(socket));
			}

			BOOL option = TRUE;
			::setsockopt(socket, SOL_SOCKET, SO_DEBUG, reinterpret_cast<char*>(&option), sizeof(option));

			return Socket{ socket };
#else // _DEBUG
			return Socket{ abi::CreateRawTCP(type) };
#endif // !_DEBUG
		}

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

			if (type == SocketType::SoFlagOverlapped)
			{
				//::setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&socket), sizeof(socket));
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
		inline constexpr bool IsOut() const noexcept
		{
			return isOut;
		}

		inline constexpr bool IsOut() const volatile noexcept
		{
			return isOut;
		}

		volatile bool isOut;
		SOCKET myHandle;
		EndPoint myAddress, myEndPoint;
};
}
