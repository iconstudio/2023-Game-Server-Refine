module Client.Framework;
import Utility.Print;
import Utility.Time;
import Client.IntroScene;
import Client.TitleScene;
import Client.MainScene;
import Client.GameScene;

using namespace ::game;

using dt_clock = std::chrono::steady_clock;
using time_point = dt_clock::time_point;
using duration = dt_clock::duration;
using period = dt_clock::period;
using alter_duration = std::chrono::milliseconds;
constexpr float num = static_cast<float>(period::num);
constexpr float den = static_cast<float>(period::den);
constexpr float idv = num / den;

time_point start_time{};
time_point current_time{};
duration between{};
float est = 0;

alter_duration stacked_time{};

void Framework::Awake()
{
	util::Println("�ý����� �ʱ�ȭ�մϴ�.");

	SceneManager::AddScene(SceneManager::CreateScene<IntroScene>());
	SceneManager::AddScene(SceneManager::CreateScene<MainScene>());
	SceneManager::AddScene(SceneManager::CreateScene("�� 2"));

	SceneManager::AwakeScenes();

	SceneManager::SetActiveScene(0);
}

void Framework::UpdateOnce(Scene* const& scene)
{
	start_time = dt_clock::now();

	scene->Update(est);
	util::debug::Println("�� ������Ʈ (�ð�: {:.5f}��)", est);

	current_time = dt_clock::now();
	between = current_time - start_time;
	est = static_cast<float>(between.count()) * idv;
	stacked_time += std::chrono::duration_cast<alter_duration>(between);

	if (scene->IsCompleted())
	{
		if (!game::SceneManager::IsAvailableNext())
		{
			util::Println("���� ���� �����ϴ�.");
			return;
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
				return;
			}
		}
	}
}
