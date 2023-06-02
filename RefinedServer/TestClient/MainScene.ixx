export module Client.MainScene;
import Utility;
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;
import Game.Scene.Specialized;

export class MainScene final : public game::NamedScene<MainScene>
{
public:
	using basis = game::NamedScene<MainScene>;

	MainScene() noexcept
		: basis(this, "Main Scene")
		, mySocket()
		, myAddress()
	{}

	void Awake() override;
	void Start() noexcept override;
	void Update(const float delta_time) override;
	void Render(const void* context) noexcept override;
	void Destroy() override;

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;
};

void MainScene::Awake()
{
	myAddress = net::EndPoint::CreateStaticTCP(net::IPv4, 54000);

	net::Socket::CreateTCP(net::SocketType::SoFlagOverlapped).if_then([this](net::Socket&& socket) {
		mySocket = static_cast<net::Socket&&>(socket);
	});

	Scene::Awake();
}

void MainScene::Start() noexcept
{
	Scene::Start();
}

void MainScene::Update(const float delta_time)
{
	Scene::Update(delta_time);
}

void MainScene::Render(const void* context) noexcept
{
	Scene::Render(context);
}

void MainScene::Destroy()
{
	Scene::Destroy();
}
