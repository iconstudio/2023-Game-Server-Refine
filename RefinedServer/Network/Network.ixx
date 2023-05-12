module;
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net;
export import :Setting;

export namespace net
{
	using ::WSAOVERLAPPED;
	using ::WSABUF;
	using ::LPWSABUF;
	using ::SOCKET;

	class CompletionPort;
	class Address;
	class EndPoint;
	class Socket;

	class Context;
	template<typename T, typename E>
	class Promise;
	class SendContext;

	class BasicSession;
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

	namespace debug
	{
		inline constexpr
			bool
			CheckSocket(const unsigned long long& socket) noexcept
		{
			return INVALID_SOCKET != socket;
		}

		inline constexpr
			bool
			CheckError(const int& socket_fn_result) noexcept
		{
			return SOCKET_ERROR == socket_fn_result;
		}

		inline constexpr
			bool
			CheckError(int&& socket_fn_result) noexcept
		{
			return SOCKET_ERROR == static_cast<int&&>(socket_fn_result);
		}

		inline constexpr
			bool
			CheckError(const volatile int& socket_fn_result) noexcept
		{
			return SOCKET_ERROR == socket_fn_result;
		}

		inline constexpr
			bool
			CheckError(volatile int&& socket_fn_result) noexcept
		{
			return SOCKET_ERROR == static_cast<volatile int&&>(socket_fn_result);
		}

		inline constexpr
			bool
			CheckPending(const int& socket_error_code) noexcept
		{
			return ERROR_IO_PENDING == socket_error_code;
		}

		inline constexpr
			bool
			CheckPending(int&& socket_error_code) noexcept
		{
			return ERROR_IO_PENDING == static_cast<int&&>(socket_error_code);
		}

		inline constexpr
			bool
			CheckPending(const volatile int& socket_error_code) noexcept
		{
			return ERROR_IO_PENDING == socket_error_code;
		}

		inline constexpr
			bool
			CheckPending(volatile int&& socket_error_code) noexcept
		{
			return ERROR_IO_PENDING == static_cast<volatile int&&>(socket_error_code);
		}

		inline constexpr
			bool
			CheckIncomplete(const int& socket_error_code) noexcept
		{
			return ERROR_IO_INCOMPLETE == socket_error_code;
		}

		inline constexpr
			bool
			CheckIncomplete(int&& socket_error_code) noexcept
		{
			return ERROR_IO_INCOMPLETE == static_cast<int&&>(socket_error_code);
		}

		inline constexpr
			bool
			CheckIncomplete(const volatile int& socket_error_code) noexcept
		{
			return ERROR_IO_INCOMPLETE == socket_error_code;
		}

		inline constexpr
			bool
			CheckIncomplete(volatile int&& socket_error_code) noexcept
		{
			return ERROR_IO_INCOMPLETE == static_cast<volatile int&&>(socket_error_code);
		}

		using ::DWORD;
		using ::GetLastError;
		using ::WSAGetLastError;
	}
}
