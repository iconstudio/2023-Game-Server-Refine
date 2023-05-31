#pragma once
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;
import Game.Scene.Specialized;

class MainScene final : public game::NamedScene<MainScene>
{
public:
	MainScene() noexcept
		: NamedScene<MainScene>(this, "Main Scene")
		, mySocket()
		, myAddress()
	{}

	void Awake() override;
	void Start() noexcept override;
	void Update(const float delta_time) override;
	void LateUpdate(const float delta_time) override;
	void Destroy() override;

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;
};
