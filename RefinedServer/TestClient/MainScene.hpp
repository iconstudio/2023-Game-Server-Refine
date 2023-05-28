#pragma once
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;
import System.PipelineObject;
import Game.Camera;
import Game.Scene;

class MainScene final
	: public game::Scene<MainScene>
{
public:
	MainScene() noexcept
		: Scene(this)
		, mySocket()
		, myAddress(), myCamera()
	{}

	~MainScene() noexcept
	{}

	void Awake();
	void Start();
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;

	game::Camera myCamera;
};
