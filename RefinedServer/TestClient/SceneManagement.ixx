export module Game.Scene.Management;
import <vector>;
import <memory>;
import <algorithm>;
import Utility;
import Utility.Constraints;
import Game.Scene;

using namespace std;

export namespace game
{
	using SceneHandle = std::unique_ptr<Scene>;

	class SceneManager
	{
	public:
		static constexpr void AddScene(SceneHandle&& scene) noexcept
		{
			everyScene.push_back(std::move(scene));

			auto lt = std::unique(everyScene.begin(), everyScene.end());

			if (everyScene.end() != lt)
			{
				everyScene.erase(lt, everyScene.end());
			}
		}

		template<typename S>
		static constexpr void AddScene(S* const& scene) noexcept
		{
			static_assert(util::hierachy<S, Scene>, "S must be derived from Scene");

			AddScene(SceneHandle(scene));
		}

		template<typename S, typename... Args>
		static constexpr SceneHandle CreateScene(Args&&... args)
			noexcept(util::nothrow_constructibles<S, Args...>)
		{
			static_assert(util::hierachy<S, Scene>, "S must be derived from Scene");

			return SceneHandle(new S(util::forward<Args>(args)...));
		}

		static std::vector<SceneHandle> everyScene;
	};
}
