export module Game.Scene.Specialized;
import <string>;
import <memory>;
import Utility.Singleton;
import Utility.Classifier;
import Game.Scene;

export namespace game
{
	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using singletone = util::Singleton<type>;
	};

	template<typename S>
	class [[nodiscard]] alignas(64) NamedScene
		: public Scene
		, public SceneTraits<S>::singletone
		, public util::Classifier<S>
	{
	public:
		using singleton = SceneTraits<S>::singletone;

		constexpr NamedScene(S* const& scene, std::string_view name) noexcept
			: Scene(name)
			, singleton(scene)
			, Classifier<S>()
		{}

		constexpr ~NamedScene() noexcept = default;

		NamedScene(const NamedScene& other) = delete;
		constexpr NamedScene(NamedScene&& other) noexcept = default;
		NamedScene& operator=(const NamedScene& other) = delete;
		constexpr NamedScene& operator=(NamedScene&& other) noexcept = default;
	};
}

export template<typename S>
struct util::class_id<game::NamedScene<S>>
{
	static constexpr size_t id = 4;
};
