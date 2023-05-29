export module Game.Scene;
import <memory>;
import <string_view>;
import <vector>;
import <algorithm>;
import Utility.Singleton;
import Utility.Constraints;
import System.PipelineObject;
export import Game.Camera;
export import Game.Scene.Basis;
export import Game.GameObject;

export extern "C++" namespace game
{
	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using pipeline = sys::PipelineObject<type>;
		using singletone = util::Singleton<type>;
	};

	class Scene : public SceneBasis
	{
	public:
		constexpr Scene() noexcept
			: Scene("Scene")
		{}

		constexpr Scene(const std::string_view& name) noexcept
			: SceneBasis(name)
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
}
