module;
#include "pch.hpp"

module Client.Framework;
import Utility.Print;
import Utility.Time;
import Client.IntroScene;
import Client.TitleScene;
import Client.MainScene;
import Client.GameScene;

using namespace ::game;

void Framework::Awake()
{
	util::Println("시스템을 초기화합니다.");

	SceneManager::AddScene(SceneManager::CreateScene<IntroScene>());
	SceneManager::AddScene(SceneManager::CreateScene<MainScene>());
	SceneManager::AddScene(SceneManager::CreateScene("씬 2"));

	SceneManager::AwakeScenes();

	SceneManager::SetActiveScene(0);
}

void Framework::UpdateOnce(Scene* const& scene)
{
	//auto start_time = std::chrono::high_resolution_clock::now();
	/*
	start_time = clock::now();

	scene->Update(est);
	util::debug::Println("씬 업데이트 (시간: {:.5f}초)", est);

	current_time = clock::now();
	between = current_time - start_time;
	est = static_cast<float>(between.count()) * idv;
	stacked_time += std::chrono::duration_cast<alter_duration>(between);

	if (scene->IsCompleted())
	{
		if (!game::SceneManager::IsAvailableNext())
		{
			util::Println("다음 씬이 없습니다.");
			break;
		}
		else
		{
			const util::Monad<game::Scene*> scene_wrapper = game::SceneManager::GotoNextScene();

			if (scene_wrapper.has_value())
			{
				game::Scene* const& next = *scene_wrapper;

				util::Println("다음 씬 {}을(를) 시작합니다.", *next);
				next->Start();
			}
			else
			{
				util::Println("오류! 씬 {}에 문제가 있습니다.", *scene);
				break;
			}
		}
	}*/
}
