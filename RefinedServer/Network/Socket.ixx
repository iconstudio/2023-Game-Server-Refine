module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket;
import Utility;
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
	}

	using ::SOCKET;

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

		inline ioError BeginRecv(WSABUF& buffer, Context* context, unsigned long* bytes = nullptr, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return io::Execute(::WSARecv, myHandle, &buffer, 1UL, bytes, &flags, context, nullptr);
		}

		inline constexpr bool IsValid() const noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

		inline constexpr bool IsValid() const volatile noexcept
		{
			return myHandle != abi::InvalidSocket;
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
