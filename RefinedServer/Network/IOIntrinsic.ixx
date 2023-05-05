module;
#include <WS2tcpip.h>

export module Net.IOIntrinsic;
import Utility;
import Utility.Monad;
import Utility.Constraints;
import Utility.Traits;
import Net;

export namespace net
{
	using ioError = util::Monad<int>;

	inline ioError CheckPending() noexcept
	{
		int error = debug::WSAGetLastError();
		if (debug::CheckPending(error))
		{
			return util::nullopt;
		}
		else
		{
			return ioError{ static_cast<int&&>(error) };
		}
	}

	inline ioError CheckIncomplete() noexcept
	{
		int error = debug::WSAGetLastError();
		if (debug::CheckIncomplete(error))
		{
			return util::nullopt;
		}
		else
		{
			return ioError{ static_cast<int&&>(error) };
		}
	}

	inline ioError CheckIO(const int& socket_fn_result) noexcept
	{
		if (debug::CheckError(socket_fn_result))
		{
			return CheckPending();
		}
		else
		{
			return util::nullopt;
		}
	}

	inline ioError CheckIO(int&& socket_fn_result) noexcept
	{
		if (debug::CheckError(static_cast<int&&>(socket_fn_result)))
		{
			return CheckPending();
		}
		else
		{
			return util::nullopt;
		}
	}

	class [[nodiscard]] alignas(unsigned long long) ioIntrinsic : util::Monad<int>
	{
	public:
		using Monad::Monad;

		ioIntrinsic(const int& socket_fn_result) noexcept
			: Monad()
		{
			if (debug::CheckError(socket_fn_result))
			{
				int error = debug::WSAGetLastError();

				if (!debug::CheckPending(error))
				{
					value() = ::WSAGetLastError();
				}
			}
		}

		int errorCode;
	};

	namespace io
	{
		template<typename Fn, typename... Args>
			requires util::r_invocables<Fn, int, ::SOCKET, Args...>
		inline
			int
			Forward(Fn&& fn, ::SOCKET socket, Args&&... args)
			noexcept(noexcept(util::invoke(util::forward<Fn>(fn), util::declval<::SOCKET>(), util::declval<Args>(args)...)))
		{
			return util::invoke(util::forward<Fn>(fn), socket, util::forward<Args>(args)...);
		}
	}
}
