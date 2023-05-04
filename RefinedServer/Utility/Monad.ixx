export module Utility.Monad;
import Utility;

export namespace util
{
	template<typename Rx, typename Fn, typename... Args>
	concept invocable_results = invocables<Fn, Args...>&& convertible_to<invoke_result_t<Fn, Args...>, Rx>;

	template<typename Fn, typename... Args>
	using monad_result_t = clean_t<invoke_result_t<Fn, Args...>>;

	template<typename T>
	class [[nodiscard]] Monad;

	template<copyable T>
	class [[nodiscard]] Monad<T>
	{
	public:
		using value_type = T;

		constexpr Monad(nullopt_t) noexcept(nothrow_constructibles<T>)
			: myValue()
		{}

		template<convertible_to<T> Uty>
		constexpr Monad(Uty&& uty) noexcept(nothrow_constructibles<T, Uty&&>)
			: myValue(util::forward<Uty>(uty))
			, hasValue(true)
		{}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&, Args...>
		inline constexpr Monad&
			if_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>(), declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(myValue, forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&, Args...>
		inline constexpr const Monad&
			if_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>(), declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(myValue, forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&&, Args...>
		inline constexpr Monad&&
			if_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>(), declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&&, Args...>
		inline constexpr const Monad&&
			if_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>(), declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, T&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (hasValue)
			{
				return forward<Fn>(action)(myValue, forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, const T&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (hasValue)
			{
				return forward<Fn>(action)(myValue, forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&&, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, T&&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (hasValue)
			{
				return forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&&, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, const T&&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (hasValue)
			{
				return forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<invocables<T&> Lfn, invocables<T&> Rfn>
		inline constexpr
			monad_result_t<Lfn, T&>
			transform(Lfn&& safe_action, Rfn&& fail_action) &
			noexcept(noexcept(forward<Lfn>(safe_action)(declval<T&>())) && noexcept(forward<Rfn>(fail_action)(declval<T&>())))
		{
			using safe_result_t = monad_result_t<Lfn, T&>;
			using fail_result_t = monad_result_t<Rfn, T&>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(myValue);
				}
				else
				{
					return forward<Lfn>(safe_action)(myValue);
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(myValue);
				}
				else
				{
					return forward<Rfn>(fail_action)(myValue);
				}
			}
		}

		template<invocables<const T&> Lfn, invocables<const T&> Rfn>
		inline constexpr
			monad_result_t<Lfn, const T&&>
			transform(Lfn&& safe_action, Rfn&& fail_action) const&
			noexcept(noexcept(forward<Lfn>(safe_action)(declval<const T&>())) && noexcept(forward<Rfn>(fail_action)(declval<const T&>())))
		{
			using safe_result_t = monad_result_t<Lfn, const T&>;
			using fail_result_t = monad_result_t<Rfn, const T&>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(myValue);
				}
				else
				{
					return forward<Lfn>(safe_action)(myValue);
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(myValue);
				}
				else
				{
					return forward<Rfn>(fail_action)(myValue);
				}
			}
		}

		template<invocables<T&&> Lfn, invocables<T&&> Rfn>
		inline constexpr
			monad_result_t<Lfn, T&>
			transform(Lfn&& safe_action, Rfn&& fail_action) &&
			noexcept(noexcept(forward<Lfn>(safe_action)(declval<T&&>())) && noexcept(forward<Rfn>(fail_action)(declval<T&&>())))
		{
			using safe_result_t = monad_result_t<Lfn, T&&>;
			using fail_result_t = monad_result_t<Rfn, T&&>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(move(myValue));
				}
				else
				{
					return forward<Lfn>(safe_action)(move(myValue));
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(move(myValue));
				}
				else
				{
					return forward<Rfn>(fail_action)(move(myValue));
				}
			}
		}

		template<invocables<const T&&> Lfn, invocables<const T&&> Rfn>
		inline constexpr
			monad_result_t<Lfn, const T&&>
			transform(Lfn&& safe_action, Rfn&& fail_action) const&&
			noexcept(noexcept(forward<Lfn>(safe_action)(declval<const T&&>())) && noexcept(forward<Rfn>(fail_action)(declval<const T&&>())))
		{
			using safe_result_t = monad_result_t<Lfn, const T&&>;
			using fail_result_t = monad_result_t<Rfn, const T&&>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(move(myValue));
				}
				else
				{
					return forward<Lfn>(safe_action)(move(myValue));
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(move(myValue));
				}
				else
				{
					return forward<Rfn>(fail_action)(move(myValue));
				}
			}
		}

