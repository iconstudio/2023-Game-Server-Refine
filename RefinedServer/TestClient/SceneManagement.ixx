module;
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <memory>

export module Game.Scene.Management;
import Utility.Singleton;
import Game.Scene;

using namespace std;

export namespace game
{
	template<typename S>
	using SceneHandle = std::shared_ptr<Scene<S>>;

	class SceneManager : public util::Singleton<SceneManager>
	{
	public:
		static std::vector<SceneHandle> everyScene;
		static std::unordered_map<std::string, SceneHandle> scenesMap;
	};
}
