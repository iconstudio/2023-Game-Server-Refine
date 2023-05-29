export module Game.Scene;
import <memory>;
import <string>;
import <string_view>;
import <vector>;
import <algorithm>;
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
