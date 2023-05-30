#pragma once
#include <memory>
#include <chrono>
#include <vector>

import Game.Scene;
import Game.Camera;

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

	void Awake();
	void Start();
	void Update();

	void UpdateOnce(game::Scene* scene, const float delta_time);
	void LateUpdateOnce(game::Scene* scene, const float delta_time);

	std::vector<std::unique_ptr<game::Scene>> everyScene;
	size_t roomIndex = 0;
};
