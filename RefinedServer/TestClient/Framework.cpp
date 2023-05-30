#include "Framework.hpp"

import Utility.Print;

void Framework::Awake()
{
	util::Println("�ý����� �ʱ�ȭ�մϴ�.");

	game::SceneManager::AddScene(std::make_unique<game::Scene>("�� 1"));
	game::SceneManager::AddScene(std::make_unique<game::Scene>("�� 2"));

	for (std::unique_ptr<game::Scene>& scene : game::SceneManager::everyScene)
	{
		scene->Awake();
	}
}

void Framework::Start() noexcept
{
	util::Println("�����ӿ�ũ�� �غ��մϴ�.");

	CurrentScene().if_then([](game::Scene* scene) {
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
		if (!scene_ptr.has_value())
		{
			util::Println("���� ����Ű�� ���� �����ϴ�.");
			break;
		}

		const auto& scene = *scene_ptr;

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

		if (scene->IsCompleted())
		{
			roomIndex++;

			if (game::SceneManager::everyScene.size() <= roomIndex)
			{
				util::Println("���� ���� �����ϴ�.");
				break;
			}
			else
			{
				auto& scene = game::SceneManager::everyScene[roomIndex];

				util::Println("���� �� {}��(��) �����մϴ�.", roomIndex);
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
