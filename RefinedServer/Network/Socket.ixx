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
import Net.Intrinsics;
export import Net.Socket.ABI;
export import Net.Socket.Basic;

namespace net
{
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

		inline ioError Accept(const Socket& client, void* const& buffer, unsigned long& result_bytes, Context& context)
		{
			return Accept(client, buffer, util::addressof(result_bytes), context);
		}

		inline ioError Accept(const Socket& client, void* const& buffer, unsigned long* const& result_bytes, Context& context)
		{
			return CheckBool(
				BasicSocket::Accept(client.Handle(), buffer, result_bytes, util::addressof(context))
			);
		}

		inline ioError Accept(const Socket& client, void* const& buffer, unsigned long* const& result_bytes, Context* const& context)
		{
			return CheckBool(
				BasicSocket::Accept(client.Handle(), buffer, result_bytes, context)
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

			return CheckIO(BasicSocket::BeginRecv(buffer, context, routine, flags));
		}

		inline ioError BeginSend(WSABUF buffer, Context* const& context, CompletionRoutine routine, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return CheckIO(BasicSocket::BeginSend(buffer, context, routine, flags));
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
