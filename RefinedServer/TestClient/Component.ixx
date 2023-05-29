export module Game.Component;
import <memory>;
export import Game.Object;
export import Game.GameObject;

export namespace game
{
	class [[nodiscard]] Component
		: public Object
		, public util::Classifier<Component>
	{
	public:
		Component() = default;
		Component(const Component&) = delete;
		Component(Component&&) = default;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = default;
		constexpr ~Component() = default;

		constexpr void SetGameObject(GameObject* object) noexcept
		{
			gameObject = object;
		}

		[[nodiscard]]
		constexpr GameObject* const& GetGameObject() const noexcept
		{
			return gameObject;
		}

		GameObject* gameObject = nullptr;
	};
}

export template<>
struct util::class_id<game::Component>
{
	static constexpr size_t id = 3;
};
