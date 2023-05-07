export module Net.Promise;
import Utility;
import Utility.Monad;
import Utility.Monad.Loosen;
import Net;

export namespace net
{
	namespace io
	{
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
