module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket;
import Utility;
import Utility.Monad;
import Utility.Memory;
import Net;
import Net.EndPoint;
import Net.Context;
import Net.Intrinsics;

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
		inline constexpr unsigned long DEFAULT_ACCEPT_BUFFER_SIZE = sizeof(SOCKADDR_IN) + 16;
		inline constexpr ::SOCKET InvalidSocket = INVALID_SOCKET;
	}

	using ::SOCKET;

	class [[nodiscard]] Socket
	{
	public:
		constexpr Socket() noexcept
			: myHandle(abi::InvalidSocket), isOut(false)
		{}

		constexpr Socket(Socket&& other) noexcept
			: myHandle(static_cast<::SOCKET&&>(other.myHandle)), isOut(false)
		{
			other.isOut = true;
		}

		constexpr Socket& operator=(Socket&& other) noexcept
		{
			myHandle = static_cast<::SOCKET&&>(other.myHandle);
			other.isOut = true;

			return *this;
		}

		explicit constexpr Socket(const ::SOCKET& handle) noexcept
			: myHandle(handle), isOut(false)
		{}

		explicit constexpr Socket(::SOCKET&& handle) noexcept
			: myHandle(static_cast<::SOCKET&&>(handle)), isOut(false)
		{}

		~Socket() noexcept
		{
			if (!isOut)
			{
				::closesocket(myHandle);
			}
		}

		util::Monad<int> BeginRecv(WSABUF& buffer, Context* context, unsigned* bytes = nullptr, unsigned flags = 0) noexcept
		{
			const int result = ::WSARecv(myHandle, &buffer, 1, bytes, &flags, context, nullptr);
			if (debug::CheckError(result))
			{
				const int error = ::WSAGetLastError();
				if (error != WSA_IO_PENDING)
				{
					delete context;
					return util::Monad<int>::Error(error);
				}
			}
			return util::Monad<int>::Ok(bytes);
		}

		template<typename Option = ios::immediately_t>
		[[nodiscard]]
		inline IoState Bind(const EndPoint& address, Option&& io = {})
		{
			return IoState{ abi::Bind(Handle, *address), io };
		}

		template<typename Option = ios::immediately_t>
		[[nodiscard]]
		inline IoState Bind(EndPoint&& address, Option&& io = {})
		{
			return IoState{ abi::Bind(Handle, *util::move(address)), io };
		}

		template<typename Option = ios::immediately_t>
		[[nodiscard]]
		inline IoState Connect(const ::SOCKADDR* const address, int addrlen, Option&& io = {})
		{
			return IoState{ ::connect(Handle, address, addrlen), io };
		}

		template<typename Option = ios::immediately_t>
		[[nodiscard]]
		inline IoState Listen(const int& backlog = constants::LISTEN_MAX, Option&& io = {}) const
		{
			return IoState{ ::listen(Handle, backlog), io };
		}

		template<typename Option = ios::immediately_t>
		[[nodiscard]]
		inline IoState Listen(int&& backlog = constants::LISTEN_MAX, Option&& io = {}) const
		{
			return IoState{ ::listen(Handle, util::move(backlog)), io };
		}

		inline constexpr bool IsValid() const noexcept
		{
			return myHandle != INVALID_SOCKET;
		}

		inline constexpr bool IsOut() const noexcept
		{
			return isOut;
		}

		Socket(const Socket& other) = delete;
		Socket& operator=(const Socket& other) = delete;

	private:
		::SOCKET myHandle;
		bool isOut;
	};
}
