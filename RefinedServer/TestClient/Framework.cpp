#include "pch.hpp"
#include "Framework.hpp"
#include "MainScene.hpp"

import Utility.Print;

using namespace ::game;
using namespace ::std::chrono_literals;

void Framework::Awake()
{
	util::Println("�ý����� �ʱ�ȭ�մϴ�.");

	SceneManager::AddScene(SceneManager::CreateScene<MainScene>());
	SceneManager::AddScene(SceneManager::CreateScene("�� 2"));

	for (SceneHandle& scene : SceneManager::everyScene)
	{
		scene->Awake();
	}
}

void Framework::Start() noexcept
{
	util::Println("�����ӿ�ũ�� �غ��մϴ�.");

	CurrentScene().if_then([](Scene* scene) {
		scene->Start();
	}).else_then([]() noexcept {
		util::Println("ù��° ���� �����ϴ�.");
	});
}

void Framework::Update()
{
	util::Println("�����ӿ�ũ�� �����մϴ�.");

	long long start_time = 0;
	long long current_time = 0;

	while (true)
	{
		const auto& scene_ptr = CurrentScene();
		if (!scene_ptr.has_value()) UNLIKELY
		{
			util::Println("���� ����Ű�� ���� �����ϴ�.");
			break;
		};

		Scene* const& scene = *scene_ptr;

		if (scene->IsPaused()) UNLIKELY
		{
			std::this_thread::yield();
			std::this_thread::sleep_for(10ms);
			continue;
		};

		long long between = current_time - start_time;
		float est = static_cast<float>(between) / 1000.0f;

		UpdateOnce(scene, est);
		util::debug::Println("�� ������Ʈ 1 (�ð�: {:.3f}��)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		between = current_time - start_time;
		est = static_cast<float>(between) / 1000.0f;
		LateUpdateOnce(scene, est);
		util::debug::Println("�� ������Ʈ 2 (�ð�: {:.3f}��)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (scene->IsCompleted()) UNLIKELY
		{
			roomIndex++;

			if (game::SceneManager::NumberOfScenes() <= roomIndex) UNLIKELY
			{
				util::Println("���� ���� �����ϴ�.");
				break;
			}
			else
			{
				const util::Monad<game::Scene*> scene_wrapper = SceneManager::GetScene(roomIndex);

				if (scene_wrapper.has_value())
				{
					Scene* const& next = *scene_wrapper;

					util::Println("���� �� {}��(��) �����մϴ�.", roomIndex);
					next->Start();
				}
				else
				{
					util::Println("����! ���� �� {}�� ������ �ֽ��ϴ�.", roomIndex);
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
