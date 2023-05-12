export module Net.IoState;
import Utility.Constraints;
import Utility.Identity;

export namespace net::io
{
	namespace tags
	{
		enum class io_success {};
		enum class io_failure {};
		enum class io_defered {};
	}

	template<typename T> // T: Success Handle
	using success_t = util::Identity<T, tags::io_success>;
	template<typename T> // T: Error
	using failure_t = util::Identity<T, tags::io_failure>;
	template<typename T> // T: Cause of Defer
	using defer_t = util::Identity<T, tags::io_defered>;

	constexpr failure_t<int> error(const int& error_code) noexcept
	{
		return failure_t<int>{ error_code };
	}
	constexpr failure_t<int> error(int&& error_code) noexcept
	{
		return failure_t<int>{ static_cast<int&&>(error_code) };
	}

	using just_success_t = success_t<void>;
	using just_failure_t = failure_t<void>;
	using just_defered_t = defer_t<void>;

	inline constexpr just_success_t success{ };
	inline constexpr just_failure_t failure{ };
	inline constexpr just_defered_t defered{ };

	template<typename T>
		requires(util::notvoids<T>)
	constexpr success_t<T> make_success(const T& value)
		noexcept(util::nothrow_copy_constructibles<T>)
	{
		return success_t<T>{ value };
	}

	template<typename T>
		requires(util::notvoids<T>)
	constexpr success_t<T> make_success(T&& value)
		noexcept(util::nothrow_copy_constructibles<T>)
	{
		return success_t<T>{ static_cast<T&&>(value) };
	}

	constexpr just_success_t make_success()
		noexcept
	{
		return success;
	}

	template<typename T>
		requires(util::notvoids<T>)
	constexpr failure_t<T> make_failure(const T& value)
		noexcept(util::nothrow_copy_constructibles<T>)
	{
		return failure_t<T>{ value };
	}

	template<typename T>
		requires(util::notvoids<T>)
	constexpr failure_t<T> make_failure(T&& value)
		noexcept(util::nothrow_copy_constructibles<T>)
	{
		return failure_t<T>{ static_cast<T&&>(value) };
	}

	constexpr just_failure_t make_failure()
		noexcept
	{
		return failure;
	}
}
