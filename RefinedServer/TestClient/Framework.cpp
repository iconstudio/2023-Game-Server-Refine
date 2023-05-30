#include "Framework.hpp"

import Utility.Print;

void Framework::Awake()
{
	util::Println("시스템을 초기화합니다.");

	for (std::unique_ptr<game::Scene>& scene : everyScene)
	{
		scene->Awake();
	}
}

void Framework::Start()
{
	util::Println("프레임워크를 준비합니다.");

	std::unique_ptr<game::Scene>& scene = CurrentScene();

	scene->Start();
}

void Framework::Update()
{
	util::Println("프레임워크를 시작합니다.");

	long long start_time = 0;
	long long current_time = 0;

	while (true)
	{
		auto& scene = CurrentScene();

		long long between = current_time - start_time;
		float est = static_cast<float>(between) / 1000.0f;

		UpdateOnce(scene.get(), est);
		util::debug::Println("씬 업데이트 1 (시간: {:.3f}초)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		between = current_time - start_time;
		est = static_cast<float>(between) / 1000.0f;
		LateUpdateOnce(scene.get(), est);
		util::debug::Println("씬 업데이트 2 (시간: {:.3f}초)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (scene->IsCompleted())
		{
			roomIndex++;

			if (everyScene.size() <= roomIndex)
			{
				util::Println("다음 씬이 없습니다.");
				break;
			}
			else
			{
				auto& scene = everyScene[roomIndex];

				util::Println("다음 씬 {}을(를) 시작합니다.", roomIndex);
				scene->Start();
			}
		}
	}
}

void Framework::UpdateOnce(game::Scene* const& scene, const float delta_time)
{
	scene->Update(delta_time);
}

void Framework::LateUpdateOnce(game::Scene* const& scene, const float delta_time)
{
	scene->LateUpdate(delta_time);
}
