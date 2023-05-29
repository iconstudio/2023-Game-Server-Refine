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
	class NamedScene final
		: public Scene
		, public SceneTraits<S>::singletone
	{
	public:
		using singleton = SceneTraits<S>::singletone;

		constexpr NamedScene(std::string_view name) noexcept
			: Scene(name)
			, singleton(this)
		{}

		constexpr ~NamedScene() noexcept = default;
	};
}
