export module Client.Framework;
import Utility.Singleton;
import Utility.Memory;
import Utility.Print;
import Utility.Concurrency.Thread;
import Game.Scene;
import Game.Scene.Management;

export class Framework : public util::Singleton<Framework>
{
public:
	using Hetero = util::unique_ptr<util::jthread>;

	friend void GameUpdater(Framework& system);
	friend void GameInputs(Framework& system);
	friend void GameRenderer(Framework& system);

	constexpr Framework() noexcept
		: Singleton<Framework>(this)
	{}

	constexpr ~Framework() noexcept = default;

	void Awake();

	void Start() noexcept
	{
		util::Println("������ �غ��մϴ�.");

		if (game::Scene* const& intro = game::SceneManager::GetActiveScene(); intro != nullptr)
		{
			util::debug::Println("ù��° ������ '{}'���� ������ �����մϴ�.", *intro);
			intro->Start();
		}
		else
		{
			util::Println("ù��° ���� ��� ������ ������ �� �����ϴ�.");
		}
	}

	void Update()
	{
		util::Println("�����ӿ�ũ�� ���� ������ �����մϴ�.");

		while (true)
		{
			game::Scene* const& scene = game::SceneManager::GetActiveScene();
			if (scene == nullptr)
			{
				util::Println("���� ����Ű�� ���� �����ϴ�.");
				break;
			};

			if (scene->IsPaused())
			{
				util::this_thread::yield();
				continue;
			};

			UpdateOnce(scene);
		}
	}

	void UpdateOnce(game::Scene* const& scene);

	void Render(game::Scene* const& scene, const void* context) noexcept
	{
		scene->Render(context);
	}

	Hetero myWorkUnit = nullptr, myInputUnit = nullptr, myRenderUnit = nullptr;

	int myWindow;
};
