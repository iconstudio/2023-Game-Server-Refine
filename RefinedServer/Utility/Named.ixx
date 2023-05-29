export module Utility.Named;
import <string>;
import <string_view>;

export namespace util
{
	/// <summary>
	/// Make a named object.
	/// </summary>
	class Named
	{
	public:
		constexpr Named() noexcept
			: myName()
		{}

		constexpr Named(const std::string& name) noexcept
			: myName(name)
		{}

		constexpr Named(std::string&& name) noexcept
			: myName(static_cast<std::string&&>(name))
		{}

		constexpr Named(std::string_view name) noexcept
			: myName(name)
		{}

		constexpr ~Named() noexcept = default;

		constexpr void SetName(const std::string& name) noexcept
		{
			myName = name;
		}

		constexpr void SetName(std::string&& name) noexcept
		{
			myName = static_cast<std::string&&>(name);
		}

		constexpr void SetName(std::string_view name) noexcept
		{
			myName = name;
		}

		[[nodiscard]]
		constexpr const std::string& GetName() const& noexcept
		{
			return myName;
		}

		[[nodiscard]]
		constexpr std::string&& GetName() && noexcept
		{
			return static_cast<std::string&&>(myName);
		}

	private:
		std::string myName;
	};
}
