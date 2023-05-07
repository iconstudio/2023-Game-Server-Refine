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

		struct error_t
		{
			constexpr error_t(const int& error_code) noexcept
				: errorCode(error_code)
			{}

			constexpr error_t(int&& error_code) noexcept
				: errorCode(static_cast<int&&>(error_code))
			{}

			int errorCode;
		};

		struct defer_t { explicit constexpr defer_t() noexcept = default; };

		inline constexpr error_t error(const int& error_code) noexcept
		{
			return error_t{ error_code };
		}
		inline constexpr error_t error(int&& error_code) noexcept
		{
			return error_t{ static_cast<int&&>(error_code) };
		}

		inline constexpr defer_t defer{ };
		inline constexpr success_t<void> success{ };

		template<typename T>
		inline constexpr success_t<T> succeed_io{ };
		inline constexpr defer_t      defered_io{ };
		inline constexpr error_t      failed_io{ -1 };
	}

	class Promise
	{
	public:
		using monad_t = util::LooseMonad<io::success_t<void>, io::error_t, io::defer_t>;

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

		constexpr Promise(io::success_t<void>) noexcept
			: myState(util::in_place_type<io::success_t<void>>, io::success)
		{}

		constexpr Promise(const io::error_t& error) noexcept
			: myState(util::in_place_type<io::error_t>, error)
		{}

		constexpr Promise(io::error_t&& error) noexcept
			: myState(util::in_place_type<io::error_t>, static_cast<io::error_t&&>(error))
		{}

		constexpr Promise(io::defer_t) noexcept
			: myState(util::in_place_type<io::defer_t>, io::defer)
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

	private:
		monad_t myState;
	};
}
