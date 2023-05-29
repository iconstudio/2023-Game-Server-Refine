export module Game.Object;
import Utility.Indexer;
export import Utility.Classifier;

export namespace game
{
	class Object
		: private util::Indexer<Object>
		, public util::Classifier<Object>
	{
	public:
		constexpr Object() noexcept = default;
		constexpr ~Object() noexcept = default;

		[[nodiscard]]
		constexpr size_t GetID() const noexcept
		{
			return GetIndex();
		}

		[[nodiscard]]
		constexpr bool operator==(const Object& other) const noexcept = default;

		Object(const Object& other) = delete;
		constexpr Object(Object&& other) noexcept = default;
		Object& operator=(const Object& other) = delete;
		constexpr Object& operator=(Object&& other) noexcept = default;
	};
}

export template<>
struct util::class_id<game::Object>
{
	static constexpr size_t id = 1;
};
