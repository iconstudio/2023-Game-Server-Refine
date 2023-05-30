#pragma once
#include <memory>
#include <chrono>
#include <vector>

import Utility;
import Utility.Constraints;
import Game.Camera;
import Game.GameObject;
import Game.Scene;
import Game.Scene.Specialized;

class Framework
{
public:
	Framework() noexcept
		: everyScene()
	{
		everyScene.emplace_back(std::make_unique<game::Scene>());

		//auto camera = game::GameObject::Instantiate<game::Camera>();
	}

	~Framework() noexcept
	{}

	template<typename S>
	void AddScene() noexcept
	{
		everyScene.emplace_back(std::make_unique<S>());
	}

	void Awake();
	void Start();
	void Update();

	void UpdateOnce(game::Scene* scene, const float delta_time);
	void LateUpdateOnce(game::Scene* scene, const float delta_time);

	std::vector<std::unique_ptr<game::Scene>> everyScene;
	size_t roomIndex = 0;
};
