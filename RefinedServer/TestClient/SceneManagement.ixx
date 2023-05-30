export module Game.Scene.Management;
import <vector>;
import <memory>;
import Utility;
import Utility.Constraints;
import Utility.Monad;
import Game.Scene;

export namespace game
{
	using SceneHandle = std::unique_ptr<Scene>;

	class SceneManager
	{
	public:
		static constexpr void AddScene(SceneHandle&& scene) noexcept
		{
			everyScene.push_back(static_cast<SceneHandle&&>(scene));
		}

		template<typename S>
		static constexpr void AddScene(S* const& scene) noexcept
		{
			static_assert(util::hierachy<S, Scene>, "S must be derived from Scene");

			AddScene(SceneHandle(scene));
		}

		template<typename S = Scene, typename... Args>
		[[nodiscard]]
		static constexpr SceneHandle CreateScene(Args&&... args)
			noexcept(util::nothrow_constructibles<S, Args...>)
		{
			static_assert(util::hierachy<S, Scene>, "S must be derived from Scene");

			return SceneHandle(new S(util::forward<Args>(args)...));
		}

		[[nodiscard]]
		static constexpr util::Monad<Scene*> GetScene(const size_t index) noexcept
		{
			if (index < everyScene.size())
			{
				return everyScene[index].get();
			}
			else
			{
				return util::nullopt;
			}
		}

		static std::vector<SceneHandle> everyScene;
	};

	std::vector<SceneHandle> SceneManager::everyScene{};
}
