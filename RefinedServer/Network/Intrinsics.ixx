module;
#include <WS2tcpip.h>

export module Net.Intrinsics;
import Utility;
import Utility.Constraints;
import Utility.Traits;
import Net;
import Net.IoState;
import Net.Promise;

export extern "C++" namespace net
{
#if true
	export namespace io
	{
		template<typename Fn, typename... Args>
		concept so_invocables = util::r_invocables<Fn, int, Args...>;

		template<typename Gateway, typename Fn, typename... Args>
			//requires util::r_invocables<Gateway, ioError, int>
		inline
			auto
			ExecuteVia(Gateway&& gateway, Fn&& fn, Args&&... args)
			noexcept(noexcept(util::forward<Gateway>(gateway)(util::forward<Fn>(fn)(util::declval<Args>()...))))
		{
			static_assert(util::invocables<Fn, Args...>, "The functor should be able to execute via args.");
			static_assert(util::r_invocables<Fn, int, Args...>, "The functor should return a int.");

#if _DEBUG
			int&& result = util::forward<Fn>(fn)(util::forward<Args>(args)...);

			return util::forward<Gateway>(gateway)(static_cast<int&&>(result));
#else // _DEBUG
			return util::forward<Fn>(fn)(util::forward<Args>(args)...);
#endif
		}

		template<typename Fn, typename... Args>
		inline
			int
			Execute(Fn&& fn, Args&&... args)
			noexcept(noexcept(util::forward<Fn>(fn)(util::declval<Args>()...)))
		{
			return util::forward<Fn>(fn)(util::forward<Args>(args)...);
		}

		template<typename Class, typename Method, typename... Args>
			requires so_invocables<Method, Args...>
		inline
			int
			Delegate(Method Class::* (&& action), Class& obj, SOCKET socket, Args&&... args)
			noexcept(noexcept((obj.*util::forward<Method Class::*>(action))(util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			return (obj.*util::forward<Method Class::*>(action))(socket, util::forward<Args>(args)...);
		}

		template<typename Class, typename Method, typename... Args>
			requires so_invocables<Method, Args...>
		inline
			int
			Delegate(Method Class::* (&& action), const Class& obj, SOCKET socket, Args&&... args)
			noexcept(noexcept((obj.*util::forward<Method Class::*>(action))(util::declval<SOCKET>(), util::declval<Args>()...)))
		{
			return (obj.*util::forward<Method Class::*>(action))(socket, util::forward<Args>(args)...);
		}
	}
#endif // false
}
