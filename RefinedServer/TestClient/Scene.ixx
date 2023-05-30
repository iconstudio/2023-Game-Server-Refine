module;
#define SCENE_ATOMIC_FLAGS false
export module Game.Scene;

#if SCENE_ATOMIC_FLAGS
import Utility.Atomic;

#define SCENE_CONSTEXPR inline
#define GET_FLAG(flag) flag.load(util::memory_order_relaxed)
using FLAG_T = util::atomic_bool;

#else // SCENE_ATOMIC_FLAGS

#define SCENE_CONSTEXPR constexpr
#define GET_FLAG(flag) flag
using FLAG_T = bool;
#endif // !SCENE_ATOMIC_FLAGS

import <vector>;
import <algorithm>;
import <ranges>;
import Utility;
import Utility.Constraints;
import Utility.Named;
import Utility.Indexer;
import Utility.String;
import System.PipelineObject;
export import Game.Camera;
export import Game.GameObject;

export extern "C++" namespace game
{
	class [[nodiscard]] alignas(64) Scene
		: public sys::PipelineModel
		, public util::Named
		, public util::Indexer<Scene>
	{
	public:
		constexpr Scene() noexcept
			: Scene("Scene")
		{}

		constexpr Scene(util::string_view name) noexcept
			: Named(name), Indexer<Scene>()
		{
			gameObjects.reserve(10ULL);
		}

#if SCENE_ATOMIC_FLAGS
		inline Scene(Scene&& other) noexcept
			: Named(static_cast<Scene&&>(other).GetName())
			, Indexer<Scene>(static_cast<Indexer<Scene>&&>(other))
			, gameObjects(static_cast<std::vector<GameObjectHandle>&&>(other.gameObjects))
			, isAwaken(other.IsAwaken())
			, isPaused(other.IsPaused())
			, isCompleted(other.IsCompleted())
		{}

		inline Scene& operator=(Scene&& other) noexcept
		{
			Swap(other);

			return *this;
		}
#else
		SCENE_CONSTEXPR Scene(Scene&& other) noexcept = default;
		SCENE_CONSTEXPR Scene& operator=(Scene&& other) noexcept = default;
#endif

		virtual SCENE_CONSTEXPR ~Scene() noexcept = default;

		constexpr void AddInstance(GameObjectHandle&& gameObject) noexcept
		{
			gameObjects.emplace_back(static_cast<GameObjectHandle&&>(gameObject));
		}

		constexpr void RemoveInstance(const GameObject& gameObject) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&gameObject](const GameObjectHandle& ptr) {
				return ptr.get() == util::addressof(gameObject);
			});
		}

		constexpr void RemoveInstance(std::string_view name) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&name](const GameObjectHandle& ptr) {
				return ptr->GetName() == name;
			});
		}

		virtual SCENE_CONSTEXPR
			void
			Swap(Scene& other) noexcept
		{
			gameObjects.swap(other.gameObjects);

			bool temp1 = IsAwaken();
			isAwaken = other.IsAwaken();
			other.isAwaken = temp1;

			bool temp2 = IsPaused();
			isPaused = other.IsPaused();
			other.isPaused = temp2;

			bool temp3 = IsCompleted();
			isCompleted = other.IsCompleted();
			other.isCompleted = temp3;
		}

		/// <summary>
		/// Resetting the status to the moment of awakening.
		/// </summary>
		virtual void Reset() noexcept
		{
			if (!isAwaken)
			{
				Awake();
				isAwaken = true;

				return;
			}

			std::ranges::for_each(gameObjects
				, [](const GameObjectHandle& ptr) {
				ptr->Reset();
			});

			// Awakening would not be called again.
			isAwaken = true;
			isPaused = false;
			isCompleted = false;
		}

		SCENE_CONSTEXPR void MakeAwake() volatile noexcept
		{
			isAwaken = true;
		}

		SCENE_CONSTEXPR bool SetPause(const bool flag) volatile noexcept
		{
			return isPaused = flag;
		}

		SCENE_CONSTEXPR void MakeCompletion() volatile noexcept
		{
			isCompleted = true;
		}

		[[nodiscard]]
		SCENE_CONSTEXPR bool IsAwaken() const volatile noexcept
		{
			return GET_FLAG(isAwaken);
		}

		[[nodiscard]]
		SCENE_CONSTEXPR bool IsPaused() const volatile noexcept
		{
			return GET_FLAG(isPaused);
		}

		[[nodiscard]]
		SCENE_CONSTEXPR bool IsCompleted() const volatile noexcept
		{
			return GET_FLAG(isCompleted);
		}

		Scene(const Scene& other) = delete;
		Scene& operator=(const Scene& other) = delete;

	protected:
		std::vector<GameObjectHandle> gameObjects{};

		volatile FLAG_T isAwaken = false;
		volatile FLAG_T isPaused = false;
		volatile FLAG_T isCompleted = false;
	};
}

export namespace std
{
	inline SCENE_CONSTEXPR
		void swap(game::Scene& lhs, game::Scene& rhs)
		noexcept(util::nothrow_swappables<game::Scene>)
	{
		lhs.Swap(rhs);
	}
}
