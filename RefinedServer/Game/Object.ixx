module;
#include "Object.cc"

export module Game.Object;
import Utility.Indexer;
export import Utility.Classifier;

export namespace game
{
	class alignas(32) Object
		: private util::Indexer<Object>
		, public util::Classifier<Object>
		//, public util::Object
	{
	public:
		constexpr Object() noexcept = default;
		constexpr ~Object() noexcept = default;

		constexpr void SetEnabled(const bool enabled) noexcept
		{
			isEnabled = enabled;
		}

		[[nodiscard]]
		constexpr bool IsEnabled() const noexcept
		{
			return isEnabled;
		}

		[[nodiscard]]
		constexpr const size_t& GetID() const& noexcept
		{
			return Indexer<Object>::GetIndex();
		}

		[[nodiscard]]
		constexpr size_t&& GetID() && noexcept
		{
			return static_cast<Object&&>(*this).Indexer<Object>::GetIndex();
		}

		[[nodiscard]]
		constexpr bool operator==(const Object& other) const noexcept = default;

		constexpr Object(Object&& other) noexcept = default;
		constexpr Object& operator=(Object&& other) noexcept = default;

	protected:
		constexpr Object(const Object& other) noexcept = default;
		constexpr Object& operator=(const Object& other) noexcept = default;

		bool isEnabled = true;
	};
}

export template<>
struct util::class_id<game::Object>
{
	static inline constexpr size_t id = 1;
};
