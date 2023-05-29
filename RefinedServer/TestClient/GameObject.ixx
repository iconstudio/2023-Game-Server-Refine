export module Game.GameObject;
import <new>;
import <string_view>;
import <memory>;
import <vector>;
import Utility.Named;
import System.PipelineObject;
import Game.Object;
import Game.Component;

export namespace game
{
	class GameObject
		: public Object
		, public util::Named
		, public util::Classifier<GameObject>
	{
	public:
		constexpr GameObject() noexcept = default;
		constexpr ~GameObject() noexcept = default;

		constexpr void SetChild(std::unique_ptr<GameObject>&& child) noexcept
		{
			myChild = std::move(child);
		}

		constexpr bool TrySetChild(std::unique_ptr<GameObject>&& child) noexcept
		{
			if (HasChild())
			{
				return false;
			}
			else
			{
				SetChild(std::move(child));
				return true;
			}
		}

		constexpr void SetSibling(std::unique_ptr<GameObject>&& sibling) noexcept
		{
			mySibling = std::move(sibling);
		}

		constexpr bool TrySetSibling(std::unique_ptr<GameObject>&& sibling) noexcept
		{
			if (HasSibling())
			{
				return false;
			}
			else
			{
				SetSibling(std::move(sibling));
				return true;
			}
		}

		void AddChild(std::unique_ptr<GameObject>&& child) noexcept
		{
			if (!HasChild())
			{
				SetChild(std::move(child));
			}
			else
			{
				myChild->AddSibling(std::move(child));
			}
		}

		void AddSibling(std::unique_ptr<GameObject>&& sibling) noexcept
		{
			if (HasSibling())
			{
				mySibling->AddSibling(std::move(sibling));
			}
			else
			{
				SetSibling(std::move(sibling));
			}
		}

		constexpr void DestroyChildren() noexcept
		{
			if (HasChild())
			{
				// Destroy all children first
				myChild->DestroyChildren();

				myChild = nullptr;
			}
		}

		constexpr void DestroySiblings() noexcept
		{
			if (HasSibling())
			{
				// Destroy my siblings first
				mySibling->DestroySiblings();

				mySibling = nullptr;
			}
		}

		constexpr GameObject* const& GetChild() const
		{
			return myChild.get();
		}

		constexpr GameObject* const& GetSibling() const
		{
			return mySibling.get();
		}

		constexpr bool HasChild() const noexcept
		{
			return nullptr != myChild;
		}

		constexpr bool HasSibling() const noexcept
		{
			return nullptr != mySibling;
		}

		GameObject(const GameObject& other) = delete;
		constexpr GameObject(GameObject&& other) noexcept = default;
		GameObject& operator=(const GameObject& other) = delete;
		constexpr GameObject& operator=(GameObject&& other) noexcept = default;

	protected:
		std::unique_ptr<GameObject> myChild = nullptr;
		std::unique_ptr<GameObject> mySibling = nullptr;

		std::vector<std::unique_ptr<Component>> myComponents;
	};
}

export template<>
struct util::class_id<game::GameObject>
{
	static constexpr size_t id = 2;
};
