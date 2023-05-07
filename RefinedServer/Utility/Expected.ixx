module;
#include <optional>

export module Utility.Expected;
import Utility.Constraints;

export namespace util
{
	template<typename T, typename E>
	class Expected
	{
	public:
		using value_type = T;
		using error_type = E;

		constexpr Expected() noexcept(nothrow_constructibles<T>)
			: myValue()
		{}

		constexpr Expected(const T& value) noexcept(nothrow_copy_constructibles<T>)
			: myValue(value)
		{}

		constexpr Expected(T&& value) noexcept(nothrow_move_constructibles<T>)
			: myValue(static_cast<T&&>(value))
		{}

		constexpr Expected(const E& error) noexcept(nothrow_copy_constructibles<E>)
			: myError(error)
		{}

		constexpr Expected(E&& error) noexcept(nothrow_move_constructibles<E>)
			: myError(static_cast<E&&>(error))
		{}

		constexpr Expected(const Expected& other) noexcept(nothrow_copy_constructibles<T, E>)
			: myValue(other.myValue)
			, myError(other.myError)
		{}

		constexpr Expected(Expected&& other) noexcept(nothrow_move_constructibles<T, E>)
			: myValue(static_cast<T&&>(other.myValue))
			, myError(static_cast<E&&>(other.myError))
		{}

		constexpr Expected& operator=(const Expected& other) &
			noexcept(nothrow_copy_assignables<T, E>)
		{
			myValue = other.myValue;
			myError = other.myError;
			return *this;
		}

		constexpr Expected& operator=(Expected&& other) &
			noexcept(nothrow_move_assignables<T, E>)
		{
			myValue = static_cast<T&&>(other.myValue);
			myError = static_cast<E&&>(other.myError);
			return *this;
		}

		constexpr Expected& operator=(const Expected&& other) &
			noexcept(nothrow_move_assignables<T, E>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			myError = static_cast<const E&&>(other.myError);
			return *this;
		}

		constexpr const Expected& operator=(const Expected& other) const&
			noexcept(nothrow_copy_assignables<T, E>)
		{
			myValue = other.myValue;
			myError = other.myError;
			return *this;
		}

		constexpr const Expected& operator=(Expected&& other) const&
			noexcept(nothrow_move_assignables<T, E>)
		{
			myValue = static_cast<T&&>(other.my);
		}

		constexpr const Expected& operator=(const Expected&& other) const&
			noexcept(nothrow_move_assignables<T, E>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			myError = static_cast<const E&&>(other.myError);
			return *this;
		}

		constexpr const T& value() const& noexcept
		{
			return myValue;
		}

		constexpr T& value() & noexcept
		{
			return myValue;
		}

		constexpr const T&& value() const&& noexcept
		{
			return static_cast<const T&&>(myValue);
		}

		constexpr T&& value() && noexcept
		{
			return static_cast<T&&>(myValue);
		}

		constexpr const E& error() const& noexcept
		{
			return myError;
		}

		constexpr E& error() & noexcept
		{
			return myError;
		}

		constexpr const E&& error() const&& noexcept
		{
			return static_cast<const E&&>(myError);
		}

		constexpr E&& error() && noexcept
		{
			return static_cast<E&&>(myError);
		}

		constexpr bool hasValue() const noexcept
		{
			return myValue.has_value();
		}

		constexpr bool hasError() const noexcept
		{
			return myError.has_value();
		}

		constexpr operator bool() const noexcept
		{
			return hasValue();
		}

		constexpr const T& operator*() const& noexcept
		{
			return myValue.value();
		}

		constexpr T& operator*() & noexcept
		{
			return myValue.value();
		}

		constexpr const T&& operator*() const&& noexcept
		{
			return static_cast<const T&&>(myValue.value());
		}

		constexpr T&& operator*() && noexcept
		{
			return static_cast<T&&>(myValue.value());
		}

		constexpr void swap(Expected& other) noexcept(nothrow_swappables<T, E>)
		{
			std::swap(myValue, other.myValue);
			std::swap(myError, other.myError);
		}

		private:
			std::optional<T> myValue;
			std::optional<E> myError;
	};
}
