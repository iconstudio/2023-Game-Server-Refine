export module Game.Object;
import Utility.Indexer;
import Utility.Classifier;

export namespace game
{
	class Object
		: private util::Indexer<Object>
		, public util::Classifier<Object>
	{
	public:
		constexpr Object() noexcept;
		constexpr ~Object() noexcept;

		[[nodiscard]]
		constexpr size_t GetID() const noexcept
		{
			return GetIndex();
		}

		[[nodiscard]]
		constexpr bool operator==(const Object& other) const noexcept = default;
	};
}

export template<>
struct util::class_id<game::Object>
{
	static constexpr size_t id = 1;
};
