module;
#include <string>
#include <string_view>
#include <chrono>
export module Net:Setting;

using namespace std::chrono;
using namespace std::chrono_literals;

export namespace net::constants
{
	inline constexpr std::string_view VERSION = "0.0.2";

	inline constexpr unsigned long long BUFFSIZE = 512ULL;

	inline constexpr unsigned long long SERVER_ID = 0xffff;
	inline constexpr unsigned short SERVER_PORT = 54000ULL;
	inline constexpr unsigned short GAME_PORT_FIRST = SERVER_PORT + 1;
	inline constexpr unsigned short GAME_PORT_LAST = GAME_PORT_FIRST + 100;

	inline constexpr int LISTEN_MAX = 1000000;
	inline constexpr unsigned long long WORKERS_COUNT = 6ULL;

	inline constexpr milliseconds UPDATE_INTERVAL = milliseconds(45);
}

export namespace net
{
	inline constexpr const std::string_view& (GetServerVersion)() noexcept
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
