export module Net.IoState;
import Utility;
import Utility.Constraints;
export import Utility.Identity;

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
	constexpr auto make_success(T&& value)
		noexcept(util::nothrow_constructibles<success_t<T>, util::add_rvalue_reference_t<T>>)
		-> success_t<util::clean_t<T>>
	{
		return success_t<util::clean_t<T>>{ util::forward<T>(value) };
	}

	constexpr just_success_t make_success()
		noexcept
	{
		return success;
	}

	template<typename T>
	constexpr auto make_defered(T&& value)
		noexcept(util::nothrow_constructibles<defer_t<T>, util::add_rvalue_reference_t<T>>)
		-> defer_t<util::clean_t<T>>
	{
		return defer_t<util::clean_t<T>>{ util::forward<T>(value) };
	}

	constexpr just_defered_t make_defered()
		noexcept
	{
		return defered;
	}

	template<typename T>
	constexpr auto make_failure(T&& value)
		noexcept(util::nothrow_constructibles<failure_t<T>, util::add_rvalue_reference_t<T>>)
		-> failure_t<util::clean_t<T>>
	{
		return failure_t<util::clean_t<T>>{ util::forward<T>(value) };
	}

	constexpr just_failure_t make_failure()
		noexcept
	{
		return failure;
	}
}
