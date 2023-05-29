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
	class Scene;

	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using wrapper = Scene<type>;
		using pipeline = sys::PipelineObject<type>;
		using singletone = util::Singleton<type>;
	};

	template<typename S>
	class Scene
		: public SceneBasis
		, public SceneTraits<S>::pipeline
		, public SceneTraits<S>::singletone
	{
	public:
		using scene_type = SceneTraits<S>::type;
		using wrapper_type = SceneTraits<S>::wrapper;
		using pipeline = SceneTraits<S>::pipeline;
		using singletone = SceneTraits<S>::singletone;

		constexpr Scene(S* const& scene) noexcept
			: Scene(scene, "Scene")
		{}

		constexpr Scene(S* const& scene, const std::string_view& name) noexcept
			: SceneBasis(name)
			, pipeline()
			, singletone(scene)
			, gameObjects()
		{
			gameObjects.reserve(10ULL);
		}

		void Destroy() noexcept(noexcept(Cast()->Destroy()))
		{
			Cast()->Destroy();
		}

		inline ~Scene() noexcept(noexcept(Cast()->Destroy()))
		{
			Cast()->Destroy();
			SceneBasis::~SceneBasis();
		}

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		std::vector<std::unique_ptr<GameObject>> gameObjects;

	private:
		[[nodiscard]]
		constexpr S* Cast() noexcept
		{
			return pipeline::Cast();
		}

		[[nodiscard]]
		constexpr const S* Cast() const noexcept
		{
			return pipeline::Cast();
		}
	};
}
