export module Game.Scene;
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
		using singletone = util::Singleton<type>;
	};

	template<typename S>
	class Scene
		: public SceneBasis
		, public SceneTraits<S>::singletone
	{
	public:
		using scene_type = SceneTraits<S>::type;
		using wrapper_type = SceneTraits<S>::wrapper;
		using singletone = SceneTraits<S>::singletone;

		constexpr Scene(S *const& scene) noexcept
			: SceneBasis()
			, singletone(scene)
		{}

		constexpr Scene(S* const& scene, std::string_view name) noexcept
			: SceneBasis(name)
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
	};
}
