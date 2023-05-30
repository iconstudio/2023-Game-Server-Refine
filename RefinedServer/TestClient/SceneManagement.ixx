export module Game.Scene.Management;
import <vector>;
import <unordered_map>;
import <string>;
import <string_view>;
import <memory>;
import <algorithm>;
import Game.Scene;

using namespace std;

export namespace game
{
	class SceneManager
	{
	public:
		using SceneHandle = std::unique_ptr<Scene>;

		static void AddScene(SceneHandle&& scene) noexcept
		{
			//everyScene.push_back(scene);
			auto lt = std::unique(everyScene.begin(), everyScene.end());
			//everyScene.erase(lt, everyScene.end());

			scenesMap[scene->GetName()] = std::move(scene);
		}

		static std::vector<SceneHandle> everyScene;
		static std::unordered_map<std::string, SceneHandle> scenesMap;
	};
}
