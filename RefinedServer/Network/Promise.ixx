export module Net.Promise;
import Utility;
import Utility.Constraints;

using namespace util;

export namespace net
{
	namespace io
	{
		namespace tags
		{
			enum class io_success {};
			enum class io_failure {};
			enum class io_defered {};
		}

		namespace detail
		{
			template<typename T> // T: Success Handle
			struct success : util::type_identity<T>
			{};

			template<>
			struct success<void>
			{
				using type = tags::io_success;
			};

			template<typename E> // E: Error
			struct failure : util::type_identity<E>
			{};

			template<>
			struct failure<void>
			{
				using type = tags::io_failure;
			};

			template<typename C> // C: Cause of Defer
			struct defered : util::type_identity<C>
			{};

			template<>
			struct defered<void>
			{
				using type = tags::io_defered;
			};
		}

		template<typename T> // T: Success Handle
		using success_t = typename detail::template success<T>::type;
		template<typename E> // E: Error
		using failure_t = typename detail::template failure<E>::type;
		template<typename C> // C: Cause of Defer
		using defered_t = typename detail::template defered<C>::type;

		using just_success_t = success_t<void>;
		using just_failure_t = failure_t<void>;
		using just_defered_t = defered_t<void>;

		inline constexpr just_success_t success{ };
		inline constexpr just_failure_t failure{ };
		inline constexpr just_defered_t defered{ };
	}

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

		constexpr Promise(nullopt_t) noexcept
		{}

		template<typename U>
			requires (notvoids<T>&& same_as<clean_t<U>, T>)
		constexpr Promise(U&& pass)
			noexcept(nothrow_constructibles<T, U&&>)
			: mySucceed(forward<U>(pass))
		{}

		template<typename U>
			requires (notvoids<E>&& same_as<clean_t<U>, E>)
		constexpr Promise(U&& fail)
			noexcept(nothrow_constructibles<E, U&&>)
			: myFailure(forward<U>(fail))
		{}

		template<typename U>
			requires (notvoids<C>&& same_as<clean_t<U>, C>)
		constexpr Promise(U&& cause)
			noexcept(nothrow_constructibles<C, U&&>)
			: myDefered(forward<U>(cause))
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
			noexcept(nothrow_copy_constructibles<T>)
			: mySucceed(ok), myIndex(1)
		{}

		constexpr Promise(succeed_t&& ok)
			noexcept(nothrow_move_constructibles<T>)
			: mySucceed(static_cast<succeed_t&&>(ok)), myIndex(1)
		{}

		constexpr Promise(const failure_t& error)
			noexcept(nothrow_copy_constructibles<E>)
			: myFailure(error), myIndex(2)
		{}

		constexpr Promise(failure_t&& error)
			noexcept(nothrow_move_constructibles<E>)
			: myFailure(static_cast<failure_t&&>(error)), myIndex(2)
		{}

		constexpr Promise(const defered_t& cause)
			noexcept(nothrow_copy_constructibles<C>)
			: myDefered(cause), myIndex(3)
		{}

		constexpr Promise(defered_t&& cause)
			noexcept(nothrow_move_constructibles<C>)
			: myDefered(static_cast<failure_t&&>(cause)), myIndex(3)
		{}

		constexpr Promise& operator=(nullopt_t) noexcept
		{
			myIndex = 0;
			return *this;
		}

		constexpr Promise& operator=(const succeed_t& ok) noexcept
		{
			if (0 == myIndex || IsSuccess())
			{
				mySucceed = ok;
				myIndex = 1;
			}
			return *this;
		}

		constexpr Promise& operator=(succeed_t&& ok) noexcept
		{
			if (0 == myIndex || IsSuccess())
			{
				mySucceed = static_cast<succeed_t&&>(ok);
				myIndex = 1;
			}
			return *this;
		}

		constexpr Promise& operator=(const failure_t& error) noexcept
		{
			if (0 == myIndex || IsFailed())
			{
				myFailure = error;
				myIndex = 2;
			}
			return *this;
		}

		constexpr Promise& operator=(failure_t&& error) noexcept
		{
			if (0 == myIndex || IsFailed())
			{
				myFailure = static_cast<failure_t&&>(error);
				myIndex = 2;
			}
			return *this;
		}

