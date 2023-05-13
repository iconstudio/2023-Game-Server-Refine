export module Net.Promise;
import Utility;
import Utility.Constraints;
import Utility.Monad.Loosen;
export import Net;
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

	template<typename T, typename E = void>
	class Promise;

	template<typename T, util::copyable E>
	class Promise<T, E> final
	{
	public:
		static_assert(!util::same_as<T, E>, "T and E must not to be same!");

		using succeed_t = io::success_t<T>;
		using failed_t = io::failure_t<E>;
		using defered_t = io::just_defered_t;
		using monad_t = util::LooseMonad<succeed_t, failed_t, defered_t>;

		constexpr Promise() noexcept
			: myState()
		{}

		constexpr Promise(util::nullopt_t) noexcept
			: myState(util::nullopt)
		{}

		template<typename U>
			requires (util::notvoids<T>&& util::same_as<util::clean_t<U>, T>)
		constexpr Promise(U&& pass)
			noexcept(util::nothrow_copy_constructibles<T>)
			: myState(util::in_place_type<succeed_t>, io::make_success(util::forward<U>(pass)))
		{}

		template<typename U>
			requires (util::same_as<util::clean_t<U>, E>)
		constexpr Promise(U&& fail)
			noexcept(util::nothrow_copy_constructibles<E>)
			: myState(util::in_place_type<failed_t>, io::make_failure(util::forward<U>(fail)))
		{}

		constexpr Promise(const Promise& other) noexcept
			: myState(other.myState)
		{}

		constexpr Promise(Promise&& other) noexcept
			: myState(static_cast<monad_t&&>(other.myState))
		{}

		explicit constexpr Promise(const monad_t& state) noexcept
			: myState(state)
		{}

		explicit constexpr Promise(monad_t&& state) noexcept
			: myState(static_cast<monad_t&&>(state))
		{}

		constexpr Promise(const succeed_t& success)
			noexcept(util::nothrow_copy_constructibles<succeed_t>) requires util::notvoids<T>
			: myState(util::in_place_type<succeed_t>, success)
		{}

		constexpr Promise(succeed_t&& success)
			noexcept(util::nothrow_move_constructibles<succeed_t>) requires util::notvoids<T>
			: myState(util::in_place_type<succeed_t>, static_cast<succeed_t&&>(success))
		{}

		constexpr Promise(succeed_t)
			noexcept requires (!util::notvoids<T>)
			: myState(util::in_place_type<succeed_t>, succeed_t{})
		{}

		constexpr Promise(const failed_t& error)
			noexcept(util::nothrow_copy_constructibles<failed_t>)
			: myState(util::in_place_type<failed_t>, error)
		{}

		constexpr Promise(failed_t&& error)
			noexcept(util::nothrow_move_constructibles<failed_t>)
			: myState(util::in_place_type<failed_t>, static_cast<failed_t&&>(error))
		{}

		constexpr Promise(defered_t) noexcept
			: myState(util::in_place_type<defered_t>, defered_t{})
		{}

		constexpr Promise& operator=(const Promise& other) noexcept
		{
			myState = other.myState;
			return *this;
		}

		constexpr Promise& operator=(Promise&& other) noexcept
		{
			myState = static_cast<monad_t&&>(other.myState);
			return *this;
		}

		constexpr ~Promise()
			noexcept(util::nothrow_destructibles<succeed_t, failed_t, defered_t>)
			requires(util::trivially_destructibles<succeed_t, failed_t, defered_t>)
		{}

		constexpr ~Promise()
			noexcept(util::nothrow_destructibles<succeed_t, failed_t, defered_t>)
			requires(!util::trivially_destructibles<succeed_t, failed_t, defered_t>)
		= default;

		/// <summary>
		/// operator>> (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::lv_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		/// <summary>
		/// operator>> (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::cl_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		/// <summary>
		/// operator>> (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::rv_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		/// <summary>
		/// operator>> (on succeed)
		/// </summary>
		/// <param name="promise"></param>
		/// <param name="action"></param>
		template<util::cr_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_crvalue_t<T>>{};
			}
		}

		/// <summary>
		/// operator>> (not failed)
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
		/// operator>> (not failed)
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
		/// operator>> (not failed)
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
		/// operator>> (not failed)
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

		template<util::invocables<E&> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, E&>
			operator<<(Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, E&>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)(promise.GetError());
			}
			else
			{
				return util::monad_result_t<Fn, E&>{};
			}
		}

		template<util::invocables<const E&> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, const E&>
			operator<<(const Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, const E&>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)(promise.GetError());
			}
			else
			{
				return util::monad_result_t<Fn, const E&>{};
			}
		}

		template<util::invocables<E&&> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, E&&>
			operator<<(Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&&>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, E&&>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)(static_cast<Promise&&>(promise).GetError());
			}
			else
			{
				return util::monad_result_t<Fn, E&&>{};
			}
		}

		template<util::invocables<const E&&> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, const E&&>
			operator<<(const Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&&>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, const E&&>, void>, "Error monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)(static_cast<const Promise&&>(promise).GetError());
			}
			else
			{
				return util::monad_result_t<Fn, const E&&>{};
			}
		}

		/// <summary>
		/// if_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::lv_invocable<T> Fn>
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
		/// if_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cl_invocable<T> Fn>
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
		/// if_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::rv_invocable<T> Fn>
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
		/// if_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cr_invocable<T> Fn>
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
		/// if_then (not failed)
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
		/// if_then (not failed)
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
		/// if_then (not failed)
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
		/// if_then (not failed)
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
		/// and_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::lv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_lvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_lvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		/// <summary>
		/// and_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cl_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_clvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_clvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		/// <summary>
		/// and_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::rv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_rvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_rvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		/// <summary>
		/// and_then (on succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::cr_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_crvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_crvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_crvalue_t<T>>{};
			}
		}

		/// <summary>
		/// else_then, no parameter (on not succeed)
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
		/// else_then, no parameter (on not succeed)
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
		/// else_then, no parameter (on not succeed)
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
		/// else_then, no parameter (on not succeed)
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
		template<util::invocables<E&> Fn>
		constexpr
			Promise&
			else_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&>())))
		{
			if (IsFailed())
			{
				util::forward<Fn>(action)(GetError());
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables<const E&> Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&>())))
		{
			if (IsFailed())
			{
				util::forward<Fn>(action)(GetError());
			}

			return *this;
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables<E&&> Fn>
		constexpr
			Promise&&
			else_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&&>())))
		{
			if (IsFailed())
			{
				util::forward<Fn>(action)(util::move(*this).GetError());
			}

			return util::move(*this);
		}

		/// <summary>
		/// else_then, parameter E (on failed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables<const E&&> Fn>
		constexpr
			const Promise&&
			else_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&&>())))
		{
			if (IsFailed())
			{
				util::forward<Fn>(action)(util::move(*this).GetError());
			}

			return util::move(*this);
		}

		/// <summary>
		/// or_else, no parameter (on not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		/// or_else, no parameter (on not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		/// or_else, no parameter (on not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		/// or_else, no parameter (on not succeed)
		/// </summary>
		/// <param name="action"></param>
		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		template<util::invocables<E&> Fn>
		constexpr
			util::monad_result_t<Fn, E&>
			or_else(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&>())))
		{
			using fwd_result_t = util::monad_result_t<Fn, E&>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		template<util::invocables<const E&> Fn>
		constexpr
			util::monad_result_t<Fn, const E&>
			or_else(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&>())))
		{
			using fwd_result_t = util::monad_result_t<Fn, const E&>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		template<util::invocables<E&&> Fn>
		constexpr
			util::monad_result_t<Fn, E&&>
			or_else(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<E&&>())))
		{
			using fwd_result_t = util::monad_result_t<Fn, E&&>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

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
		template<util::invocables<const E&&> Fn>
		constexpr
			util::monad_result_t<Fn, const E&&>
			or_else(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const E&&>())))
		{
			using fwd_result_t = util::monad_result_t<Fn, const E&&>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

			if (IsFailed())
			{
				return util::forward<Fn>(action)(util::move(*this).GetError());
			}
			else
			{
				return fwd_result_t{};
			}
		}

		constexpr succeed_t& GetResult() & noexcept
			requires util::notvoids<T>
		{
			return myState.get<succeed_t>();
		}

		constexpr const succeed_t& GetResult() const& noexcept
			requires util::notvoids<T>
		{
			return myState.get<succeed_t>();
		}

		constexpr succeed_t&& GetResult() && noexcept
			requires util::notvoids<T>
		{
			return util::move(myState).get<succeed_t>();
		}

		constexpr const succeed_t&& GetResult() const&& noexcept
			requires util::notvoids<T>
		{
			return util::move(myState).get<succeed_t>();
		}

		constexpr failed_t& GetError() & noexcept
		{
			return myState.get<failed_t>();
		}

		constexpr const failed_t& GetError() const& noexcept
		{
			return myState.get<failed_t>();
		}

		constexpr failed_t&& GetError() && noexcept
		{
			return static_cast<monad_t&&>(myState).get<failed_t>();
		}

		constexpr const failed_t&& GetError() const&& noexcept
		{
			return static_cast<const monad_t&&>(myState).get<failed_t>();
		}

		constexpr bool IsSuccess() const noexcept
		{
			return myState.has_value<succeed_t>();
		}

		constexpr bool IsFailed() const noexcept
		{
			return myState.has_value<failed_t>();
		}

		constexpr bool IsDefered() const noexcept
		{
			return myState.has_value<defered_t>();
		}

	private:
		consteval void GetResult() const noexcept
			requires (!util::notvoids<T>)
		{
			return;
		}

		monad_t myState;
	};

	template<typename T>
	class Promise<T, void> final
	{
	public:
		using succeed_t = io::success_t<T>;
		using failed_t = io::just_failure_t;
		using defered_t = io::just_defered_t;
		using monad_t = util::LooseMonad<succeed_t, failed_t, defered_t>;

		constexpr Promise() noexcept
			: myState()
		{}

		constexpr Promise(util::nullopt_t) noexcept
			: myState(util::nullopt)
		{}

		template<typename U>
			requires util::notvoids<T>
		constexpr Promise(U&& successor)
			noexcept(util::nothrow_copy_constructibles<T>)
			: myState(util::in_place_type<succeed_t>, io::make_success(util::forward<U>(successor)))
		{}

		constexpr Promise(const Promise& other)
			noexcept requires (util::copyable<T>)
			: myState(other.myState)
		{}

		constexpr Promise(const Promise& other)
			noexcept requires (!util::copyable<T>)
			: myState(static_cast<monad_t&&>(other.myState))
		{}

		constexpr Promise(Promise&& other) noexcept
			: myState(static_cast<monad_t&&>(other.myState))
		{}

		explicit constexpr Promise(const monad_t& state) noexcept
			: myState(state)
		{}

		explicit constexpr Promise(monad_t&& state) noexcept
			: myState(static_cast<monad_t&&>(state))
		{}

		constexpr Promise(const succeed_t& success)
			noexcept(util::nothrow_copy_constructibles<succeed_t>) requires util::notvoids<T>
			: myState(util::in_place_type<succeed_t>, success)
		{}

		constexpr Promise(succeed_t&& success)
			noexcept(util::nothrow_move_constructibles<succeed_t>) requires util::notvoids<T>
			: myState(util::in_place_type<succeed_t>, util::move(success))
		{}

		constexpr Promise(succeed_t)
			noexcept requires (!util::notvoids<T>)
			: myState(util::in_place_type<succeed_t>, succeed_t{})
		{}

		constexpr Promise(failed_t) noexcept
			: myState(util::in_place_type<failed_t>, failed_t{})
		{}

		constexpr Promise(defered_t) noexcept
			: myState(util::in_place_type<defered_t>, defered_t{})
		{}

		constexpr Promise& operator=(const Promise& other) noexcept
		{
			myState = other.myState;
			return *this;
		}

		constexpr Promise& operator=(Promise&& other) noexcept
		{
			myState = static_cast<monad_t&&>(other.myState);
			return *this;
		}

		constexpr ~Promise()
			noexcept(util::nothrow_destructibles<succeed_t, failed_t, defered_t>)
			requires(util::trivially_destructibles<succeed_t, failed_t, defered_t>)
		= default;

		constexpr ~Promise()
			noexcept(util::nothrow_destructibles<succeed_t, failed_t, defered_t>)
			requires(!util::trivially_destructibles<succeed_t, failed_t, defered_t>)
		{}

		template<util::lv_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		template<util::cl_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		template<util::rv_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		template<util::cr_invocable<T> Fn>
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
				return util::monad_result_t<Fn, util::make_crvalue_t<T>>{};
			}
		}

		template<util::invocables Fn>
		inline friend constexpr
			util::monad_result_t<Fn>
			operator<<(const Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		template<util::invocables Fn>
		inline friend constexpr
			util::monad_result_t<Fn>
			operator<<(Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Monadic result cannot be void.");

			if (promise.IsFailed())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{};
			}
		}

		template<util::lv_invocable<T> Fn>
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

		template<util::cl_invocable<T> Fn>
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

		template<util::rv_invocable<T> Fn>
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

		template<util::cr_invocable<T> Fn>
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

		template<util::lv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_lvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_lvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		template<util::cl_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_clvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_clvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		template<util::rv_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) &&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_rvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_rvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		template<util::cr_invocable<T> Fn>
		constexpr
			auto
			and_then(Fn&& action) const&&
			noexcept(noexcept(noexcept_t<Fn, T>::template Eval<util::make_crvalue_t<T>>()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_crvalue_t<T>>, void>, "Monadic result cannot be void.");

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
				return util::monad_result_t<Fn, util::make_crvalue_t<T>>{};
			}
		}

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

		template<util::invocables Fn>
		constexpr
			const Promise&
			else_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (!IsSuccess())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		template<util::invocables Fn>
		constexpr
			util::monad_result_t<Fn>
			or_else(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			using fwd_result_t = util::monad_result_t<Fn>;

			static_assert(!util::same_as<fwd_result_t, void>, "Monadic result cannot be void.");

			if (!IsSuccess())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{};
			}
		}

		constexpr auto GetResult() & noexcept
			requires util::notvoids<T>
		{
			return myState.get<succeed_t>();
		}

		constexpr auto GetResult() const& noexcept
			requires util::notvoids<T>
		{
			return myState.get<succeed_t>();
		}

		constexpr auto GetResult() && noexcept
			requires util::notvoids<T>
		{
			return util::move(myState).get<succeed_t>();
		}

		constexpr auto GetResult() const&& noexcept
			requires util::notvoids<T>
		{
			return util::move(myState).get<succeed_t>();
		}

		constexpr bool IsSuccess() const noexcept
		{
			return myState.has_value<succeed_t>();
		}

		constexpr bool IsFailed() const noexcept
		{
			return myState.has_value<failed_t>();
		}

		constexpr bool IsDefered() const noexcept
		{
			return myState.has_value<defered_t>();
		}

	private:
		monad_t myState;
	};

	using Proxy = Promise<void, void>;
	template<typename T>
	using ErrorHandler = Promise<T, int>;
	using PointerHandler = ErrorHandler<void*>;

	template<typename T>
	Promise(T) -> Promise<T, void>;

	template<typename T, util::copyable E>
	Promise(T, E) -> Promise<T, E>;
}

#pragma warning(push, 1)
namespace net::test
{
	static void test_promise() noexcept
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
}
#pragma warning(pop)
