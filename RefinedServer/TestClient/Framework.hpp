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
	{

	}

	void Awake()
	{
		for (auto& scene : everyScene)
		{
			scene->Awake();
		}
	}

	void Start()
	{
		auto& scene = everyScene[roomIndex];

		scene->Start();
	}

	void Update()
	{
		long long start_time = 0;
		long long current_time = 0;

		while (true)
		{
			auto& scene = everyScene[roomIndex];

			UpdateOnce(scene.get(), current_time - start_time);

			start_time = current_time;
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			LateUpdateOnce(scene.get(), current_time - start_time);

			start_time = current_time;
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void UpdateOnce(game::Scene* scene, const float& delta_time)
	{
		scene->Update(delta_time);
	}

	void LateUpdateOnce(game::Scene* scene, const float& delta_time)
	{
		scene->LateUpdate(delta_time);
	}

	std::vector<std::unique_ptr<game::Scene>> everyScene;
	size_t roomIndex = 0;
};