		constexpr Promise& operator=(const defered_t& cause) noexcept
		{
			if (0 == myIndex || IsDefered())
			{
				myDefered = cause;
				myIndex = 3;
			}
			return *this;
		}

		constexpr Promise& operator=(defered_t&& cause) noexcept
		{
			if (0 == myIndex || IsDefered())
			{
				myDefered = static_cast<defered_t&&>(cause);
				myIndex = 3;
			}
			return *this;
		}

		constexpr ~Promise()
			noexcept(nothrow_destructibles<T, E, C>)
		{}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<lv_invocable<T> Fn>
		inline friend constexpr
			auto
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_lvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(promise.GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_lvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<cl_invocable<T> Fn>
		inline friend constexpr
			auto
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_clvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(promise.GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_clvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<rv_invocable<T> Fn>
		inline friend constexpr
			auto
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_rvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(move(promise).GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_rvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, parameter T (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<cr_invocable<T> Fn>
		inline friend constexpr
			auto
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_crvalue_t<T>>()))
		{
			if (promise.IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(move(promise).GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_crvalue_t>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<invocables Fn>
		inline friend constexpr
			auto
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<invocables Fn>
		inline friend constexpr
			auto
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<invocables Fn>
		inline friend constexpr
			auto
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// operator>>, no parameter (not failed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<invocables Fn>
		inline friend constexpr
			auto
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!promise.IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		template<lv_invocable<E> Fn>
		inline friend constexpr
			auto
			operator<<(Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_lvalue_t<E>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, E, make_lvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(promise.GetError());
				}
			}
			else
			{
				return fn_result_t<Fn, E, make_lvalue_t>{};
			}
		}

		template<cl_invocable<E> Fn>
		inline friend constexpr
			auto
			operator<<(const Promise& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_clvalue_t<E>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, E, make_clvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(promise.GetError());
				}
			}
			else
			{
				return fn_result_t<Fn, E, make_clvalue_t>{};
			}
		}

		template<rv_invocable<E> Fn>
		inline friend constexpr
			auto
			operator<<(Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_rvalue_t<E>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, E, make_rvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(static_cast<Promise&&>(promise).GetError());
				}
			}
			else
			{
				return fn_result_t<Fn, E, make_rvalue_t>{};
			}
		}

		template<cr_invocable<E> Fn>
		inline friend constexpr
			auto
			operator<<(const Promise&& promise, Fn&& action)
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_crvalue_t<E>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, E, make_crvalue_t>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(static_cast<const Promise&&>(promise).GetError());
				}
			}
			else
			{
				return fn_result_t<Fn, E, make_crvalue_t>{};
			}
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<lv_invocable<T> Fn>
		constexpr
			Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_lvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(GetResult());
				}
			}

			return *this;
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<cl_invocable<T> Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_clvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(GetResult());
				}
			}

			return *this;
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<rv_invocable<T> Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_rvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(move(*this).GetResult());
				}
			}

			return move(*this);
		}

		/// <summary>
		/// if_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<cr_invocable<T> Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_crvalue_t<T>>()))
		{
			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(move(*this).GetResult());
				}
			}

			return move(*this);
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		/// <summary>
		/// if_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsFailed())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<lv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_lvalue_t<T>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, T, make_lvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_lvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<cl_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_clvalue_t<T>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, T, make_clvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_clvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<rv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_rvalue_t<T>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, T, make_rvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(move(*this).GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_rvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, parameter T (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<cr_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<make_crvalue_t<T>>()))
		{
			static_assert(!same_as<fn_result_t<Fn, T, make_crvalue_t>, void>, "Promise result cannot be void.");

			if (IsSuccess())
			{
				if constexpr (same_as<T, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(move(*this).GetResult());
				}
			}
			else
			{
				return fn_result_t<Fn, T, make_crvalue_t>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			static_assert(!same_as<monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			static_assert(!same_as<monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			static_assert(!same_as<monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// and_then, no parameter (not failed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			static_assert(!same_as<monad_result_t<Fn>, void>, "Promise result cannot be void.");

			if (!IsFailed())
			{
				return forward<Fn>(action)();
			}
			else
			{
				return monad_result_t<Fn>{};
			}
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			Promise&
			else_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				forward<Fn>(action)();
			}

			return *this;
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			Promise&&
			else_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		/// <summary>
		/// else_then, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			const Promise&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				forward<Fn>(action)();
			}

			return move(*this);
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<lv_invocable<E> Fn>
		constexpr
			Promise&
			else_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_lvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(GetError());
				}
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<cl_invocable<E> Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_clvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(GetError());
				}
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<rv_invocable<E> Fn>
		constexpr
			Promise&&
			else_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_rvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(move(*this).GetError());
				}
			}

			return move(*this);
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<cr_invocable<E> Fn>
		constexpr
			const Promise&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_crvalue_t<E>>()))
		{
			if (IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					forward<Fn>(action)();
				}
				else
				{
					forward<Fn>(action)(move(*this).GetError());
				}
			}

			return move(*this);
		}

		/// <summary>
		/// or_else, no parameter (not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<invocables Fn>
		constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return forward<Fn>(action)();
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
		template<invocables Fn>
		constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return forward<Fn>(action)();
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
		template<invocables Fn>
		constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return forward<Fn>(action)();
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
		template<invocables Fn>
		constexpr
			monad_result_t<Fn>
			or_else(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)()))
		{
			using fwd_result_t = monad_result_t<Fn>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (!IsSuccess())
			{
				return forward<Fn>(action)();
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
		template<lv_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_lvalue_t<E>>()))
		{
			using fwd_result_t = fn_result_t<Fn, E, make_lvalue_t>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return forward<Fn>(action)(GetError());
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
		template<cl_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_clvalue_t<E>>()))
		{
			using fwd_result_t = fn_result_t<Fn, E, make_clvalue_t>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return forward<Fn>(action)(GetError());
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
		template<rv_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_rvalue_t<E>>()))
		{
			using fwd_result_t = fn_result_t<Fn, E, make_rvalue_t>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				return forward<Fn>(action)(move(*this).GetError());
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
		template<cr_invocable<E> Fn>
		constexpr
			auto
			or_else(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, E>::template Eval<make_crvalue_t<E>>()))
		{
			using fwd_result_t = fn_result_t<Fn, E, make_crvalue_t>;

			static_assert(!same_as<fwd_result_t, void>, "Promise result cannot be void.");

			if (IsFailed())
			{
				if constexpr (same_as<E, void>)
				{
					return forward<Fn>(action)();
				}
				else
				{
					return forward<Fn>(action)(move(*this).GetError());
				}
			}
			else
			{
				return fwd_result_t{};
			}
		}

		constexpr succeed_t& GetResult() & noexcept
			requires notvoids<T>
		{
			return mySucceed;
		}

		constexpr const succeed_t& GetResult() const& noexcept
			requires notvoids<T>
		{
			return mySucceed;
		}

		constexpr succeed_t&& GetResult() && noexcept
			requires notvoids<T>
		{
			return static_cast<succeed_t&&>(mySucceed);
		}

		constexpr const succeed_t&& GetResult() const&& noexcept
			requires notvoids<T>
		{
			return static_cast<const succeed_t&&>(mySucceed);
		}

		constexpr void GetResult() const noexcept
			requires (!notvoids<T>)
		{
			static_assert(always_false<T>, "T is void.");
		}

		constexpr failure_t& GetError() & noexcept
			requires notvoids<E>
		{
			return myFailure;
		}

		constexpr const failure_t& GetError() const& noexcept
			requires notvoids<E>
		{
			return myFailure;
		}

		constexpr failure_t&& GetError() && noexcept
			requires notvoids<E>
		{
			return static_cast<failure_t&&>(myFailure);
		}

		constexpr const failure_t&& GetError() const&& noexcept
			requires notvoids<E>
		{
			return static_cast<const failure_t&&>(myFailure);
		}

		constexpr void GetError() const noexcept
			requires (!notvoids<E>)
		{
			static_assert(always_false<E>, "E is void.");
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

	using Proxy = Promise<void, void>;

	using ioError = Promise<void, int>;

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
