module;
#include <vector>
#include <functional>

export module Utility.Option;
import Utility;
import Utility.Constraints;

export namespace util
{
	template<typename InternalOpt>
	class Option
	{
	public:
		using OptHandler = std::function<void(const InternalOpt& option)>;

		constexpr Option() noexcept
			: myOption()
			, myHandlers(8ULL)
		{}

		constexpr Option(const InternalOpt init) noexcept(nothrow_copy_constructibles<InternalOpt>)
			: myOption(init)
			, myHandlers(8ULL)
		{}

		constexpr Option(const InternalOpt init, const OptHandler handler) noexcept(nothrow_copy_constructibles<InternalOpt>)
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

		constexpr void AddListener(const OptHandler handler) noexcept
		{
			myHandlers.push_back(handler);
		}

		constexpr InternalOpt& Get() & noexcept
		{
			return myOption;
		}

		constexpr const InternalOpt& Get() const& noexcept
		{
			return myOption;
		}

		constexpr InternalOpt&& Get() && noexcept
		{
			return util::move(myOption);
		}

		constexpr const InternalOpt&& Get() const&& noexcept
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
		{}

		constexpr Option(const InternalOpt init, const OptHandler handler) noexcept
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

		constexpr void AddListener(const OptHandler handler) noexcept
		{
			myHandlers.push_back(handler);
		}

		constexpr InternalOpt& Get() & noexcept
		{
			return myOption;
		}

		constexpr const InternalOpt& Get() const& noexcept
		{
			return myOption;
		}

		constexpr InternalOpt&& Get() && noexcept
		{
			return util::move(myOption);
		}

		constexpr const InternalOpt&& Get() const&& noexcept
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
