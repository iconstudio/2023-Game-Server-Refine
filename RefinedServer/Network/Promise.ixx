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
				: value(value)
			{}

			explicit constexpr success_t(T&& value) noexcept
				: value(static_cast<T&&>(value))
			{}

			constexpr operator T& () & noexcept
			{
				return value;
			}

			constexpr operator const T& () const& noexcept
			{
				return value;
			}

			constexpr operator T && () && noexcept
			{
				return static_cast<T&&>(value);
			}

			constexpr operator const T && () const&& noexcept
			{
				return static_cast<const T&&>(value);
			}

			constexpr ~success_t() noexcept(util::nothrow_destructibles<T>) = default;

			T value;
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
		struct success_t { explicit constexpr success_t() noexcept = default; };

		inline constexpr error_t error(const int& error_code) noexcept
		{
			return error_t{ error_code };
		}

		inline constexpr error_t error(int&& error_code) noexcept
		{
			return error_t{ static_cast<int&&>(error_code) };
		}

		inline constexpr defer_t defer{ };
		inline constexpr success_t success{ };
	}

	class Promise
	{
	public:
		using monad_t = util::LooseMonad<io::success_t, io::error_t, io::defer_t>;

		constexpr Promise() noexcept
			: myState()
		{}

		constexpr Promise(Promise&& other) noexcept
			: myState(static_cast<monad_t&&>(other.myState))
		{}

		monad_t myState;
	};
}
