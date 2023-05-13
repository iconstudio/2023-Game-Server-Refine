export module Net.Intrinsics;
import Utility;
import Utility.Constraints;
import Utility.Traits;
import Net;
export import Net.Promise;

export namespace net
{
	using ioError = Promise<void, int>;

	inline ioError CheckPending() noexcept
	{
		int error = debug::WSAGetLastError();
		if (debug::CheckPending(error))
		{
			return util::nullopt;
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
			return util::nullopt;
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
			return util::nullopt;
		}
	}

	export namespace io
	{
		template<typename Fn, typename... Args>
		concept so_invocables = util::r_invocables<Fn, int, Args...>;

		template<typename Fn, typename... Args>
		inline
			ioError
			Execute(Fn&& fn, Args&&... args)
			noexcept(noexcept(util::forward<Fn>(fn)(util::declval<Args>()...)))
		{
			static_assert(util::invocables<Fn, Args...>, "The functor should be able to execute via args.");
			static_assert(util::r_invocables<Fn, int, Args...>, "The functor should return a int.");

#if _DEBUG
			int&& result = util::forward<Fn>(fn)(util::forward<Args>(args)...);

			return CheckIO(static_cast<int&&>(result));
#else // _DEBUG
			return CheckIO(util::forward<Fn>(fn)(util::forward<Args>(args)...));
#endif
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
