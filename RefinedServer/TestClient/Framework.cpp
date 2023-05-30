#include "pch.hpp"
#include "Framework.hpp"

import Utility.Print;

using namespace game;

void Framework::Awake()
{
	util::Println("시스템을 초기화합니다.");

	SceneManager::AddScene(SceneManager::CreateScene("씬 1"));
	SceneManager::AddScene(SceneManager::CreateScene("씬 2"));

	for (SceneHandle& scene : SceneManager::everyScene)
	{
		scene->Awake();
	}
}

void Framework::Start() noexcept
{
	util::Println("프레임워크를 준비합니다.");

	CurrentScene().if_then([](Scene* scene) {
		scene->Start();
	}).else_then([]() noexcept {
		util::Println("첫번째 씬이 없습니다.");
	});
}

void Framework::Update()
{
	util::Println("프레임워크를 시작합니다.");

	long long start_time = 0;
	long long current_time = 0;

	while (true)
	{
		const auto& scene_ptr = CurrentScene();
		if (!scene_ptr.has_value())
		{
			util::Println("현재 가리키는 씬이 없습니다.");
			break;
		}

		const auto& scene = *scene_ptr;

		long long between = current_time - start_time;
		float est = static_cast<float>(between) / 1000.0f;

		UpdateOnce(scene, est);
		util::debug::Println("씬 업데이트 1 (시간: {:.3f}초)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		between = current_time - start_time;
		est = static_cast<float>(between) / 1000.0f;
		LateUpdateOnce(scene, est);
		util::debug::Println("씬 업데이트 2 (시간: {:.3f}초)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (scene->IsCompleted())
		{
			roomIndex++;

			if (game::SceneManager::everyScene.size() <= roomIndex)
			{
				util::Println("다음 씬이 없습니다.");
				break;
			}
			else
			{
				auto scene_wrapper = SceneManager::GetScene(roomIndex);

				if (scene_wrapper.has_value())
				{
					auto& scene = *scene_wrapper;

					util::Println("다음 씬 {}을(를) 시작합니다.", roomIndex);
					scene->Start();
				}
				else
				{
					util::Println("오류! 다음 씬 {}에 문제가 있습니다.", roomIndex);
					break;
				}
			}
		}
	}
}

void Framework::UpdateOnce(Scene* const& scene, const float delta_time)
{
	scene->Update(delta_time);
}

void Framework::LateUpdateOnce(Scene* const& scene, const float delta_time)
{
	scene->LateUpdate(delta_time);
}
