export module Game.Scene.Specialized;
import <string>;
import <memory>;
import Utility.Singleton;
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
	class NamedScene
		: public Scene
		, public SceneTraits<S>::singletone
	{
	public:
		using singleton = SceneTraits<S>::singletone;

		constexpr NamedScene(S* const& scene, std::string_view name) noexcept
			: Scene(name)
			, singleton(scene)
		{}

		constexpr ~NamedScene() noexcept = default;

		NamedScene(const NamedScene& other) = delete;
		constexpr NamedScene(NamedScene&& other) noexcept = default;
		NamedScene& operator=(const NamedScene& other) = delete;
		constexpr NamedScene& operator=(NamedScene&& other) noexcept = default;
	};
}