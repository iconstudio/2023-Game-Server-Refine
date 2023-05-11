export module Net.Intrinsics;
import Utility;
import Utility.Monad;
import Utility.Constraints;
import Utility.Traits;
import Net;
import Net.Promise;

export namespace net
{
	using ioError = util::Monad<int>;

	export namespace io
	{
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

		inline constexpr ioError CheckHandle(const void* const& handle) noexcept
		{
			if (nullptr == handle)
			{
				return util::nullopt;
			}
			else
			{
				return ioError{ debug::WSAGetLastError() };
			}
		}

		inline constexpr ioError CheckHandle(void*&& handle) noexcept
		{
			if (nullptr == static_cast<void*&&>(handle))
			{
				return util::nullopt;
			}
			else
			{
				return ioError{ debug::WSAGetLastError() };
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

		template<typename Fn, typename... Args>
		concept io_invocables = util::r_invocables<Fn, int, SOCKET, Args...>;

		template<typename Fn, typename... Args>
			requires io_invocables<Fn, Args...>
		inline
			ioError
			Execute(Fn&& fn, SOCKET socket, Args&&... args)
			noexcept(noexcept(util::invoke(util::forward<Fn>(fn), util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			return CheckIO(util::forward<Fn>(fn)(socket, util::forward<Args>(args)...));
		}

		template<typename Class, typename Method, typename... Args>
			requires io_invocables<Method, Args...>
		inline
			ioError
			Delegate(Method Class::* (&& action), Class& obj, SOCKET socket, Args&&... args)
			noexcept(noexcept((obj.*util::forward<Method Class::*>(action))(util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			int&& result = (obj.*util::forward<Method Class::*>(action))(socket, util::forward<Args>(args)...);

			return CheckIO(static_cast<int&&>(result));
		}

		template<typename Class, typename Method, typename... Args>
			requires io_invocables<Method, Args...>
		inline
			ioError
			Delegate(Method Class::* (&& action), const Class& obj, SOCKET socket, Args&&... args)
			noexcept(noexcept((obj.*util::forward<Method Class::*>(action))(util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			int&& result = (obj.*util::forward<Method Class::*>(action))(socket, util::forward<Args>(args)...);

			return CheckIO(static_cast<int&&>(result));
		}
	}
}
