#pragma once
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;
import System.PipelineObject;
import Game.Camera;

class System : sys::PipelineObject
{
public:
	System() noexcept;
	~System() noexcept;

	void Awake();
	void Start();
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;

	game::Camera myCamera;
};
