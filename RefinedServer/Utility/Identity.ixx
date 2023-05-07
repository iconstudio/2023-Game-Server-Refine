export module Utility.Identity;
import Utility.Traits;
import Utility.Constraints;

export namespace util
{
	template<typename T>
	class [[nodiscard]] Identity
	{
	public:
		using value_type = T;

		constexpr Identity()
			noexcept(nothrow_constructibles<T>)
			requires(default_initializables<T>)
			: myValue()
		{}

		constexpr Identity(const T& value)
			noexcept(nothrow_copy_constructibles<T>)
			requires(copy_constructibles<T>)
			: myValue(value)
		{}

		constexpr Identity(T&& value)
			noexcept(nothrow_move_constructibles<T>)
			requires(move_constructibles<T>)
			: myValue(static_cast<T&&>(value))
		{}

		constexpr Identity& operator=(const Identity& other) &
			noexcept(nothrow_copy_assignables<T>)
			requires(copy_assignables<T>)
		{
			myValue = other.myValue;
			return *this;
		}

		constexpr Identity& operator=(Identity&& other) &
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<T>)
		{
			myValue = static_cast<T&&>(other.myValue);
			return *this;
		}

		constexpr Identity& operator=(const Identity&& other) &
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<T>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			return *this;
		}

		constexpr const Identity& operator=(const Identity& other) const&
			noexcept(nothrow_copy_assignables<T>)
			requires(copy_assignables<const T>)
		{
			myValue = other.myValue;
			return *this;
		}

		constexpr const Identity& operator=(Identity&& other) const&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<const T>)
		{
			myValue = static_cast<T&&>(other.myValue);
			return *this;
		}

		constexpr const Identity& operator=(const Identity&& other) const&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<const T>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			return *this;
		}

		constexpr Identity&& operator=(const Identity& other) &&
			noexcept(nothrow_copy_assignables<T>)
			requires(copy_assignables<T>)
		{
			myValue = other.myValue;
			return static_cast<Identity&&>(*this);
		}

		constexpr Identity&& operator=(Identity&& other) &&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<T>)
		{
			myValue = static_cast<T&&>(other.myValue);
			return static_cast<Identity&&>(*this);
		}

		constexpr Identity&& operator=(const Identity&& other) &&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<T>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			return static_cast<Identity&&>(*this);
		}

		constexpr const Identity&& operator=(const Identity& other) const&&
			noexcept(nothrow_copy_assignables<T>)
			requires(copy_assignables<const T>)
		{
			myValue = other.myValue;
			return static_cast<const Identity&&>(*this);
		}

		constexpr const Identity&& operator=(Identity&& other) const&&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<const T>)
		{
			myValue = static_cast<T&&>(other.myValue);
			return static_cast<const Identity&&>(*this);
		}

		constexpr const Identity&& operator=(const Identity&& other) const&&
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<const T>)
		{
			myValue = static_cast<const T&&>(other.myValue);
			return static_cast<const Identity&&>(*this);
		}

		constexpr void swap(Identity& other) noexcept(nothrow_swappables<T>)
		{
			T temp = static_cast<T&&>(myValue);
			myValue = static_cast<T&&>(other.myValue);
			other.myValue = static_cast<T&&>(temp);
		}

		constexpr T& value() & noexcept
		{
			return myValue;
		}

		constexpr const T& value() const& noexcept
		{
			return myValue;
		}

		constexpr T&& value() &&
			noexcept(nothrow_move_constructibles<T>)
		{
			return static_cast<T&&>(myValue);
		}

		constexpr const T&& value() const&&
			noexcept(nothrow_move_constructibles<const T>)
		{
			return static_cast<const T&&>(myValue);
		}

		constexpr T& operator*() & noexcept
		{
			return myValue;
		}

		constexpr const T& operator*() const& noexcept
		{
			return myValue;
		}

		constexpr T&& operator*() && 
			noexcept(nothrow_move_constructibles<T>)
			requires(move_constructibles<T>)
		{
			return static_cast<T&&>(myValue);
		}

		constexpr const T&& operator*() const&&
			noexcept(nothrow_move_constructibles<T>)
			requires(move_constructibles<const T>)
		{
			return static_cast<const T&&>(myValue);
		}

		constexpr T get() const&
			noexcept(nothrow_copy_constructibles<T>)
		{
			return myValue;
		}

		constexpr T&& get() &&
			noexcept(nothrow_move_constructibles<T>)
			requires(move_constructibles<T>)
		{
			return static_cast<T&&>(myValue);
		}

		explicit constexpr operator T () const
			noexcept(nothrow_copy_constructibles<T>)
		{
			return myValue;
		}

		constexpr Identity(const Identity&) noexcept(nothrow_copy_constructibles<T>) = default;
		constexpr Identity(Identity&&) noexcept(nothrow_move_constructibles<T>) = default;
		constexpr ~Identity() noexcept(nothrow_destructibles<T>) = default;

	private:
		T myValue;
	};
}

export namespace std
{
	template<typename T>
	constexpr void
		swap(util::Identity<T>& lhs, util::Identity<T>& rhs)
		noexcept(util::nothrow_swappables<T>)
	{
		return lhs.swap(rhs);
	}
}
