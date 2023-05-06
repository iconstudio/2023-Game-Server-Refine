export module Utility.Monad;
export import Utility;
import Utility.Traits;
import Utility.Constraints;
import Utility.Identity;

export namespace util
{
	template<typename Fn, typename... Args>
	using monad_result_t = clean_t<invoke_result_t<Fn, Args...>>;

	template<typename T>
	class [[nodiscard]] Monad : public Identity<T>
	{
	public:
		static_assert(!same_as<T, nullopt_t>, "T must not be nullopt_t.");

		using base_type = Identity<T>;
		using value_type = T;

		constexpr Monad()
			noexcept(nothrow_constructibles<T>)
			: base_type()
		{}

		constexpr Monad(nullopt_t)
			noexcept(nothrow_constructibles<T>)
			: base_type()
		{}

		constexpr Monad(const T& fwd)
			noexcept(nothrow_copy_constructibles<T>)
			: base_type(fwd)
			, hasValue(true)
		{}

		constexpr Monad(T&& fwd)
			noexcept(nothrow_move_constructibles<T>)
			: base_type(static_cast<T&&>(fwd))
			, hasValue(true)
		{}

		template<invocables<T&> Fn>
		inline constexpr
			Monad&
			if_then(Fn&& action) &
			noexcept(noexcept(forward<Fn>(action)(declval<T&>())))
		{
			if (has_value())
			{
				forward<Fn>(action)(this->value());
			}

			return *this;
		}

