#pragma once
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;
import Game.Scene;

class MainScene final : public game::Scene
{
public:
	MainScene() noexcept
		: Scene(this)
		, mySocket()
		, myAddress()
	{}

	void Awake();
	void Start();
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);
	void Destroy();

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;
};
