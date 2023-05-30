export module Game.Component;
import Utility.Classifier;
export import System.PipelineObject;
export import Game.Object;

export namespace game
{
	class [[nodiscard]] Component
		: public Object
		, public sys::PipelineModel
		, public util::Classifier<Component>
	{
	public:
		constexpr Component() = default;
		virtual constexpr ~Component() = default;

		constexpr void SetOwner(Object* object) noexcept
		{
			myOwner = object;
		}

		[[nodiscard]]
		constexpr Object* const& GetOwner() const noexcept
		{
			return myOwner;
		}

		[[nodiscard]]
		constexpr Component DeepCopy(const Component& component) const noexcept
		{
			return Component(component);
		}

		constexpr Component(Component&& other) noexcept = default;
		constexpr Component& operator=(Component&& other) noexcept = default;

	protected:
		constexpr Component(const Component& other) noexcept = default;
		constexpr Component& operator=(const Component& other) noexcept = default;

		Object* myOwner = nullptr;
	};
}

export template<>
struct util::class_id<game::Component>
{
	static constexpr size_t id = 3;
};