		template<invocables<const T&> Fn>
		inline constexpr
			const Monad&
			if_then(Fn&& action) const&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&>())))
		{
			if (has_value())
			{
				forward<Fn>(action)(this->value());
			}

			return *this;
		}

		template<invocables<T&&> Fn>
		inline constexpr
			Monad&&
			if_then(Fn&& action) &&
			noexcept(noexcept(forward<Fn>(action)(declval<T&&>())))
		{
			if (has_value())
			{
				forward<Fn>(action)(this->value());
			}

			return move(*this);
		}

		template<invocables<const T&&> Fn>
		inline constexpr
			const Monad&&
			if_then(Fn&& action) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<const T&&>())))
		{
			if (has_value())
			{
				forward<Fn>(action)(this->value());
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

			if (has_value())
			{
				return forward<Fn>(action)(this->value());
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

			if (has_value())
			{
				return forward<Fn>(action)(this->value());
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

			if (has_value())
			{
				return forward<Fn>(action)(this->value());
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

			if (has_value())
			{
				return forward<Fn>(action)(this->value());
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

			if (has_value())
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(this->value());
				}
				else
				{
					return forward<Lfn>(safe_action)(this->value());
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(this->value());
				}
				else
				{
					return forward<Rfn>(fail_action)(this->value());
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

			if (has_value())
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(this->value());
				}
				else
				{
					return forward<Lfn>(safe_action)(this->value());
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(this->value());
				}
				else
				{
					return forward<Rfn>(fail_action)(this->value());
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

			if (has_value())
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(this->value());
				}
				else
				{
					return forward<Lfn>(safe_action)(this->value());
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(this->value());
				}
				else
				{
					return forward<Rfn>(fail_action)(this->value());
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

			if (has_value())
			{
				if constexpr (is_same_v<safe_result_t, void>)
				{
					forward<Lfn>(safe_action)(this->value());
				}
				else
				{
					return forward<Lfn>(safe_action)(this->value());
				}
			}
			else
			{
				if constexpr (is_same_v<fail_result_t, void>)
				{
					forward<Rfn>(fail_action)(this->value());
				}
				else
				{
					return forward<Rfn>(fail_action)(this->value());
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

			if (has_value())
			{
				return static_cast<Uty>(forward<Fn>(safe_action)(this->value()));
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

			if (has_value())
			{
				return static_cast<Uty>(forward<Fn>(safe_action)(this->value()));
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

			if (has_value())
			{
				return static_cast<Uty>(forward<Fn>(safe_action)(this->value()));
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

			if (has_value())
			{
				return static_cast<Uty>(forward<Fn>(safe_action)(this->value()));
			}
			else
			{
				return forward<Uty>(default_value);
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			Monad&
			else_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&
			else_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			Monad&&
			else_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return util::move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&&
			else_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return util::move(*this);
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

			if (!has_value())
			{
				return forward<Fn>(action)(this->value(), forward<Args>(args)...);
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

			if (!has_value())
			{
				return forward<Fn>(action)(this->value(), forward<Args>(args)...);
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

			if (!has_value())
			{
				return forward<Fn>(action)(this->value(), forward<Args>(args)...);
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

			if (!has_value())
			{
				return forward<Fn>(action)(this->value(), forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<convertible_to<T> U>
		constexpr T value_or(const U& failsafe) const& noexcept(nothrow_constructibles<T>)
		{
			if (has_value())
			{
				return this->value();
			}
			else
			{
				return static_cast<T>(failsafe);
			}
		}

		constexpr T value_or(T&& failsafe) const& noexcept(nothrow_move_constructibles<T>)
		{
			if (has_value())
			{
				return this->value();
			}
			else
			{
				return failsafe;
			}
		}

		template<convertible_to<T> U>
		constexpr T value_or(const U& failsafe) && noexcept(nothrow_constructibles<T>)
		{
			if (has_value())
			{
				return this->value();
			}
			else
			{
				return static_cast<T>(failsafe);
			}
		}

		constexpr T&& value_or(T&& failsafe) && noexcept(nothrow_move_constructibles<T>)
		{
			if (has_value())
			{
				return this->value();
			}
			else
			{
				return move(failsafe);
			}
		}

		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		explicit constexpr operator T () const noexcept(nothrow_copy_constructibles<T>)
		{
			return this->value();
		}

		explicit constexpr operator bool() const noexcept
		{
			return hasValue;
		}

		constexpr Monad(const Monad&) noexcept(nothrow_copy_constructibles<T>) = default;
		constexpr Monad(Monad&&) noexcept(nothrow_move_constructibles<T>) = default;
		constexpr Monad& operator=(const Monad&) noexcept(nothrow_copy_assignables<T>) = default;
		constexpr Monad& operator=(Monad&&) noexcept(nothrow_move_assignables<T>) = default;
		constexpr ~Monad() noexcept(nothrow_destructibles<T>) = default;

	private:
		bool hasValue = false;
	};

	template<>
	class [[nodiscard]] Monad<void>
	{
	public:
		using value_type = void;

		constexpr Monad() noexcept
			: hasValue(true)
		{}

		constexpr Monad(nullopt_t) noexcept
			: hasValue(false)
		{}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			Monad&
			if_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&
			if_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			Monad&&
			if_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return std::move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&&
			if_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return std::move(*this);
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args)
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (has_value())
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			auto
			and_then(Fn&& action, Args&&... args) const
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (has_value())
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		template<invocables Lfn, invocables Rfn>
		inline constexpr
			monad_result_t<Lfn>
			transform(Lfn&& safe_action, Rfn&& fail_action) const
			noexcept(noexcept(forward<Lfn>(safe_action)) && noexcept(forward<Rfn>(fail_action)))
		{
			using safe_result_t = monad_result_t<Lfn>;
			using fail_result_t = monad_result_t<Rfn>;

			static_assert(same_as<safe_result_t, fail_result_t> || (same_as<safe_result_t, void> && same_as<fail_result_t, void>));

			if (has_value())
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

			if (has_value())
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
		inline constexpr
			Monad&
			else_then(Fn&& action, Args&&... args) &
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&
			else_then(Fn&& action, Args&&... args) const&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			Monad&&
			else_then(Fn&& action, Args&&... args) &&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			const Monad&&
			else_then(Fn&& action, Args&&... args) const&&
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			if (!has_value())
			{
				forward<Fn>(action)(forward<Args>(args)...);
			}

			return *this;
		}

		template<typename Fn, typename... Args>
			requires invocables<Fn, Args...>
		inline constexpr
			auto
			or_else(Fn&& action, Args&&... args) const
			noexcept(noexcept(forward<Fn>(action)(declval<Args>()...)))
		{
			using fwd_result_t = monad_result_t<Fn, Args...>;

			static_assert(!is_same_v<fwd_result_t, void>);
			static_assert(is_specialization_v<fwd_result_t, Monad>);

			using fwd_value_t = fwd_result_t::value_type;

			if (!has_value())
			{
				return forward<Fn>(action)(forward<Args>(args)...);
			}
			else
			{
				return Monad<fwd_value_t>{ nullopt };
			}
		}

		constexpr bool has_value() const noexcept
		{
			return hasValue;
		}

		explicit constexpr operator bool() const noexcept
		{
			return hasValue;
		}

		constexpr Monad(const Monad&) noexcept = default;
		constexpr Monad(Monad&&) noexcept = default;
		constexpr Monad& operator=(const Monad&) noexcept = default;
		constexpr Monad& operator=(Monad&&) noexcept = default;
		constexpr ~Monad() noexcept = default;

	private:
		bool hasValue;
	};

	using Proxy = Monad<void>;
}

namespace util
{
	inline void do_something() noexcept
	{}

	void test_monad() noexcept
	{
		Monad<int> monad4{ 1000 };
		const bool has4 = monad4.has_value();
		const Monad<int> monad5{ 1000 };
		const bool has5 = monad5.has_value();
		constexpr Monad<int> monad6{ 1000 };
		constexpr bool has6 = monad6.has_value();

		Monad<int> monad7{ int(80264.01954f) };
		const Monad<int> monad8{ false };
		constexpr Monad<int> monad9{ 1000ULL };

		const auto valor7 = monad7.value_or(42);

		const auto expr8_3 = monad8.transform(
			[](const int& v) -> float { return static_cast<float>(v); },
			[](const int& v) -> float { return v * 100.0f; }
		);

		const auto expr8_4 = monad8.translate([](auto&&) -> long long { return 15718742LL; }, -7);

		const auto& expr8_5 = monad8.else_then([]() { do_something(); });

		Monad<int> monad1{ false };
		const Monad<int> monad2{ false };
		constexpr Monad<int> monad3{ false };

		constexpr int valor3 = monad3.value_or(3000.0);

		constexpr const auto& expr9_1 = monad9.if_then([](auto&&) -> float { return 3000.0; });

		constexpr Monad<float> expr9_2 = monad9.and_then(
			[](auto&&) -> Monad<float> {
			return 3000.0;
		});

		constexpr double expr9_3 = monad9.transform(
			[](const int& value) -> double { return value + 3000.0; },
			[](auto&&) -> double { return 0.0; }
		);

		constexpr int expr9_4 = monad9.translate(
			[](auto&&) -> long long { return 15718742LL; },
			-7
		);

		constexpr const Monad<int>& expr9_5 = monad9.else_then(
			[]() {}
		);

		const auto expr9_6 = monad7.or_else([](const int& v) -> Monad<float> {
			return 5020.0f;
		});
	}
}
