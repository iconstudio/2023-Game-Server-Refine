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
export import Net.Socket.ABI;
export import Net.Socket.Basic;

namespace net
{
	export inline ioError CheckIO(const int& socket_fn_result) noexcept
	{
		if (debug::CheckError(socket_fn_result))
		{
			int error = WSAGetLastError();
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

	export inline ioError CheckBool(const int& bool_fn_result) noexcept
	{
		if (0 == bool_fn_result)
		{
			int error = WSAGetLastError();
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

	export class [[nodiscard]] Socket : public BasicSocket
	{
	public:
		constexpr Socket() noexcept
			: BasicSocket()
		{
			InitializeOptions();
		}

		constexpr Socket(Socket&& other) noexcept
			: BasicSocket(static_cast<BasicSocket&&>(other))
		{
			InitializeOptions();
		}

		explicit constexpr Socket(const SOCKET& handle) noexcept
			: BasicSocket(handle)
		{
			InitializeOptions();
		}

		explicit constexpr Socket(SOCKET&& handle) noexcept
			: BasicSocket(static_cast<SOCKET&&>(handle))
		{
			InitializeOptions();
		}

		~Socket() noexcept = default;

		inline ioError Bind(const EndPoint& target) noexcept
		{
			return CheckIO(BasicSocket::Bind(reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize()));
		}

		inline ioError Connect(const EndPoint& target) noexcept
		{
			return CheckIO(BasicSocket::Connect(reinterpret_cast<const ::SOCKADDR*>(target.GetAddress()), target.GetiSize())).else_then([&]() {
				myEndPoint = target;
			});
		}

		inline ioError Connect(const ::SOCKADDR* const& address, const int& addrlen) const noexcept
		{
			return CheckIO(BasicSocket::Connect(address, addrlen));
		}

		inline ioError Listen(const int& backlog = constants::LISTEN_MAX) noexcept
		{
			return CheckIO(BasicSocket::Listen(backlog));
		}

		inline ioError Accept(const Socket& client, void* const& buffer, Context& context, unsigned long& result_bytes)
		{
			return Accept(client, buffer, util::addressof(context), util::addressof(result_bytes));
		}

		inline ioError Accept(const Socket& client, void* const& buffer, Context* const& context, unsigned long* result_bytes)
		{
			return CheckBool(
				BasicSocket::Accept(client.myHandle, buffer, context, result_bytes)
			);
		}

		inline ioError Recv(WSABUF& buffer, Context* const& context, unsigned long* bytes = nullptr, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(BasicSocket::Recv(buffer, context, bytes, flags));
		}

		inline ioError Send(WSABUF buffer, Context* const& context, unsigned long* bytes = nullptr, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(BasicSocket::Send(buffer, context, bytes, flags));
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
		Socket& operator=(const Socket& other) = delete;
		Socket& operator=(Socket&& other) = default;

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
		constexpr void InitializeOptions() noexcept
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

		EndPoint myEndPoint{};
	};
}
