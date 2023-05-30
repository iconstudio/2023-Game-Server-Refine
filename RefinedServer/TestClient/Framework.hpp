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
			float est = static_cast<float>((current_time - start_time) / 1000);

			UpdateOnce(scene.get(), est);

			start_time = current_time;
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			est = static_cast<float>((current_time - start_time) / 1000);
			LateUpdateOnce(scene.get(), est);

			start_time = current_time;
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			if (scene->IsCompleted())
			{
				roomIndex++;

				if (everyScene.size() <= roomIndex)
				{
					break;
				}
				else
				{
					auto& scene = everyScene[roomIndex];
					scene->Start();
				}
			}
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
