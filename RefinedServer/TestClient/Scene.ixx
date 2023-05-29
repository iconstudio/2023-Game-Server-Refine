export module Game.Scene;
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

		constexpr Scene(S *const& scene) noexcept
			: SceneBasis()
			, pipeline()
			, singletone(scene)
		{}

		constexpr Scene(S* const& scene, std::string_view name) noexcept
			: SceneBasis(name)
			, pipeline()
			, singletone(scene)
		{}

		~Scene() noexcept
		{
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
			return static_cast<S*>(this);
		}

		[[nodiscard]]
		constexpr const S* Cast() const noexcept
		{
			return static_cast<const S*>(this);
		}
	};
}
