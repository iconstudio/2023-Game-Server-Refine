export module Game.Component;
import <memory>;
export import Game.Object;

export namespace game
{
	class [[nodiscard]] Component
		: public Object
		, public util::Classifier<Component>
	{
	public:
		constexpr Component() = default;
		constexpr ~Component() = default;

		constexpr void SetOwner(Object* object) noexcept
		{
			myOwner = object;
		}

		[[nodiscard]]
		constexpr Object* const& GetOwner() const noexcept
		{
			return myOwner;
		}

		Component(const Component&) = delete;
		constexpr Component(Component&&) = default;
		Component& operator=(const Component&) = delete;
		constexpr Component& operator=(Component&&) = default;

		Object* myOwner = nullptr;
	};
}

export template<>
struct util::class_id<game::Component>
{
	static constexpr size_t id = 3;
};
