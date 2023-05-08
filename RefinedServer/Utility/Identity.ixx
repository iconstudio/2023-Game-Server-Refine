export module Utility.Identity;
import Utility.Traits;
import Utility.Constraints;

export namespace util
{
	template<typename T, typename Tag = void>
	class [[nodiscard]] Identity
	{
	public:
		using value_type = T;

		constexpr Identity()
			noexcept(nothrow_constructibles<T>) requires(default_initializables<T>)
			: myValue()
		{}

		constexpr Identity(const T& value)
			noexcept(nothrow_copy_constructibles<T>) requires(copy_constructibles<T>)
			: myValue(value)
		{}

		constexpr Identity(T&& value)
			noexcept(nothrow_move_constructibles<T>) requires(move_constructibles<T>)
			: myValue(static_cast<T&&>(value))
		{}

		constexpr Identity& operator=(const Identity& other)
			noexcept(nothrow_copy_assignables<T>)
			requires(copy_assignables<T>)
		{
			myValue = other.myValue;
			return *this;
		}

		constexpr Identity& operator=(Identity&& other)
			noexcept(nothrow_move_assignables<T>)
			requires(move_assignables<T>)
		{
			myValue = static_cast<T&&>(other.myValue);
			return *this;
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

		constexpr operator T& () & noexcept(nothrow_move_constructibles<T>)
		{
			return myValue;
		}

		constexpr operator const T& () const& noexcept(nothrow_move_constructibles<const T>)
		{
			return myValue;
		}

		constexpr operator T && () && noexcept(nothrow_copy_constructibles<T>)
		{
			return static_cast<T&&>(myValue);
		}

		constexpr operator const T && () const&& noexcept(nothrow_copy_constructibles<const T>)
		{
			return static_cast<const T&&>(myValue);
		}

		constexpr Identity(const Identity&) noexcept(nothrow_copy_constructibles<T>) = default;
		constexpr Identity(Identity&&) noexcept(nothrow_move_constructibles<T>) = default;
		constexpr ~Identity() noexcept(nothrow_destructibles<T>) = default;

	private:
		T myValue;
	};


	template<typename Tag>
	class Identity<void, Tag>
	{
	public:
		using value_type = void;

		constexpr Identity() noexcept = default;
		constexpr Identity(const Identity&) noexcept = default;
		constexpr Identity(Identity&&) noexcept = default;
		constexpr ~Identity() noexcept = default;

		constexpr Identity& operator=([[maybe_unused]] Identity) & noexcept
		{
			return *this;
		}

		constexpr const Identity& operator=([[maybe_unused]] Identity) const& noexcept
		{
			return *this;
		}

		constexpr Identity&& operator=([[maybe_unused]] Identity) && noexcept
		{
			return move(*this);
		}

		constexpr const Identity&& operator=([[maybe_unused]] Identity) const&& noexcept
		{
			return move(*this);
		}

		constexpr void swap([[maybe_unused]] Identity& other) noexcept
		{}

		constexpr void operator*() const noexcept
		{}
	};

	template<typename T, typename Tag>
	Identity(T, Tag) -> Identity<T, Tag>;

	template<typename T>
	Identity(T) -> Identity<T, void>;
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
