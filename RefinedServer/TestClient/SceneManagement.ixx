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
	using SceneHandle = std::shared_ptr<SceneBasis>;

	class SceneManager : public util::Singleton<SceneManager>
	{
	public:
			scenesMap[scene->GetName()] = scene;
		static std::vector<SceneHandle> everyScene;
		static std::unordered_map<std::string, SceneHandle> scenesMap;
	};
}
