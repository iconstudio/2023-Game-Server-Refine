module;
#include <vector>

export module Utility.Option;
import Utility;

export namespace util
{
	template<typename InternalOpt>
	class Option
	{
	public:
		using OptHandler = void(*)(const InternalOpt& option);

		constexpr Option(const InternalOpt init) noexcept
			: myOption(init)
			, myHandlers(8)
		{}

		constexpr Option(const InternalOpt init, OptHandler handler) noexcept
			: Option(init)
		{
			myHandlers.push_back(handler);
		}

		constexpr ~Option() noexcept
		{
			myHandlers.clear();
			myHandlers.shrink_to_fit();
		}

		constexpr Option& operator=(const InternalOpt& option) noexcept
		{
			if (option != myOption)
			{
				for (OptHandler& handler : myHandlers)
				{
					handler(option);
				}

				myOption = option;
			}

			return *this;
		}

		constexpr InternalOpt& get() & noexcept
		{
			return myOption;
		}

		constexpr const InternalOpt& get() const& noexcept
		{
			return myOption;
		}

		constexpr InternalOpt&& get() && noexcept
		{
			return util::move(myOption);
		}

		constexpr const InternalOpt&& get() const&& noexcept
		{
			return util::move(myOption);
		}

		constexpr InternalOpt& operator*() & noexcept
		{
			return myOption;
		}

		constexpr const InternalOpt& operator*() const& noexcept
		{
			return myOption;
		}

		constexpr InternalOpt&& operator*() && noexcept
		{
			return util::move(myOption);
		}

		constexpr const InternalOpt&& operator*() const&& noexcept
		{
			return util::move(myOption);
		}

		Option(const Option&) = delete;
		constexpr Option(Option&&) noexcept = default;
		Option& operator=(const Option&) = delete;
		constexpr Option& operator=(Option&&) noexcept = default;

	private:
		InternalOpt myOption;
		std::vector<OptHandler> myHandlers;
	};
}
