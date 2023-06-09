module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket.Basic;
import Utility;
export import Net;
export import Net.Context;
export import Net.Socket.ABI;

export namespace net
{
	class BasicSocket
	{
	public:
		constexpr BasicSocket() noexcept
		{}

		constexpr BasicSocket(BasicSocket&& other) noexcept
			: BasicSocket()
		{
			myHandle = static_cast<SOCKET&&>(other.myHandle);
			other.isOut = true;
		}

		explicit constexpr BasicSocket(const SOCKET& handle) noexcept
			: isOut(false), myHandle(handle)
		{}

		explicit constexpr BasicSocket(SOCKET&& handle) noexcept
			: isOut(false), myHandle(static_cast<SOCKET&&>(handle))
		{}

		constexpr BasicSocket& operator=(BasicSocket&& other) noexcept
		{
			myHandle = static_cast<SOCKET&&>(other.myHandle);
			other.isOut = true;

			return *this;
		}

		~BasicSocket() noexcept
		{
			if (!isOut && IsValid())
			{
				::closesocket(myHandle);
				myHandle = abi::InvalidSocket;
			}
		}

		inline int Bind(const SOCKADDR& target, const int& size = sizeof(SOCKADDR)) noexcept
		{
			return ::bind(myHandle
				, &target, size);
		}

		inline int Bind(const SOCKADDR_IN& target) noexcept
		{
			return Bind(reinterpret_cast<const SOCKADDR*>(&target), sizeof(SOCKADDR_IN));
		}

		inline int Bind(const SOCKADDR* const& target, const int& size = sizeof(SOCKADDR)) noexcept
		{
			return ::bind(myHandle
				, target, size);
		}

		inline int Bind(const SOCKADDR_IN* const& target) noexcept
		{
			return Bind(reinterpret_cast<const SOCKADDR*>(target), sizeof(SOCKADDR_IN));
		}

		inline int Connect(const SOCKADDR& address) noexcept
		{
			return ::connect(myHandle
				, &address, sizeof(SOCKADDR));
		}

		inline int Connect(const SOCKADDR_IN& address) noexcept
		{
			return ::connect(myHandle
				, reinterpret_cast<const ::SOCKADDR*>(&address), sizeof(SOCKADDR_IN));
		}

		inline int Connect(const SOCKADDR* const& address) noexcept
		{
			return ::connect(myHandle
				, address, sizeof(SOCKADDR));
		}

		inline int Connect(const SOCKADDR_IN* const& address) noexcept
		{
			return ::connect(myHandle
				, reinterpret_cast<const ::SOCKADDR*>(address), sizeof(SOCKADDR_IN));
		}

		inline int Connect(const SOCKADDR* const& address, const int& addrlen) const noexcept
		{
			return ::connect(myHandle, address, addrlen);
		}

		inline int Listen(const int& backlog = constants::LISTEN_MAX) noexcept
		{
			return ::listen(myHandle, backlog);
		}

		inline int Accept(const SOCKET& target, void* const& buffer, unsigned long& result_bytes, Context* const& context)
		{
			return Accept(target, buffer
				, util::addressof(result_bytes)
				, context
			);
		}

		inline int Accept(const BasicSocket& client, void* const& buffer, unsigned long* result_bytes, Context* const& context)
		{
			return Accept(client.myHandle
				, buffer
				, result_bytes
				, context
			);
		}

		inline int Accept(const SOCKET& target, void* const& buffer, unsigned long* result_bytes, Context* const& context)
		{
			return ::AcceptEx(myHandle, target
				, buffer, 0UL
				, abi::DEFAULT_ACCEPT_SIZE
				, abi::DEFAULT_ACCEPT_SIZE
				, result_bytes
				, context
			);
		}

		inline int Recv(WSABUF& buffer, Context* const& context, unsigned long* bytes = nullptr, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return ::WSARecv(myHandle
				, util::addressof(buffer), 1UL
				, bytes
				, &flags
				, context, nullptr
			);
		}

		inline int Send(WSABUF buffer, Context* const& context, unsigned long* bytes = nullptr, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return ::WSASend(myHandle
				, util::addressof(buffer), 1UL
				, bytes
				, flags
				, context, nullptr
			);
		}

		inline int BeginRecv(WSABUF& buffer, Context* const& context, LPWSAOVERLAPPED_COMPLETION_ROUTINE routine, unsigned long flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return ::WSARecv(myHandle
				, util::addressof(buffer), 1UL
				, nullptr
				, &flags
				, context, routine
			);
		}

		inline int BeginSend(WSABUF buffer, Context* const& context, LPWSAOVERLAPPED_COMPLETION_ROUTINE routine, const unsigned long& flags = 0) noexcept
		{
			if (buffer.len <= 0)
			{
				return -1;
			}

			return ::WSASend(myHandle
				, util::addressof(buffer), 1UL
				, nullptr
				, flags
				, context, routine
			);
		}

		inline int SetOption(const SocketOptions& option, const bool& flag)
		{
			const int iflag = flag; // BOOL

			return ::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&iflag), static_cast<int>(sizeof(iflag))
			);
		}

		inline int SetOptionByHandle(const SocketOptions& option, BasicSocket& other) noexcept
		{
			return ::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&other.myHandle), static_cast<int>(sizeof(SOCKET))
			);
		}

		template<typename T>
		inline int SetOptionByValue(const SocketOptions& option, const T& value)
		{
			return ::setsockopt(myHandle, SOL_SOCKET
				, static_cast<int>(option)
				, reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(T))
			);
		}

		template<typename Control>
		[[nodiscard]]
		inline int AcquireIoControl(Control& ptr, const DWORD& code, GUID uid, PDWORD bytes, Context* overlap = nullptr) const
		{
			if constexpr (0 == sizeof(Control))
			{
				return -1;
			}

			if (nullptr == bytes)
			{
				return -1;
			}

			if (0 == code)
			{
				return -1;
			}

			return ::WSAIoctl(myHandle
				, code
				, &uid, sizeof(uid)
				, &ptr, sizeof(ptr)
				, bytes
				, overlap, nullptr);
		}

		template<typename Fn>
		[[nodiscard]]
		inline int AcquireIoMethod(Fn& method_ptr, GUID uid, PDWORD bytes, Context* overlap = nullptr) const
		{
			return
				this->AcquireIoControl<Fn>(method_ptr
				, SocketIos::SocketIoForGettingFunction
				, uid
				, bytes, overlap);
		}

		[[nodiscard]]
		constexpr SOCKET Handle() const& noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr SOCKET&& Handle() && noexcept
		{
			return static_cast<SOCKET&&>(myHandle);
		}

		[[nodiscard]]
		constexpr bool IsValid() const noexcept
		{
			return myHandle != abi::InvalidSocket;
		}

		BasicSocket(const BasicSocket& other) = delete;
		BasicSocket& operator=(const BasicSocket& other) = delete;

	protected:
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
	};
}
