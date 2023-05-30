export module Game.Scene;
import <atomic>;
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
	class [[nodiscard]] alignas(64) Scene
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
		{
			gameObjects.reserve(10ULL);
		}

		virtual constexpr ~Scene() noexcept = default;

		constexpr void AddInstance(std::unique_ptr<GameObject>&& gameObject) noexcept
		{
			gameObjects.emplace_back(std::move(gameObject));
		}

		constexpr void RemoveInstance(const GameObject& gameObject) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&gameObject](const std::unique_ptr<GameObject>& ptr) {
				return ptr.get() == std::addressof(gameObject);
			});
		}

		constexpr void RemoveInstance(std::string_view name) noexcept
		{
			(void)std::ranges::remove_if(gameObjects
				, [&name](const std::unique_ptr<GameObject>& ptr) {
				return ptr->GetName() == name;
			});
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
				, [](const std::unique_ptr<GameObject>& ptr) {
				ptr->Reset();
			});

			// Awakening would not be called again.
			isAwaken = true;
			isPaused = false;
			isCompleted = false;
		}

		inline void MakeAwake() noexcept
		{
			isAwaken = true;
		}

		inline bool SetPause(const bool flag) noexcept
		{
			return isPaused = flag;
		}

		inline bool SetCompletion(const bool done) noexcept
		{
			return isCompleted = done;
		}

		[[nodiscard]]
		inline bool IsAwaken() const noexcept
		{
			return isAwaken;
		}

		[[nodiscard]]
		inline bool IsPaused() const noexcept
		{
			return isPaused;
		}

		[[nodiscard]]
		inline bool IsCompleted() const noexcept
		{
			return isCompleted;
		}

		Scene(const Scene& other) = delete;
		constexpr Scene(Scene&& other) noexcept = default;
		Scene& operator=(const Scene& other) = delete;
		constexpr Scene& operator=(Scene&& other) noexcept = default;

	protected:
		std::vector<std::unique_ptr<GameObject>> gameObjects{};

		std::atomic_bool isAwaken = false;
		std::atomic_bool isPaused = false;
		std::atomic_bool isCompleted = false;
	};

	using SceneHandle = std::shared_ptr<Scene>;
}
