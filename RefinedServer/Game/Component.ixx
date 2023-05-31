export module Game.Component;
import <memory>;
import Utility.Constraints;
import Utility.Classifier;
export import System.PipelineObject;
export import Game.Object;

export namespace game
{
	class [[nodiscard]] alignas(32) Component
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
		virtual constexpr Component* Clone() const
		{
			return DeepCopy(this);
		}

		constexpr Component(Component&& other) noexcept = default;
		constexpr Component& operator=(Component&& other) noexcept = default;

	protected:
		template<typename T>
		[[nodiscard]]
		static constexpr T* DeepCopy(const T* const& component)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			static_assert(util::hierachy<T, Component>, "T must be a Component");

			return new T(*component);
		}

		constexpr Component(const Component& other) noexcept = default;
		constexpr Component& operator=(const Component& other) noexcept = default;

		Object* myOwner = nullptr;
	};
}

export template<>
struct util::class_id<game::Component>
{
	static inline constexpr size_t id = 3;
};
