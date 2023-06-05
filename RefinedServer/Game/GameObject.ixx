module;
#include <glm/ext.hpp>

export module Game.GameObject;
import <memory>;
import <vector>;
import Utility.Constraints;
import Utility.Named;
import Utility.String;
import Utility.Memory.Watcher;
import System.PipelineObject;
import Game.Object;
import Game.Component;

export namespace game
{
	class alignas(32) GameObject
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
			static_assert(util::hierachy<util::clean_t<T>, GameObject>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(new T());
		}

		template<util::copyable T>
		[[nodiscard]]
		static inline
			std::unique_ptr<T>
			Instantiate(const T* const& original)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			static_assert(util::hierachy<util::clean_t<T>, GameObject>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(DeepCopy(original));
		}

		template<typename T>
		[[nodiscard]]
		static inline
			std::unique_ptr<T>
			Instantiate(const std::unique_ptr<T>& original)
			noexcept(util::nothrow_copy_constructibles<T>)
		{
			static_assert(util::hierachy<T, GameObject>, "T must be derived from GameObject!");

			return std::unique_ptr<GameObject>(DeepCopy(original.get()));
		}

		inline void Awake()
		{
			for (auto& component : myComponents)
			{
				component->Awake();
			}
		}

		inline void Start() noexcept
		{
			for (auto& component : myComponents)
			{
				component->Start();
			}
		}

		inline void Update(const float& delta_time)
		{
			for (auto& component : myComponents)
			{
				component->Update(delta_time);
			}
		}

		inline void Render(const void* context) noexcept
		{
			for (auto& component : myComponents)
			{
				component->Render(context);
			}
		}

		inline void Destroy()
		{
			for (auto& component : myComponents)
			{
				component->Destroy();
			}
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

		[[nodiscard]]
		inline GameObject* Clone() const
		{
			return DeepCopy(this);
		}

		GameObject(const GameObject& other) = delete;
		constexpr GameObject(GameObject&& other) noexcept = default;
		GameObject& operator=(const GameObject& other) = delete;
		constexpr GameObject& operator=(GameObject&& other) noexcept = default;

	protected:
		[[nodiscard]]
		static inline GameObject* DeepCopy(const GameObject* const& obj)
		{
			GameObject* copied_child = nullptr;
			util::Watcher watch_child{ copied_child };

			if (obj->HasChild())
			{
				copied_child = DeepCopy(obj->GetChild());
			}

			GameObject* copied_sibling = nullptr;
			util::Watcher watch_sib{ copied_sibling };

			if (obj->HasSibling())
			{
				copied_sibling = DeepCopy(obj->GetSibling());
			}

			GameObject* result = new GameObject{};
			util::Watcher watch_obj{ result };

			const std::vector<std::unique_ptr<Component>>& components = obj->myComponents;

			std::vector<std::unique_ptr<Component>> temp_list{};
			temp_list.reserve(components.size());

			for (const std::unique_ptr<Component>& component : components)
			{
				temp_list.emplace_back(component->Clone());
			}

			result->myChild = std::unique_ptr<GameObject>{ copied_child };
			result->mySibling = std::unique_ptr<GameObject>{ copied_sibling };
			result->myComponents = std::move(temp_list);

			// Guarded by exception
			watch_child.Ok();
			watch_sib.Ok();
			watch_obj.Ok();
			return result;
		}

		std::unique_ptr<GameObject> myChild = nullptr;
		std::unique_ptr<GameObject> mySibling = nullptr;

		std::vector<std::unique_ptr<Component>> myComponents{};
	};

	using GameObjectHandle = std::unique_ptr<GameObject>;
}

export template<>
struct util::class_id<game::GameObject>
{
	static inline constexpr size_t id = 2;
};
