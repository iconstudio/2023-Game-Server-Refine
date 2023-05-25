export module Net.Promise;
import Utility;
import Utility.Constraints;
import Net;
export import Net.IoState;

export namespace net
{
	template<typename Fn, typename U>
	struct noexcept_t
	{
		template<typename... Args>
		static consteval bool Eval() noexcept
		{
			if constexpr (!util::same_as<U, void>)
			{
				return util::nothrow_invocables<Fn>;
			}
			else
			{
				return util::nothrow_invocables<Fn, Args...>;
			}
		}

		template<typename... Args>
			requires (util::same_as<U, void>)
		static constexpr auto Execute(Fn&& functor, [[maybe_unused]] Args...) noexcept(noexcept(util::forward<Fn>(functor)()))
		{
			return util::forward<Fn>(functor)();
		}

		template<typename... Args>
			requires (!util::same_as<U, void>)
		static constexpr auto Execute(Fn&& functor, Args&&... args) noexcept(noexcept(util::forward<Fn>(functor)(util::forward<Args>(args)...)))
		{
			return util::forward<Fn>(functor)(util::forward<Args>(args)...);
		}
	};

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">Sucess</typeparam>
	/// <typeparam name="E">Error</typeparam>
	/// <typeparam name="C">Cause of Defer</typeparam>
	template<typename T, typename E = void, typename C = void>
	class Promise
	{
	public:
		using succeed_t = io::success_t<T>;
		using failure_t = io::failure_t<E>;
		using defered_t = io::defered_t<C>;

		constexpr Promise() noexcept
		{}

		constexpr Promise(util::nullopt_t) noexcept
		{}

		template<typename U>
			requires (util::notvoids<T>&& util::same_as<util::clean_t<U>, T>)
		constexpr Promise(U&& pass)
			noexcept(util::nothrow_constructibles<T, U&&>)
			: mySucceed(util::forward<U>(pass))
		{}

		template<typename U>
			requires (util::notvoids<E>&& util::same_as<util::clean_t<U>, E>)
		constexpr Promise(U&& fail)
			noexcept(util::nothrow_constructibles<E, U&&>)
			: myFailure(util::forward<U>(fail))
		{}

		template<typename U>
			requires (util::notvoids<C>&& util::same_as<util::clean_t<U>, C>)
		constexpr Promise(U&& cause)
			noexcept(util::nothrow_constructibles<C, U&&>)
			: myDefered(util::forward<U>(cause))
		{}

		constexpr Promise(const Promise& other) noexcept
			: myIndex(other.myIndex)
		{
			switch (other.myIndex)
			{
				case 1:
				{
					mySucceed = other.mySucceed;
				}
				break;
				case 2:
				{
					myFailure = other.myFailure;
				}
				break;
				case 3:
				{
					myDefered = other.myDefered;
				}
				break;
			}
		}

		constexpr Promise(Promise&& other) noexcept
			: myIndex(static_cast<size_t&&>(other.myIndex))
		{
			switch (other.myIndex)
			{
				case 1:
				{
					mySucceed = static_cast<succeed_t&&>(other.mySucceed);
				}
				break;
				case 2:
				{
					myFailure = static_cast<failure_t&&>(other.myFailure);
				}
				break;
				case 3:
				{
					myDefered = static_cast<defered_t&&>(other.myDefered);
				}
				break;
			}
		}

		constexpr Promise(const succeed_t& ok)
			noexcept(util::nothrow_copy_constructibles<T>)
			: mySucceed(ok), myIndex(1)
		{}

		constexpr Promise(succeed_t&& ok)
			noexcept(util::nothrow_move_constructibles<T>)
			: mySucceed(static_cast<succeed_t&&>(ok)), myIndex(1)
		{}

		constexpr Promise(const failure_t& error)
			noexcept(util::nothrow_copy_constructibles<E>)
			: myFailure(error), myIndex(2)
		{}

		constexpr Promise(failure_t&& error)
			noexcept(util::nothrow_move_constructibles<E>)
			: myFailure(static_cast<failure_t&&>(error)), myIndex(2)
		{}

		constexpr Promise(const defered_t& cause)
			noexcept(util::nothrow_copy_constructibles<C>)
			: myDefered(cause), myIndex(3)
		{}

		constexpr Promise(defered_t&& cause)
			noexcept(util::nothrow_move_constructibles<C>)
			: myDefered(static_cast<failure_t&&>(cause)), myIndex(3)
		{}

		constexpr Promise& operator=(util::nullopt_t) noexcept
		{
			myIndex = 0;
			return *this;
		}

