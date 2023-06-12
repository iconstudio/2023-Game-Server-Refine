export module Game.Scene.Management;
import <vector>;
import <memory>;
import Utility;
import Utility.Constraints;
import Utility.Monad;
import Utility.Print;
import Game.Scene;

export namespace game
{
	using SceneHandle = std::unique_ptr<Scene>;
	using SceneContainer = std::vector<SceneHandle>;

	class SceneManager
	{
	public:
		static inline void AwakeScenes()
		{
			for (SceneHandle& scene : everyScene)
			{
				util::debug::Println("장면 '{}'을(를) 초기화하는 중...", scene->GetName());

				scene->Awake();
			}
		}

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

		static constexpr util::Monad<Scene*> SetActiveScene(const size_t index) noexcept
		{
			return GetScene(index).if_then([&index](Scene* scene) noexcept {
				activeIndex = index;
				activeScene = scene;
			});
		}

		static constexpr util::Monad<Scene*> GotoNextScene() noexcept
		{
			return SetActiveScene(activeIndex + 1ULL);
		}

		static constexpr util::Monad<Scene*> GotoPrevScene() noexcept
		{
			return SetActiveScene(activeIndex - 1ULL);
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

		[[nodiscard]]
		static constexpr const SceneContainer& GetScenes() noexcept
		{
			return everyScene;
		}
		
		[[nodiscard]]
		static constexpr Scene* const& GetActiveScene() noexcept
		{
			return activeScene;
		}

		[[nodiscard]]
		static constexpr util::Monad<Scene*> GetNextScene() noexcept
		{
			return GetScene(activeIndex + 1ULL);
		}

		[[nodiscard]]
		static constexpr util::Monad<Scene*> GetPrevScene() noexcept
		{
			return GetScene(activeIndex - 1ULL);
		}

		[[nodiscard]]
		static constexpr bool IsAvailableNext() noexcept
		{
			return activeIndex < everyScene.size() - 1ULL;
		}

		[[nodiscard]]
		static constexpr bool IsAvailablePrev() noexcept
		{
			return 1ULL <= activeIndex;
		}

		[[nodiscard]]
		static inline constexpr size_t NumberOfScenes() noexcept
		{
			return everyScene.size();
		}

		static inline SceneContainer everyScene{};

	private:
		static inline Scene* activeScene = nullptr;
		static inline size_t activeIndex = 0;
	};
}
