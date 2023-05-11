export module Net.Intrinsics;
import Utility;
import Utility.Constraints;
import Utility.Traits;
import Net;
import Net.Promise;

export namespace net
{
	using ioError = ErrorHandler<void>;

	inline ioError CheckPending() noexcept
	{
		int error = debug::WSAGetLastError();
		if (debug::CheckPending(error))
		{
			return io::success;
		}
		else
		{
			return static_cast<int&&>(error);
		}
	}

	inline ioError CheckIncomplete() noexcept
	{
		int error = debug::WSAGetLastError();
		if (debug::CheckIncomplete(error))
		{
			return io::success;
		}
		else
		{
			return static_cast<int&&>(error);
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
			return io::success;
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
			return io::success;
		}
	}

	export namespace io
	{
		template<typename Fn, typename... Args>
		concept so_invocables = util::r_invocables<Fn, int, SOCKET, Args...>;

		template<typename Fn, typename... Args>
			requires so_invocables<Fn, Args...>
		inline
			ioError
			Execute(Fn&& fn, SOCKET socket, Args&&... args)
			noexcept(noexcept(util::invoke(util::forward<Fn>(fn), util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			return CheckIO(util::forward<Fn>(fn)(socket, util::forward<Args>(args)...));
		}

		template<typename Class, typename Method, typename... Args>
			requires so_invocables<Method, Args...>
		inline
			ioError
			Delegate(Method Class::* (&& action), Class& obj, SOCKET socket, Args&&... args)
			noexcept(noexcept((obj.*util::forward<Method Class::*>(action))(util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			int&& result = (obj.*util::forward<Method Class::*>(action))(socket, util::forward<Args>(args)...);

			return CheckIO(static_cast<int&&>(result));
		}

		template<typename Class, typename Method, typename... Args>
			requires so_invocables<Method, Args...>
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
