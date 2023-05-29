#include "pch.hpp"
#include "MainScene.hpp"

import Utility;

void MainScene::Awake()
{
	myAddress = net::EndPoint::CreateStaticTCP(net::IPv4, 54000);

	net::Socket::CreateTCP(net::SocketType::SoFlagOverlapped).if_then([this](net::Socket&& socket) {
		mySocket = static_cast<net::Socket&&>(socket);
	});
}

void MainScene::Start() noexcept
{
}

void MainScene::Update(const float& delta_time)
{}

void MainScene::LateUpdate(const float& delta_time)
{}

void MainScene::Destroy()
{}
