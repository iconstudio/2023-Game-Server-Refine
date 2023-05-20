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

	inline ioError CheckIO(const int& socket_fn_result) noexcept
	{
		if (debug::CheckError(socket_fn_result))
		{
			int error = ::WSAGetLastError();
			if (debug::CheckPending(error))
			{
				return io::defered;
			}
			else
			{
				return static_cast<int&&>(error);
			}
		}
		else
		{
			return io::success;
		}
	}

	inline ioError CheckBool(const int& bool_fn_result) noexcept
	{
		if (0 == bool_fn_result)
		{
			int error = ::WSAGetLastError();
			if (debug::CheckPending(error))
			{
				return io::defered;
			}
			else
			{
				return static_cast<int&&>(error);
			}
		}
		else
		{
			return io::success;
		}
	}

	class [[nodiscard]] Socket
	{
		struct init_t {};

		constexpr Socket(init_t) noexcept
		{
			optDebug.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::Debug, flag);
			});

			optReuseAddress.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::Recyclable, flag);
			});

			optDontRoute.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::DontRoute, flag);
			});

			optBroadcast.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::Broadcast, flag);
			});

			optUseLoopback.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::UseLoopback, flag);
			});

			optReuseAddress.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::Recyclable, flag);
			});

			optLinger.AddListener([this](const SoLinger& linger) {
				SetOptionByValue(SocketOptions::Linger, linger);
			});

			optNoDelay.AddListener([this](const bool& flag) {
				SetOption(SocketOptions::NoDelay, flag);
			});

			optUpdateContext.AddListener([this](Socket& other) {
				SetOptionByHandle(SocketOptions::UseLoopback, other);
			});
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

		inline ioError Bind(const EndPoint& target) noexcept
		{
			return CheckIO(::bind(myHandle, reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize()));
		}

		inline ioError Connect(const EndPoint& target) noexcept
		{
			return CheckIO(::connect(myHandle, reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize())).else_then([&]() {
				myEndPoint = target;
			});
		}

		inline ioError Connect(const ::SOCKADDR* const& address, const int& addrlen) const noexcept
		{
			return CheckIO(::connect(myHandle, address, addrlen));
		}

		inline ioError Listen(const int& backlog = constants::LISTEN_MAX) noexcept
		{
			return CheckIO(::listen(myHandle, backlog));
		}

		inline ioError Accept(const Socket& client, void* const& buffer, Context& context, unsigned long& result_bytes)
		{
			return Accept(client, buffer, util::addressof(context), util::addressof(result_bytes));
		}

		inline ioError Accept(const Socket& client, void* const& buffer, Context* const& context, unsigned long* result_bytes)
		{
			return CheckBool(
				::AcceptEx(myHandle, client.myHandle, buffer, 0UL
				, abi::DEFAULT_ACCEPT_SIZE, abi::DEFAULT_ACCEPT_SIZE, result_bytes
				, context)
			);
		}

		inline ioError Recv(WSABUF& buffer, Context* const& context, unsigned long* bytes = nullptr, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(::WSARecv(myHandle, util::addressof(buffer), 1UL, bytes, &flags, context, nullptr));
		}

		inline ioError Send(WSABUF buffer, Context* const& context, unsigned long* bytes = nullptr, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(::WSASend(myHandle, util::addressof(buffer), 1UL, bytes, flags, context, nullptr));
		}

		inline ioError BeginRecv(WSABUF& buffer, Context* const& context, CompletionRoutine routine, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(::WSARecv(myHandle, util::addressof(buffer), 1UL, nullptr, &flags, context, routine));
		}

		inline ioError BeginSend(WSABUF buffer, Context* const& context, CompletionRoutine routine, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(::WSASend(myHandle, util::addressof(buffer), 1UL, nullptr, flags, context, routine));
		}

		inline ioError SetOption(const SocketOptions& option, const bool& flag)
		{
			const int iflag = flag; // BOOL

			return CheckIO(::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&iflag), static_cast<int>(sizeof(iflag)))
			);
		}

		inline ioError SetOptionByHandle(const SocketOptions& option, Socket& other) noexcept
		{
			return CheckIO(::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&other.myHandle), static_cast<int>(sizeof(SOCKET)))
			);
		}

		template<typename T>
		inline ioError SetOptionByValue(const SocketOptions& option, const T& value)
		{
			return CheckIO(::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(T)))
			);
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

		util::Option<bool> optDebug{ false };
		util::Option<bool> optReuseAddress{ false };
		util::Option<bool> optKeepAlive{ false };
		util::Option<bool> optDontRoute{ false };
		util::Option<bool> optBroadcast{ false };
		util::Option<bool> optUseLoopback{ false };
		util::Option<bool> optNoDelay{ false };
		util::Option<EndPoint> optBindAddress{};
		util::Option<::linger> optLinger{};
		util::Option<Socket&> optUpdateContext{};

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
