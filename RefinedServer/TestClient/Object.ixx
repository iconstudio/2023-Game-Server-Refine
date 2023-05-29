export module Game.Object;
import Utility.Indexer;

export namespace game
{
	class Object : private util::Indexer<Object>
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
