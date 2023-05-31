#include "pch.hpp"
#include "Framework.hpp"
#include "MainScene.hpp"

import Utility.Print;

using namespace ::game;
using namespace ::std::chrono_literals;

void Framework::Awake()
{
	util::Println("시스템을 초기화합니다.");

	SceneManager::AddScene(SceneManager::CreateScene<MainScene>());
	SceneManager::AddScene(SceneManager::CreateScene("씬 2"));

	SceneManager::SetActiveScene(0);

	for (SceneHandle& scene : SceneManager::everyScene)
	{
		util::debug::Println("장면 '{}'을(를) 초기화하는 중...", scene->GetName());

		scene->Awake();
	}
}

void Framework::Start() noexcept
{
	util::Println("게임을 준비합니다.");

	if (Scene* const& intro = SceneManager::GetActiveScene(); intro != nullptr)
	{
		util::debug::Println("첫번째 씬으로 '{}'에서 게임을 시작합니다.", intro->GetName());
		intro->Start();
	}
	else
	{
		util::Println("첫번째 씬이 없어서 게임을 시작할 수 없습니다.");
	}
}

void Framework::Update()
{
	util::Println("프레임워크의 갱신 루프를 시작합니다.");

	using clock = std::chrono::system_clock;
	using time_point = clock::time_point;
	using duration = clock::duration;
	using period = clock::period;
	constexpr float num = static_cast<float>(period::num);
	constexpr float den = static_cast<float>(period::den);
	constexpr float idv = num / den;

	time_point start_time{};
	time_point current_time{};

	while (true)
	{
		Scene* const& scene = SceneManager::GetActiveScene();
		if (scene == nullptr) UNLIKELY
		{
			util::Println("현재 가리키는 씬이 없습니다.");
			break;
		};

		if (scene->IsPaused()) UNLIKELY
		{
			std::this_thread::yield();
			std::this_thread::sleep_for(10ms);
			continue;
		};

		duration between = current_time - start_time;
		float est = static_cast<float>(between.count()) * idv;

		UpdateOnce(scene, est);
		util::debug::Println("씬 업데이트 1 (시간: {:.5f}초)", est);

		start_time = current_time;
		current_time = std::chrono::system_clock::now();

		between = current_time - start_time;
		est = static_cast<float>(between.count()) * idv;

		LateUpdateOnce(scene, est);
		util::debug::Println("씬 업데이트 2 (시간: {:.5f}초)", est);

		start_time = current_time;
		current_time = std::chrono::system_clock::now();

		if (scene->IsCompleted()) UNLIKELY
		{
			if (!game::SceneManager::IsAvailableNext()) UNLIKELY
			{
				util::Println("다음 씬이 없습니다.");
				break;
			}
			else
			{
				const util::Monad<game::Scene*> scene_wrapper = SceneManager::GotoNextScene();

				if (scene_wrapper.has_value())
				{
					Scene* const& next = *scene_wrapper;

					util::Println("다음 씬 {}을(를) 시작합니다.", next->GetName());
					next->Start();
				}
				else
				{
					util::Println("오류! 씬 {}에 문제가 있습니다.", scene->GetName());
					break;
				}
			};
		};

#if _DEBUG
		std::this_thread::sleep_for(3000ms);
#endif
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
