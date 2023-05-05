module;
#define NOMINMAX
#include <WS2tcpip.h>
#include <MSWSock.h>

export module Net.Socket;
import Utility;
import Utility.Monad;
import Net;
import Net.EndPoint;
import Net.Context;

export namespace net
{
	class [[nodiscard]] Socket
	{
	public:
		constexpr Socket();

		Socket(const Socket& other) = delete;
		Socket& operator=(const Socket& other) = delete;
		Socket(Socket&& other) noexcept = default;
		Socket& operator=(Socket&& other) noexcept = default;
	};
}
