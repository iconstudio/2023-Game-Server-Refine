#pragma once
#include <memory>
#include <chrono>
#include <vector>

import Utility.Print;
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
		util::Println("�ý����� �ʱ�ȭ�մϴ�.");

		for (auto& scene : everyScene)
		{
			scene->Awake();
		}
	}

	void Start()
	{
		util::Println("�����ӿ�ũ�� �غ��մϴ�.");

		auto& scene = everyScene[roomIndex];

		scene->Start();
	}

	void Update()
	{
		util::Println("�����ӿ�ũ�� �����մϴ�.");

		long long start_time = 0;
		long long current_time = 0;

		while (true)
		{
			auto& scene = everyScene[roomIndex];

			long long between = current_time - start_time;
			float est = static_cast<float>(between) / 1000.0f;

			UpdateOnce(scene.get(), est);
			util::debug::Println("�� ������Ʈ 1 (�ð�: {}�и���)", est);

			start_time = current_time;
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			between = current_time - start_time;
			est = static_cast<float>(between) / 1000.0f;
			LateUpdateOnce(scene.get(), est);
			util::debug::Println("�� ������Ʈ 2 (�ð�: {}�и���)", est);

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
