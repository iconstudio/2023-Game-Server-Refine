#pragma once
#include <memory>
#include <chrono>
#include <vector>

import Utility;
import Utility.Constraints;
import Utility.Monad;
import Game.Camera;
import Game.GameObject;
import Game.Scene;
import Game.Scene.Specialized;
import Game.Scene.Management;

class Framework
{
public:
	Framework() noexcept
	{}

	~Framework() noexcept = default;

	void Awake();
	void Start() noexcept;
	void Update();

	void UpdateOnce(game::Scene* const& scene, const float delta_time);
	void LateUpdateOnce(game::Scene* const& scene, const float delta_time);

	constexpr util::Monad<game::Scene*> CurrentScene() const noexcept
	{
		if (roomIndex < game::SceneManager::everyScene.size())
		{
			return game::SceneManager::everyScene[roomIndex].get();
		}
		else
		{
			return util::nullopt;
		}
	}

	size_t roomIndex = 0;
};
