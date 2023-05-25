module;
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <string>
#include <string_view>
#include <chrono>
export module Net;

export namespace net
{
	using ::WSAOVERLAPPED;
	using ::WSABUF;
	using ::LPWSABUF;
	using ::SOCKET;

	class CompletionPort;
	class WorkerUnit;
	class Address;
	class EndPoint;
	class Socket;

	class Context;
	class SendContext;

	template<typename T, typename E, typename C>
	class Promise;
	template<size_t Size>
	class SocketStream;

	namespace constants
	{
		inline constexpr std::string_view VERSION = "0.0.1";

		inline constexpr unsigned long long SERVER_ID = 0xffff;
		inline constexpr unsigned short SERVER_PORT = 54000ULL;
		inline constexpr unsigned short GAME_PORT_FIRST = SERVER_PORT + 1;
		inline constexpr unsigned short GAME_PORT_LAST = GAME_PORT_FIRST + 100;

		inline constexpr int LISTEN_MAX = 1000000;

		inline constexpr std::chrono::milliseconds UPDATE_INTERVAL = std::chrono::milliseconds(45);
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

	inline constexpr const std::string_view& (GetServerVersion) () noexcept
	{
		return constants::VERSION;
	}

	inline constexpr unsigned long long GetServerID() noexcept
	{
		return constants::SERVER_ID;
	}

	inline constexpr std::chrono::milliseconds GetFrameTime() noexcept
	{
		return constants::UPDATE_INTERVAL;
	}
}
