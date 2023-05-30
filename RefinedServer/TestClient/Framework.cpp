#include "Framework.hpp"

import Utility.Print;

void Framework::Awake()
{
	util::Println("�ý����� �ʱ�ȭ�մϴ�.");

	for (std::unique_ptr<game::Scene>& scene : everyScene)
	{
		scene->Awake();
	}
}

void Framework::Start()
{
	util::Println("�����ӿ�ũ�� �غ��մϴ�.");

	std::unique_ptr<game::Scene>& scene = CurrentScene();

	scene->Start();
}

void Framework::Update()
{
	util::Println("�����ӿ�ũ�� �����մϴ�.");

	long long start_time = 0;
	long long current_time = 0;

	while (true)
	{
		auto& scene = CurrentScene();

		long long between = current_time - start_time;
		float est = static_cast<float>(between) / 1000.0f;

		UpdateOnce(scene.get(), est);
		util::debug::Println("�� ������Ʈ 1 (�ð�: {:.3f}��)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		between = current_time - start_time;
		est = static_cast<float>(between) / 1000.0f;
		LateUpdateOnce(scene.get(), est);
		util::debug::Println("�� ������Ʈ 2 (�ð�: {:.3f}��)", est);

		start_time = current_time;
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (scene->IsCompleted())
		{
			roomIndex++;

			if (everyScene.size() <= roomIndex)
			{
				util::Println("���� ���� �����ϴ�.");
				break;
			}
			else
			{
				auto& scene = everyScene[roomIndex];

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
