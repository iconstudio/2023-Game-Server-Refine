#include "pch.hpp"
#include "System.hpp"

import Utility;

System::System() noexcept
	: mySocket()
	, myAddress(), myCamera()
{}

System::~System() noexcept
{}

void System::Awake()
{
	myAddress = net::EndPoint::CreateStaticTCP(net::IPv4, 54000);

	net::Socket::CreateTCP(net::SocketType::SoFlagOverlapped).if_then([this](net::Socket&& socket) {
		mySocket = static_cast<net::Socket&&>(socket);
	});
}

void System::Start()
{

}

void System::Update(const float& delta_time)
{}

void System::LateUpdate(const float& delta_time)
{}
