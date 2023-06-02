#pragma once
import <memory>;
import <vector>;

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

class Framework;
using Hetero = std::unique_ptr<util::jthread>;
void GameUpdater(Framework& system);
void GameInputs(Framework& system);
void GameRenderer(Framework& system);

class Framework : public util::Singleton<Framework>
{
public:
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
};
