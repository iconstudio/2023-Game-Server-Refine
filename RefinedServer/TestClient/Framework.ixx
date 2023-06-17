export module Client.Framework;
import <vector>;
import Utility.Constraints;
import Utility.Singleton;
import Utility.Memory;
import Utility.Print;
import Utility.Concurrency.Thread;
import Game.Scene;
import Game.Scene.Management;

export class Framework;

export template <typename T>
concept game_updaters = util::invocable<T, Framework&>;

export template <typename T>
concept game_renderers = util::invocable<T, Framework&>;

export template <typename T>
concept game_inputs = util::invocable<T, Framework&>;

export class Framework : public util::Singleton<Framework>
{
public:
	using Hetero = util::unique_ptr<util::jthread>;

	constexpr Framework() noexcept
		: Singleton<Framework>(this)
		, myWorkers()
	{
		myWorkers.reserve(3);
	}

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

	constexpr void AddWorker(Hetero&& worker) noexcept
	{
		myWorkers.emplace_back(static_cast<Hetero&&>(worker));
	}

	void SetWindowID(const int& id) noexcept
	{
		myWindow = id;
	}

	[[nodiscard]]
	const int& GetWindowID() const& noexcept
	{
		return myWindow;
	}

	[[nodiscard]]
	int&& GetWindowID() && noexcept
	{
		return static_cast<int&&>(myWindow);
	}

private:
	std::vector<Hetero> myWorkers;
	int myWindow;
};
