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

	void Awake();
	void Start() noexcept;
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);
	void Destroy();

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;
};
