export module Net.Promise;
import Utility;
import Utility.Constraints;
import Utility.Monad.Loosen;
import Net;

export namespace net
{
	namespace io
	{
		template<typename T = void>
		struct success_t;

		template<>
		struct success_t<void>
		{
			explicit constexpr success_t() noexcept = default;
		};

		template<util::copyable T>
		struct success_t<T>
		{
			explicit constexpr success_t(const T& value) noexcept
				: myValue(value)
			{}

			explicit constexpr success_t(T&& value) noexcept
				: myValue(static_cast<T&&>(value))
			{}

			constexpr const T& operator*() const& noexcept
			{
				return myValue;
			}

			constexpr T&& operator*() && noexcept
			{
				return myValue;
			}

			constexpr operator T& () & noexcept
			{
				return myValue;
			}

			constexpr operator const T& () const& noexcept
			{
				return myValue;
			}

			constexpr operator T && () && noexcept
			{
				return static_cast<T&&>(myValue);
			}

			constexpr operator const T && () const&& noexcept
			{
				return static_cast<const T&&>(myValue);
			}

			constexpr ~success_t() noexcept(util::nothrow_destructibles<T>) = default;

			constexpr T& operator()(T& value) const noexcept
			{
				return value;
			}

			constexpr const T& operator()(const T& value) const noexcept
			{
				return value;
			}

			constexpr T&& operator()(T&& value) const noexcept
			{
				return static_cast<T&&>(value);
			}

			constexpr const T&& operator()(const T&& value) const noexcept
			{
				return static_cast<const T&&>(value);
			}

			T myValue;
		};

		template<typename T = void>
		struct error_t final
		{
			constexpr error_t(const T& error_code) noexcept
				: errorCode(error_code)
			{}

			constexpr error_t(T&& error_code) noexcept
				: errorCode(static_cast<T&&>(error_code))
			{}

			T errorCode;
		};

		template<>
		struct error_t<void> final
		{
			explicit constexpr error_t() noexcept = default;
		};

		struct defer_t { explicit constexpr defer_t() noexcept = default; };

		inline constexpr error_t<int> error(const int& error_code) noexcept
		{
			return error_t{ error_code };
		}
		inline constexpr error_t<int> error(int&& error_code) noexcept
		{
			return error_t{ static_cast<int&&>(error_code) };
		}

		inline constexpr success_t<void> success{ };
		inline constexpr error_t<void> fail{ };
		inline constexpr defer_t defer{ };

		template<typename T>
		inline constexpr success_t<T> succeed_io{ };
		inline constexpr defer_t      defered_io{ };
		template<typename T>
		inline constexpr error_t<T>   failed_io{ };
	}

	template<typename T, typename E>
	class Promise;

	template<util::copyable T, util::copyable E>
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

		template<util::invocables<T&> Fn>
		inline constexpr
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

		template<util::invocables<const T&> Fn>
		inline constexpr
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

		template<util::invocables<T&&> Fn>
		inline constexpr
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

		template<util::invocables<const T&&> Fn>
		inline constexpr
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

	private:
		monad_t myState;
	};

	template<>
	class Promise<void, void> final
	{
	public:
		using succeed_t = io::success_t<void>;
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

		constexpr Promise(succeed_t) noexcept
			: myState(util::in_place_type<succeed_t>, io::success)
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

		template<util::invocables Fn>
		inline constexpr
			const Promise&
			if_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)();
			}

			return *this;
		}

		template<util::invocables Fn>
		inline constexpr
			Promise&&
			if_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)();
			}

			return util::move(*this);
		}

		template<util::invocables Fn>
		inline constexpr
			util::monad_result_t<Fn>
			and_then(Fn&& action) const&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Monadic result cannot be void.");

			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{ util::nullopt };
			}
		}

		template<util::invocables Fn>
		inline constexpr
			util::monad_result_t<Fn>
			and_then(Fn&& action) &&
			noexcept(noexcept(util::forward<Fn>(action)()))
		{
			static_assert(!util::same_as<util::monad_result_t<Fn>, void>, "Monadic result cannot be void.");

			if (myState.template has_value<succeed_t>())
			{
				util::forward<Fn>(action)();
			}
			else
			{
				return util::monad_result_t<Fn>{ util::nullopt };
			}
		}

		template<util::invocables Fn>
		inline constexpr
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
		inline constexpr
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
		inline constexpr
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
		inline constexpr
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

	private:
		monad_t myState;
	};

	using Proxy = Promise<void, void>;
	template<util::copyable T>
	using Handler = Promise<T, int>;
	using PointerHandler = Promise<void*, int>;
}