		template<invocables<T&> Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value) &
			noexcept(noexcept(forward<Fn>(safe_action)(declval<T&>())))
		{
			using fn_result_t = monad_result_t<Fn, T&>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)(myValue);
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<invocables<const T&> Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value) const&
			noexcept(noexcept(forward<Fn>(safe_action)(declval<const T&>())))
		{
			using fn_result_t = monad_result_t<Fn, const T&>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)(myValue);
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<invocables<T&&> Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value) &&
			noexcept(noexcept(forward<Fn>(safe_action)(declval<T&&>())))
		{
			using fn_result_t = monad_result_t<Fn, T&&>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)(move(myValue));
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<invocables<const T&&> Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value) const&&
			noexcept(noexcept(forward<Fn>(safe_action)(declval<const T&&>())))
		{
			using fn_result_t = monad_result_t<Fn, const T&&>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)(move(myValue));
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&, Args...>
		inline constexpr
			Monad&
			else_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>(), declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(myValue, forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&, Args...>
		inline constexpr
			const Monad&
			else_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>(), declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(myValue, forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&&, Args...>
		inline constexpr
			Monad&&
			else_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>(), declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&&, Args...>
		inline constexpr
			const Monad&&
			else_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>(), declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&, Args...>
		inline constexpr
			auto
			or_else(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, T&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (!hasValue)
			{
				return forward<Fn>(action)(myValue, forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&, Args...>
		inline constexpr
			auto
			or_else(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, const T&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (!hasValue)
			{
				return forward<Fn>(action)(myValue, forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, T&&, Args...>
		inline constexpr
			auto
			or_else(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, T&&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (!hasValue)
			{
				return forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, const T&&, Args...>
		inline constexpr
			auto
			or_else(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>(), declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, const T&&, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (!hasValue)
			{
				return forward<Fn>(action)(move(myValue), forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		constexpr T& value() & noexcept
		{
			return myValue;
		}

		constexpr const T& value() const& noexcept
		{
			return myValue;
		}

		constexpr T&& value() && noexcept(nothrow_move_constructibles<T>)
		{
			return move(myValue);
		}

		constexpr const T&& value() const&& noexcept(nothrow_move_constructibles<const T>)
		{
			return move(myValue);
		}

		constexpr T value_or(const T& failsafe) const& noexcept(nothrow_constructibles<T>)
		{
			if (hasValue)
			{
				return myValue;
			}
			else
			{
				return failsafe;
			}
		}

		constexpr T value_or(T&& failsafe) const& noexcept(nothrow_move_constructibles<T>)
		{
			if (hasValue)
			{
				return myValue;
			}
			else
			{
				return failsafe;
			}
		}

		constexpr T value_or(const T& failsafe) && noexcept(nothrow_constructibles<T>)
		{
			if (hasValue)
			{
				return move(myValue);
			}
			else
			{
				return (failsafe);
			}
		}

		constexpr T&& value_or(T&& failsafe) && noexcept(nothrow_move_constructibles<T>)
		{
			if (hasValue)
			{
				return move(myValue);
			}
			else
			{
				return move(failsafe);
			}
		}

		constexpr T get() const& noexcept(nothrow_copy_constructibles<T>)
		{
			return myValue;
		}

		constexpr T&& get() && noexcept(nothrow_move_constructibles<T>)
		{
			return move(myValue);
		}

		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		explicit constexpr operator T () const noexcept(nothrow_copy_constructibles<T>)
		{
			return myValue;
		}

		explicit constexpr operator bool() const noexcept
		{
			return hasValue;
		}

		constexpr Monad() noexcept(nothrow_default_constructibles<T>) = default;
		constexpr Monad(const Monad&) noexcept(nothrow_copy_constructibles<T>) = default;
		constexpr Monad(Monad&&) noexcept(nothrow_move_constructibles<T>) = default;
		constexpr Monad& operator=(const Monad&) noexcept(nothrow_copy_assignables<T>) = default;
		constexpr Monad& operator=(Monad&&) noexcept(nothrow_move_assignables<T>) = default;
		constexpr ~Monad() noexcept(nothrow_destructibles<T>) = default;

	private:
		T myValue = {};
		bool hasValue = false;
	};

	template<>
	class [[nodiscard]] Monad<void>
	{
	public:
		constexpr Monad(nullopt_t) noexcept {}

		template<copyable Uty>
		constexpr Monad(Monad<Uty>& other) noexcept
			: hasValue(other.hasValue)
		{}

		template<copyable Uty>
		constexpr Monad(const Monad<Uty>& other) noexcept
			: hasValue(other.hasValue)
		{}

		template<copyable Uty>
		constexpr Monad(Monad<Uty>&& other) noexcept
			: hasValue(move(other.hasValue))
		{}

		template<copyable Uty>
		constexpr Monad(const Monad<Uty>&& other) noexcept
			: hasValue(move(other.hasValue))
		{}

		template<copyable Uty>
		constexpr Monad& operator=(Monad<Uty>& other) noexcept
		{
			hasValue = other.hasValue;
			return *this;
		}

		template<copyable Uty>
		constexpr Monad& operator=(const Monad<Uty>& other) noexcept
		{
			hasValue = other.hasValue;
			return *this;
		}

		template<copyable Uty>
		constexpr Monad& operator=(Monad<Uty>&& other) noexcept
		{
			hasValue = move(other.hasValue);
			return *this;
		}

		template<copyable Uty>
		constexpr Monad& operator=(const Monad<Uty>&& other) noexcept
		{
			hasValue = move(other.hasValue);
			return *this;
		}

		constexpr Monad& operator=(Monad&&) noexcept = default;

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr Monad&
			if_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr const Monad&
			if_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr Monad&&
			if_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr const Monad&&
			if_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			monad_result_t<Fn, Args...>
			and_then(Fn&& action, Args&&... args)
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			if (hasValue)
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			monad_result_t<Fn, Args...>
			and_then(Fn&& action, Args&&... args) const
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			if (hasValue)
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<invocables Lfn, invocables Rfn>
		inline constexpr
			monad_result_t<Lfn>
			transform(Lfn&& safe_action, Rfn&& fail_action)
			noexcept(noexcept(forward<Lfn>(safe_action)()) && noexcept(forward<Rfn>(fail_action)()))
		{
			using safe_result_t = monad_result_t<Lfn>;
			using fail_result_t = monad_result_t<Rfn>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)();
				}
				else
				{
					return forward<Lfn>(safe_action)();
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)();
				}
				else
				{
					return forward<Rfn>(fail_action)();
				}
			}
		}

		template<invocables Lfn, invocables Rfn>
		inline constexpr
			monad_result_t<Lfn>
			transform(Lfn&& safe_action, Rfn&& fail_action) const
			noexcept(noexcept(forward<Lfn>(safe_action)()) && noexcept(forward<Rfn>(fail_action)()))
		{
			using safe_result_t = monad_result_t<Lfn>;
			using fail_result_t = monad_result_t<Rfn>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (hasValue)
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)();
				}
				else
				{
					return forward<Lfn>(safe_action)();
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)();
				}
				else
				{
					return forward<Rfn>(fail_action)();
				}
			}
		}

		template<invocables Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value)
			noexcept(noexcept(forward<Fn>(safe_action)()))
		{
			using fn_result_t = monad_result_t<Fn>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)();
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<invocables Fn, typename Uty>
		inline constexpr
			Uty
			translate(Fn&& safe_action, Uty&& default_value) const
			noexcept(noexcept(forward<Fn>(safe_action)()))
		{
			using fn_result_t = monad_result_t<Fn>;

			static_assert(convertible_to<fn_result_t, Uty>);

			if (hasValue)
			{
				return forward<Fn>(safe_action)();
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr Monad&
			else_than(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr const Monad&
			else_than(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr Monad&&
			else_than(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr const Monad&&
			else_than(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!hasValue)
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			monad_result_t<Fn, Args...>
			or_else(Fn&& action, Args&&... args)
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			if (!hasValue)
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			monad_result_t<Fn, Args...>
			or_else(Fn&& action, Args&&... args) const
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			if (!hasValue)
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return fwd_result_t{ nullopt };
			}
		}

		constexpr Monad() noexcept = default;
		constexpr Monad(const Monad&) noexcept = default;
		constexpr Monad(Monad&&) noexcept = default;
		constexpr ~Monad() noexcept = default;

		bool hasValue = false;
	};

	template<typename T>
	Monad(T) -> Monad<T>;
}

namespace util
{
	inline void do_something() noexcept
	{}

	static void test_monad() noexcept
	{
		Monad<int> monad1{};
		const Monad<int> monad2{};
		constexpr Monad<int> monad3{};

		Monad<int> monad4{ 1000 };
		const bool has4 = monad4.has_value();
		const Monad<int> monad5{ 1000 };
		const bool has5 = monad5.has_value();
		constexpr Monad<int> monad6{ 1000 };
		constexpr bool has6 = monad6.has_value();

		Monad<int> monad7{ 80264.01954f };
		const Monad<int> monad8{ false };
		constexpr Monad<int> monad9{ 1000ULL };

		const auto valor7 = monad7.value_or(42);

		const auto expr8_3 = monad8.transform(
			[](const int& v) -> float { return static_cast<unsigned>(v); },
			[](const int& v) -> float { return v * 100.0f; }
		);

		const auto expr8_4 = monad8.translate([](auto&&) -> long long { return 15718742LL; }, -7);

		const auto& expr8_5 = monad8.else_then([](auto&&) { do_something(); });

		constexpr int valor3 = monad3.value_or(3000.0);

		constexpr const auto& expr9_1 = monad9.if_then([](auto&&) -> float { return 3000.0; });

		constexpr util::Monad<float> expr9_2 = monad9.and_then(
			[](auto&&) -> util::Monad<float> {
			return { 3000.0 };
		});

		constexpr double expr9_3 = monad9.transform(
			[](const int& value) -> double { return value + 3000.0; },
			[](auto&&) -> double { return 0.0; }
		);

		constexpr int expr9_4 = monad9.translate(
			[](auto&&) -> long long { return 15718742LL; },
			-7
		);

		constexpr const util::Monad<int>& expr9_5 = monad9.else_then(
			[](auto&&) {}
		);

		constexpr util::Monad<float> expr9_6 = monad9.or_else(
			[](const int& v) -> util::Monad<float> {
			return 5020.0f;
		});
	}
}