		constexpr Promise& operator=(const succeed_t& ok)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			if (0 == myIndex || IsSuccess())
			{
				mySucceed = ok;
				myIndex = 1;
			}
			return *this;
		}

		constexpr Promise& operator=(succeed_t&& ok)
			noexcept(util::nothrow_move_constructibles<T>)
		{
			if (0 == myIndex || IsSuccess())
			{
				mySucceed = static_cast<succeed_t&&>(ok);
				myIndex = 1;
			}
			return *this;
		}

		constexpr Promise& operator=(const failure_t& error)
			noexcept(util::nothrow_copy_constructibles<E>)
		{
			if (0 == myIndex || IsFailed())
			{
				myFailure = error;
				myIndex = 2;
			}
			return *this;
		}

		constexpr Promise& operator=(failure_t&& error)
			noexcept(util::nothrow_move_constructibles<E>)
		{
			if (0 == myIndex || IsFailed())
			{
				myFailure = static_cast<failure_t&&>(error);
				myIndex = 2;
			}
			return *this;
		}

		constexpr Promise& operator=(const defered_t& cause)
			noexcept(util::nothrow_copy_constructibles<C>)
		{
			if (0 == myIndex || IsDefered())
			{
				myDefered = cause;
				myIndex = 3;
			}
			return *this;
		}

		constexpr Promise& operator=(defered_t&& cause)
			noexcept(util::nothrow_move_constructibles<C>)
		{
			if (0 == myIndex || IsDefered())
			{
				myDefered = static_cast<defered_t&&>(cause);
				myIndex = 3;
			}
			return *this;
		}

		constexpr ~Promise()
			noexcept(util::nothrow_destructibles<T, E, C>)
		{}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<typename Fn>
		inline friend constexpr
			auto
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_lvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(promise.GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_lvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<typename Fn>
		inline friend constexpr
			auto
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_clvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(promise.GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_clvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<typename Fn>
		inline friend constexpr
			auto
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_rvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(util::move(promise).GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_rvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<typename Fn>
		inline friend constexpr
			auto
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_crvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(util::move(promise).GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_crvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::invocables Fn>
		inline friend constexpr
			auto
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::invocables Fn>
		inline friend constexpr
			auto
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::invocables Fn>
		inline friend constexpr
			auto
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::invocables Fn>
		inline friend constexpr
			auto
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		template<typename Fn>
		inline friend constexpr
			auto
			operator<<(Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_lvalue_t<E>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, E, util::make_lvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(promise.GetError());
				}
			}
			else
			{
				return util::fn_result_t<Fn, E, util::make_lvalue_t>{};
			}
		}

		template<typename Fn>
		inline friend constexpr
			auto
			operator<<(const Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_clvalue_t<E>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, E, util::make_clvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(promise.GetError());
				}
			}
			else
			{
				return util::fn_result_t<Fn, E, util::make_clvalue_t>{};
			}
		}

