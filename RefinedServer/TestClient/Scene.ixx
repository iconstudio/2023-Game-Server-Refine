export module Game.Scene;
import <memory>;
import <string>;
import <string_view>;
import <vector>;
import <algorithm>;
import <ranges>;
import Utility.Named;
import Utility.Indexer;
import Utility.Constraints;
import System.PipelineObject;
export import Game.Camera;
export import Game.GameObject;

export extern "C++" namespace game
{
	class Scene
		: public sys::PipelineModel
		, public util::Named
		, public util::Indexer<Scene>
	{
	public:
		constexpr Scene() noexcept
			: Scene("Scene")
		{}

		constexpr Scene(std::string_view name) noexcept
			: Named(name), Indexer<Scene>()
			, gameObjects()
		{
			gameObjects.reserve(10ULL);
		}

		virtual constexpr ~Scene() noexcept = default;

		constexpr void AddGameObject(std::unique_ptr<GameObject>&& gameObject) noexcept
		{
			gameObjects.emplace_back(std::move(gameObject));
		}

		constexpr void RemoveGameObject(const GameObject& gameObject) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&gameObject](const std::unique_ptr<GameObject>& ptr) {
				return ptr.get() == &gameObject;
			});
		}

		constexpr void RemoveGameObject(std::string_view name) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&name](const std::unique_ptr<GameObject>& ptr) {
				return ptr->GetName() == name;
			});
		}

		/// <summary>
		/// Resetting the status to the moment of awakening.
		/// </summary>
		virtual constexpr void Reset() noexcept
		{
			if (!isAwaken)
			{
				Awake();
				isAwaken = true;

				return;
			}

			std::ranges::for_each(gameObjects
				, [](const std::unique_ptr<GameObject>& ptr) {
				ptr->Reset();
			});

			// Awakening would not be called again.
			isAwaken = true;
			isPaused = false;
			isComplete = false;
		}

		constexpr void MakeAwake() noexcept
		{
			isAwaken = true;
		}

		constexpr bool SetPause(const bool flag) noexcept
		{
			return isPaused = flag;
		}

		constexpr bool SetCompletion(const bool done) noexcept
		{
			return isComplete = done;
		}

		[[nodiscard]]
		constexpr bool IsAwaken() const noexcept
		{
			return isAwaken;
		}

		[[nodiscard]]
		constexpr bool IsPaused() const noexcept
		{
			return isPaused;
		}

		[[nodiscard]]
		constexpr bool IsComplete() const noexcept
		{
			return isComplete;
		}

		Scene(const Scene& other) = delete;
		constexpr Scene(Scene&& other) noexcept = default;
		Scene& operator=(const Scene& other) = delete;
		constexpr Scene& operator=(Scene&& other) noexcept = default;

	protected:
		std::vector<std::unique_ptr<GameObject>> gameObjects;

		bool isAwaken = false;
		bool isPaused = false;
		bool isComplete = false;
	};

	using SceneHandle = std::shared_ptr<Scene>;
}
