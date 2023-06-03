export module Client.Framework;
import Utility;
import Utility.Singleton;
import Utility.Constraints;
import Utility.Monad;
import Utility.Concurrency.Thread;
import Game.Camera;
import Game.GameObject;
import Game.Scene;
import Game.Scene.Specialized;
import Game.Scene.Management;
import <memory>;
import <vector>;

export class Framework : public util::Singleton<Framework>
{
public:
	using Hetero = std::unique_ptr<util::jthread>;

	friend void GameUpdater(Framework& system);
	friend void GameInputs(Framework& system);
	friend void GameRenderer(Framework& system);

	constexpr Framework() noexcept
		: Singleton<Framework>(this)
	{}

	constexpr ~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update();
	void UpdateOnce(game::Scene* const& scene, const float delta_time);
	void Render(game::Scene* const& scene, const void* context) noexcept;

	Hetero myWorkUnit = nullptr, myInputUnit = nullptr, myRenderUnit = nullptr;

	int myWindow;
};
