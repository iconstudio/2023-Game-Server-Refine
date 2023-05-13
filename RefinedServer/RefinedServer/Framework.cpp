#include "pch.hpp"
#include "Framework.hpp"

import Utility.Print;
import Utility.Error;

using namespace net;

Framework::Framework() noexcept
	: Singleton(this)
	, ioPort()
	, nameSocket(), nameEndPoint()
	, everySession(), everyUser()
{}

void Framework::Awake()
{
	util::Println("������ �ʱ�ȭ�ϴ� ��...");

	const int result_startup = Startup();
	if (result_startup != 0)
	{
		util::Println("WSAStartup�� �����߽��ϴ�. ���� �ڵ�: {}", std::move(result_startup));

		util::err::RaiseSystemError(result_startup);
	}

	CompletionPort::Establish(concurrentHint).if_then(
		[this](CompletionPort&& port) noexcept {
		ioPort = std::move(port);
	}).else_then([this](int&& error_code) {
		util::Println("IOCP�� �����ϴµ� �����߽��ϴ�. ���� �ڵ�: {}", error_code);

		util::err::RaiseSystemError(std::move(error_code));
	});

	nameSocket = Socket::CreateTCP();
}

void Framework::Start() noexcept
{
	util::Println("������ �����մϴ�...");
}

void Framework::Update() noexcept
{
	while (true)
	{
		UpdateOnce(0);
	}
}

void Framework::UpdateOnce(const float& delta_time) noexcept
{
	if (delta_time <= 0)
	{
		return;
	}
}

Proxy Framework::BeginAccept()
{
	return Proxy{};
}

void Framework::EndAccept() noexcept
{}

void Framework::EndFailedAccept() noexcept
{}

Proxy Framework::BeginWelcome()
{
	return Proxy{};
}

void Framework::EndWelcome() noexcept
{}

void Framework::EndFailedWelcome() noexcept
{}

Proxy Framework::BeginDispose()
{
	return Proxy{};
}

void Framework::EndDispose() noexcept
{}

void Framework::EndFailedDispose() noexcept
{}
