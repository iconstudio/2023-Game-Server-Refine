export module Game.Scene.Specialized;
import <string>;
import <memory>;
import Utility.Singleton;

export namespace game
{
	template<typename S>
	struct SceneTraits
	{
		using type = S;
		using singletone = util::Singleton<type>;
	};

}
