export module Game.Scene.Management;
import <vector>;
import <unordered_map>;
import <string>;
import <string_view>;
import <memory>;
import <algorithm>;
import Utility;
import Utility.Singleton;
import Game.Scene;

using namespace std;

export namespace game
{
	using SceneHandle = std::shared_ptr<SceneBasis>;

	class SceneManager : public util::Singleton<SceneManager>
	{
	public:
		static void AddScene(const SceneHandle& scene) noexcept
		{
			everyScene.push_back(scene);
			auto lt = std::unique(everyScene.begin(), everyScene.end());
			everyScene.erase(lt, everyScene.end());

			scenesMap[scene->GetName()] = scene;
		}

		static std::vector<SceneHandle> everyScene;
		static std::unordered_map<std::string, SceneHandle> scenesMap;
	};
}
