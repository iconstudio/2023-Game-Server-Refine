export module Game.Scene;
import <memory>;
import <string_view>;
import System.PipelineObject;
export import Game.Camera;
export import Game.Scene.Basis;
import Utility.Singleton;

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
			: Scene(scene, "Scene", nullptr)
		{}

		constexpr Scene(S* const& scene, const std::string_view& name) noexcept
			: Scene(scene, name, nullptr)
		{}

		constexpr Scene(S* const& scene, const std::shared_ptr<Camera>& camera) noexcept
			: Scene(scene, "Scene", camera)
		{}

		constexpr Scene(S* const& scene, const std::string_view& name, const std::shared_ptr<Camera>& camera) noexcept
			: SceneBasis(name)
			, pipeline()
			, singletone(scene)
			, mainCamera(camera)
		{}

		void Destroy() noexcept
		{
			Cast()->Destroy();
		}

		inline ~Scene() noexcept
		{
			Destroy();
			SceneBasis::~SceneBasis();
		}

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<Camera> mainCamera;

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
