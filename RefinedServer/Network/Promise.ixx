export module Net.Promise;
import Utility;
import Utility.Constraints;
import Utility.Identity;
import Utility.Monad.Loosen;
import Net;

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

		template<typename T>
		using success_t = util::Identity<T, tags::io_success>;

		template<typename T>
		using error_t = util::Identity<T, tags::io_failure>;

		using defer_t = util::Identity<void, tags::io_defered>;

		constexpr error_t<int> error(const int& error_code) noexcept
		{
			return error_t<int>{ error_code };
		}
		constexpr error_t<int> error(int&& error_code) noexcept
		{
			return error_t<int>{ static_cast<int&&>(error_code) };
		}

		constexpr success_t<void> success{ };
		constexpr error_t<void> fail{ };
		constexpr defer_t defer{ };

		template<typename T>
		constexpr success_t<T> succeed_io{ };
		constexpr defer_t      defered_io{ };
		template<typename T>
		constexpr error_t<T>   failed_io{ };
	}

	template<typename T, typename E>
	class Promise;

	template<typename T, util::copyable E>
	class Promise<T, E> final
	{
	public:
		using succeed_t = io::success_t<T>;
		using failed_t = io::error_t<E>;
		using defered_t = io::defer_t;
		using monad_t = util::LooseMonad<succeed_t, failed_t, defered_t>;

		constexpr Promise() noexcept
			: myState()
		{}

		constexpr Promise(util::nullopt_t) noexcept
			: myState(util::nullopt)
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

		constexpr Promise(const succeed_t& success) noexcept
			: myState(util::in_place_type<succeed_t>, success)
		{}

		constexpr Promise(succeed_t&& success) noexcept
			: myState(util::in_place_type<succeed_t>, static_cast<succeed_t&&>(success))
		{}

		constexpr Promise(const failed_t& error) noexcept
			: myState(util::in_place_type<failed_t>, error)
		{}

		constexpr Promise(failed_t&& error) noexcept
			: myState(util::in_place_type<failed_t>, static_cast<failed_t&&>(error))
		{}

		constexpr Promise(defered_t) noexcept
			: myState(util::in_place_type<defered_t>, io::defer)
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

		template<util::lv_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_lvalue_t<T>>
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_lvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(promise.GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		template<util::cl_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_clvalue_t<T>>
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_clvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(promise.GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		template<util::rv_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_rvalue_t<T>>
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_rvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(util::move(promise).GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		template<util::cr_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_crvalue_t<T>>
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_crvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(util::move(promise).GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_crvalue_t<T>>{};
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

		template<util::lv_invocable<T> Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<T&>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(myState.template get_value<succeed_t>());
			}

			return *this;
		}

		template<util::cl_invocable<T> Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const T&>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(myState.template get_value<succeed_t>());
			}

			return *this;
		}

		template<util::rv_invocable<T> Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<T&&>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(util::move(myState).template get_value<succeed_t>());
			}

			return util::move(*this);
		}

		template<util::cr_invocable<T> Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<const T&&>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(util::move(myState).template get_value<succeed_t>());
			}

			return util::move(*this);
		}

		consteval void GetResult() const noexcept
			requires (!util::notvoids<T>)
		{
			return;
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

		constexpr bool IsDeferec() const noexcept
		{
			return myState.has_value<defered_t>();
		}

	private:
		monad_t myState;
	};

	template<typename T>
	class Promise<T, void> final
	{
	public:
		using succeed_t = io::success_t<T>;
		using failed_t = io::error_t<void>;
		using defered_t = io::defer_t;
		using monad_t = util::LooseMonad<succeed_t, failed_t, defered_t>;

		constexpr Promise() noexcept
			: myState()
		{}

		constexpr Promise(util::nullopt_t) noexcept
			: myState(util::nullopt)
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

		constexpr Promise(const succeed_t& success) noexcept
			: myState(util::in_place_type<succeed_t>, success)
		{}

		constexpr Promise(succeed_t&& success) noexcept
			: myState(util::in_place_type<succeed_t>, util::move(success))
		{}

		constexpr Promise(failed_t) noexcept
			: myState(util::in_place_type<failed_t>, io::fail)
		{}

		constexpr Promise(defered_t) noexcept
			: myState(util::in_place_type<defered_t>, io::defer)
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

		template<util::lv_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_lvalue_t<T>>
			operator>>(Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_lvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(promise.GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		template<util::cl_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_clvalue_t<T>>
			operator>>(const Promise& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_clvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(promise.GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		template<util::rv_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_rvalue_t<T>>
			operator>>(Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_rvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(util::move(promise).GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		template<util::cr_invocable<T> Fn>
		inline friend constexpr
			util::monad_result_t<Fn, util::make_crvalue_t<T>>
			operator>>(const Promise&& promise, Fn&& action)
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_crvalue_t<T>>())))
		{
			if (promise.IsSuccess())
			{
				return util::forward<Fn>(action)(util::move(promise).GetResult());
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
			const Promise&
			if_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_lvalue_t<T>>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(GetResult());
			}

			return *this;
		}

		template<util::cl_invocable<T> Fn>
		constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_clvalue_t<T>>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(GetResult());
			}

			return *this;
		}

		template<util::rv_invocable<T> Fn>
		constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_rvalue_t<T>>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(GetResult());
			}

			return util::move(*this);
		}

		template<util::cr_invocable<T> Fn>
		constexpr
			const Promise&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_crvalue_t<T>>())))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)(GetResult());
			}

			return util::move(*this);
		}

		template<util::lv_invocable<T> Fn>
		constexpr
			util::monad_result_t<Fn, util::make_lvalue_t<T>>
			and_then(Fn&& action) &
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_lvalue_t<T>>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_lvalue_t<T>>, void>, "Monadic result cannot be void.");

			if (myState.has_value<succeed_t>())
			{
				return util::forward<Fn>(action)(GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_lvalue_t<T>>{};
			}
		}

		template<util::cl_invocable<T> Fn>
		constexpr
			util::monad_result_t<Fn, util::make_clvalue_t<T>>
			and_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_clvalue_t<T>>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_clvalue_t<T>>, void>, "Monadic result cannot be void.");

			if (myState.has_value<succeed_t>())
			{
				return util::forward<Fn>(action)(GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_clvalue_t<T>>{};
			}
		}

		template<util::rv_invocable<T> Fn>
		constexpr
			util::monad_result_t<Fn, util::make_rvalue_t<T>>
			and_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_rvalue_t<T>>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_rvalue_t<T>>, void>, "Monadic result cannot be void.");

			if (myState.has_value<succeed_t>())
			{
				return util::forward<Fn>(action)(GetResult());
			}
			else
			{
				return util::monad_result_t<Fn, util::make_rvalue_t<T>>{};
			}
		}

		template<util::cr_invocable<T> Fn>
		constexpr
			util::monad_result_t<Fn, util::make_crvalue_t<T>>
			and_then(Fn&& action) const&&
			noexcept(noexcept(util::forward<Fn>(action)(util::declval<util::make_crvalue_t<T>>())))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn, util::make_crvalue_t<T>>, void>, "Monadic result cannot be void.");

			if (myState.has_value<succeed_t>())
			{
				return util::forward<Fn>(action)(GetResult());
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
			if (myState.template has_value<failed_t>())
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
			if (myState.template has_value<failed_t>())
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

			if (!myState.template has_value<succeed_t>())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ util::nullopt };
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

			if (!myState.template has_value<succeed_t>())
			{
				return util::forward<Fn>(action)();
			}
			else
			{
				return fwd_result_t{ util::nullopt };
			}
		}

		consteval void GetResult() const noexcept
			requires (!util::notvoids<T>)
		{
			return;
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

		constexpr bool IsDeferec() const noexcept
		{
			return myState.has_value<defered_t>();
		}

	private:
		monad_t myState;
	};

	using Proxy = Promise<void, void>;
	template<util::copyable T>
	using ErrorHandler = Promise<T, int>;
	using PointerHandler = ErrorHandler<void*>;
}

namespace net
{
	constexpr void test_promise() noexcept
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
		const auto& cr0_0 = cvpromise0.if_then(
			[](const int& v) -> int {
			return 300;
		});
		const auto cr0_1 = cvpromise0.and_then(
			[](const int& v) -> Promise<int, void> {
			return Promise<int, void>::succeed_t{ 300 };
		});

		constexpr Promise<long long, void> cvpromise1{};
	}
}
