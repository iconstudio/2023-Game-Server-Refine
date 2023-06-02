#include "pch.hpp"
#include "Framework.hpp"

import <chrono>;
import Utility.Print;
import Client.IntroScene;
import Client.TitleScene;
import Client.MainScene;
import Client.GameScene;

using namespace ::game;
using namespace ::std::chrono_literals;

void Framework::Awake()
{
	util::Println("�ý����� �ʱ�ȭ�մϴ�.");

	SceneManager::AddScene(SceneManager::CreateScene<IntroScene>());
	SceneManager::AddScene(SceneManager::CreateScene<MainScene>());
	SceneManager::AddScene(SceneManager::CreateScene("�� 2"));

	SceneManager::AwakeScenes();

	SceneManager::SetActiveScene(0);
}

void Framework::Start() noexcept
{
	util::Println("������ �غ��մϴ�.");

	if (Scene* const& intro = SceneManager::GetActiveScene(); intro != nullptr)
	{
		util::debug::Println("ù��° ������ '{}'���� ������ �����մϴ�.", *intro);
		intro->Start();
	}
	else
	{
		util::Println("ù��° ���� ��� ������ ������ �� �����ϴ�.");
	}
}

void Framework::Update()
{
	util::Println("�����ӿ�ũ�� ���� ������ �����մϴ�.");

	using clock = std::chrono::steady_clock;
	using time_point = clock::time_point;
	using duration = clock::duration;
	using period = clock::period;
	using alter_duration = std::chrono::milliseconds;
	constexpr float num = static_cast<float>(period::num);
	constexpr float den = static_cast<float>(period::den);
	constexpr float idv = num / den;

	time_point start_time{};
	time_point current_time{};
	duration between{};
	float est = 0;

	alter_duration stacked_time{};

	while (true)
	{
		Scene* const& scene = SceneManager::GetActiveScene();
		if (scene == nullptr) UNLIKELY
		{
			util::Println("���� ����Ű�� ���� �����ϴ�.");
			break;
		};

		if (scene->IsPaused()) UNLIKELY
		{
			std::this_thread::yield();
			std::this_thread::sleep_for(10ms);
			continue;
		};

		start_time = clock::now();

		UpdateOnce(scene, est);
		util::debug::Println("�� ������Ʈ (�ð�: {:.5f}��)", est);

		current_time = clock::now();
		between = current_time - start_time;
		est = static_cast<float>(between.count()) * idv;
		stacked_time += std::chrono::duration_cast<alter_duration>(between);

		if (scene->IsCompleted()) UNLIKELY
		{
			if (!game::SceneManager::IsAvailableNext()) UNLIKELY
			{
				util::Println("���� ���� �����ϴ�.");
				break;
			}
			else
			{
				const util::Monad<game::Scene*> scene_wrapper = SceneManager::GotoNextScene();

				if (scene_wrapper.has_value())
				{
					Scene* const& next = *scene_wrapper;

					util::Println("���� �� {}��(��) �����մϴ�.", *next);
					next->Start();
				}
				else
				{
					util::Println("����! �� {}�� ������ �ֽ��ϴ�.", *scene);
					break;
				}
			};
		};

#if _DEBUG
		//std::this_thread::sleep_for(3000ms);
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
