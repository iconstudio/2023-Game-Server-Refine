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
		: public util::Named
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

		virtual void Awake()
		{}

		virtual void Start() noexcept
		{}

		virtual void Update(const float& delta_time)
		{}

		virtual void LateUpdate(const float& delta_time)
		{}

		virtual void Destroy()
		{}

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		virtual ~Scene() noexcept = default;

	protected:
		std::vector<std::unique_ptr<GameObject>> gameObjects;
	};

	using SceneHandle = std::shared_ptr<Scene>;
}