		template<typename Fn>
		inline friend constexpr
			auto
			operator<<(Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_rvalue_t<E>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, E, util::make_rvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(static_cast<Promise&&>(promise).GetError());
				}
			}
			else
			{
				return util::fn_result_t<Fn, E, util::make_rvalue_t>{};
			}
		}

		template<typename Fn>
		inline friend constexpr
			auto
			operator<<(const Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_crvalue_t<E>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, E, util::make_crvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(static_cast<const Promise&&>(promise).GetError());
				}
			}
			else
			{
				return util::fn_result_t<Fn, E, util::make_crvalue_t>{};
			}
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_lvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(GetResult());
				}
			}

			return *this;
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_clvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(GetResult());
				}
			}

			return *this;
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_rvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(util::move(*this).GetResult());
				}
			}

			return util::move(*this);
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_crvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(util::move(*this).GetResult());
				}
			}

			return util::move(*this);
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				util::forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				util::forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_lvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, T, util::make_lvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_lvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_clvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, T, util::make_clvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_clvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_rvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, T, util::make_rvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(util::move(*this).GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_rvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<typename Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_crvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::fn_result_t<Fn, T, util::make_crvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (util::same_as<T, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(util::move(*this).GetResult());
				}
			}
			else
			{
				return util::fn_result_t<Fn, T, util::make_crvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			Promise&
			else_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				util::forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				util::forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			Promise&&
			else_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			const Promise&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::lv_invocable<E> Fn>
		constexpr
			Promise&
			else_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_lvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(GetError());
				}
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cl_invocable<E> Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_clvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(GetError());
				}
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::rv_invocable<E> Fn>
		constexpr
			Promise&&
			else_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_rvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(util::move(*this).GetError());
				}
			}

			return util::move(*this);
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cr_invocable<E> Fn>
		constexpr
			const Promise&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_crvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					util::forward<Fn>(action)();
				}
				else
				{
					util::forward<Fn>(action)(util::move(*this).GetError());
				}
			}

			return util::move(*this);
		}

		/// <summary>
		/// or_else, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			or_else(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			or_else(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			or_else(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			auto
			or_else(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::lv_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_lvalue_t<E>>()))
		{
			using fwd_result_t = util::fn_result_t<Fn, E, util::make_lvalue_t>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return util::forward<Fn>(action)(GetError());
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cl_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_clvalue_t<E>>()))
		{
			using fwd_result_t = util::fn_result_t<Fn, E, util::make_clvalue_t>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return util::forward<Fn>(action)(GetError());
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::rv_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_rvalue_t<E>>()))
		{
			using fwd_result_t = util::fn_result_t<Fn, E, util::make_rvalue_t>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return util::forward<Fn>(action)(util::move(*this).GetError());
			}
			else
			{
				return fwd_result_t{};
			}
		}

		/// <summary>
		/// or_else, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cr_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<util::make_crvalue_t<E>>()))
		{
			using fwd_result_t = util::fn_result_t<Fn, E, util::make_crvalue_t>;

			static_assert(!util::same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				if constexpr (util::same_as<E, void>)
				{
					return util::forward<Fn>(action)();
				}
				else
				{
					return util::forward<Fn>(action)(util::move(*this).GetError());
				}
			}
			else
			{
				return fwd_result_t{};
			}
		}

		constexpr succeed_t& GetResult() & noexcept
			requires util::notvoids<T>
		{
			return mySucceed;
		}

		constexpr const succeed_t& GetResult() const& noexcept
			requires util::notvoids<T>
		{
			return mySucceed;
		}

		constexpr succeed_t&& GetResult() && noexcept
			requires util::notvoids<T>
		{
			return static_cast<succeed_t&&>(mySucceed);
		}

		constexpr const succeed_t&& GetResult() const&& noexcept
			requires util::notvoids<T>
		{
			return static_cast<const succeed_t&&>(mySucceed);
		}

		constexpr void GetResult() const noexcept
			requires (!util::notvoids<T>)
		{
			static_assert(util::always_false<T>, "T is void.");
		}

		constexpr failure_t& GetError() & noexcept
			requires util::notvoids<E>
		{
			return myFailure;
		}

		constexpr const failure_t& GetError() const& noexcept
			requires util::notvoids<E>
		{
			return myFailure;
		}

		constexpr failure_t&& GetError() && noexcept
			requires util::notvoids<E>
		{
			return static_cast<failure_t&&>(myFailure);
		}

		constexpr const failure_t&& GetError() const&& noexcept
			requires util::notvoids<E>
		{
			return static_cast<const failure_t&&>(myFailure);
		}

		constexpr void GetError() const noexcept
			requires (!util::notvoids<E>)
		{
			static_assert(util::always_false<E>, "E is void.");
		}

		constexpr bool IsSuccess() const noexcept
		{
			return myIndex == 1;
		}

		constexpr bool IsFailed() const noexcept
		{
			return myIndex == 2;
		}

		constexpr bool IsDefered() const noexcept
		{
			return myIndex == 3;
		}

	private:
		union
		{
			succeed_t mySucceed; // 1
			failure_t myFailure; // 2
			defered_t myDefered; // 3
		};
		size_t myIndex = 0;
	};

	using Proxy = Promise<void, void, void>;

	using ioError = Promise<void, int, void>;

	template<typename T>
	Promise(T) -> Promise<T, void, void>;

	template<typename T, typename E>
	Promise(T, E) -> Promise<T, E, void>;

	template<typename T, typename E, typename C>
	Promise(T, E, C) -> Promise<T, E, C>;
}

#pragma warning(push, 1)
namespace net::test
{
#if false
	void test_promise() noexcept
	{
		constexpr auto fnl0 = [](const int& v) -> int {
			return 300;
		};

		constexpr auto fnr0 = [](int&&) -> int {
			return 300;
		};

		Promise<int> vpromise0{};
		const auto r0 = vpromise0 >> fnl0;
		Promise<long long> vpromise1{};

		constexpr Promise<int, void> cvpromise0{};

		constexpr Promise<long long, void> cvpromise1{};

		constexpr Proxy proxy0{};
	}
#endif // false
}
#pragma warning(pop)
