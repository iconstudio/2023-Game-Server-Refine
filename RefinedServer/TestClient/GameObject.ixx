module;
#include <glm/ext.hpp>

export module Game.GameObject;
import <type_traits>;
import <new>;
import <string_view>;
import <memory>;
import <vector>;
import Utility.Constraints;
import Utility.Named;
import System.PipelineObject;
import Game.Object;
import Game.Component;

export namespace game
{
	class GameObject final
		: public Object
		, public util::Named
		, public util::Classifier<GameObject>
	{
	public:
		constexpr GameObject() noexcept = default;
		constexpr ~GameObject() noexcept = default;

		template<typename T>
		[[nodiscard]]
		static constexpr
			std::unique_ptr<T>
			Instantiate()
			noexcept(util::nothrow_default_constructibles<T>)
		{
			static_assert(std::is_base_of_v<GameObject, util::clean_t<T>>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(new T());
		}

		template<typename T>
			requires(util::copyable<T>)
		[[nodiscard]]
		static constexpr
			std::unique_ptr<T>
			Instantiate(T* original)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			static_assert(std::is_base_of_v<GameObject, util::clean_t<T>>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(new T(*original));
		}

		template<typename T>
		[[nodiscard]]
		static constexpr
			std::unique_ptr<T>
			Instantiate(const std::unique_ptr<T>& original)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			static_assert(std::is_base_of_v<GameObject, T>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(new T(*original.get()));
		}

		constexpr void Reset() noexcept
		{}

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

		constexpr void AddChild(std::unique_ptr<GameObject>&& child) noexcept
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

		template<typename O>
		constexpr void AddChild(std::unique_ptr<O>&& child) noexcept
		{
			static_assert(std::is_base_of_v<GameObject, O>);

			if (!HasChild())
			{
				SetChild(std::unique_ptr<GameObject>(child.release()));
			}
			else
			{
				myChild->AddSibling(std::move(child));
			}
		}

		constexpr void AddSibling(std::unique_ptr<GameObject>&& sibling) noexcept
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

		template<typename O>
		constexpr void AddSibling(std::unique_ptr<O>&& sibling) noexcept
		{
			static_assert(std::is_base_of_v<GameObject, O>);

			if (HasSibling())
			{
				mySibling->AddSibling(std::unique_ptr<GameObject>(sibling.release()));
			}
			else
			{
				SetSibling(std::unique_ptr<GameObject>(sibling.release()));
			}
		}

		constexpr void AddComponent(std::unique_ptr<Component>&& component) noexcept
		{
			myComponents.push_back(std::move(component));
		}

		template<typename C>
		constexpr void AddComponent(std::unique_ptr<C>&& component) noexcept
		{
			static_assert(std::is_base_of_v<Component, C>);

			myComponents.emplace_back(component.release());
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

		[[nodiscard]]
		constexpr GameObject* const& GetChild() const
		{
			return myChild.get();
		}

		[[nodiscard]]
		constexpr GameObject* const& GetSibling() const
		{
			return mySibling.get();
		}

		[[nodiscard]]
		constexpr bool HasChild() const noexcept
		{
			return nullptr != myChild;
		}

		[[nodiscard]]
		constexpr bool HasSibling() const noexcept
		{
			return nullptr != mySibling;
		}

		GameObject(const GameObject& other) = delete;
		constexpr GameObject(GameObject&& other) noexcept = default;
		GameObject& operator=(const GameObject& other) = delete;
		constexpr GameObject& operator=(GameObject&& other) noexcept = default;

	protected:
		[[nodiscard]]
		constexpr GameObject* DeepCopy(const GameObject* const& obj) const
		{
			GameObject* copied_child = nullptr;
			GameObject* copied_sibling = nullptr;

			GameObject* result = new GameObject{};
			try
			{
				if (obj->HasChild())
				{
					copied_child = DeepCopy(obj->GetChild());
				}

				if (obj->HasSibling())
				{
					copied_sibling = DeepCopy(obj->GetSibling());
				}

				std::vector<std::unique_ptr<Component>> temp_list{};
				temp_list.reserve(obj->myComponents.size());

				for (const std::unique_ptr<Component>& component : obj->myComponents)
				{
					temp_list.emplace_back(component->Clone());
				}

				result->myChild = std::unique_ptr<GameObject>{ copied_child };
				result->mySibling = std::unique_ptr<GameObject>{ copied_sibling };
				result->myComponents = std::move(temp_list);
			}
			catch (...)
			{
				delete result;
				throw;
			}

			return result;
		}

		std::unique_ptr<GameObject> myChild = nullptr;
		std::unique_ptr<GameObject> mySibling = nullptr;

		std::vector<std::unique_ptr<Component>> myComponents{};
	};
}

export template<>
struct util::class_id<game::GameObject>
{
	static constexpr size_t id = 2;
};
