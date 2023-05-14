#include "pch.hpp"
#include "Framework.hpp"

import Utility.Print;
import Utility.Error;

using namespace net;

Framework::Framework() noexcept
	: Singleton(this)
	, ioPort()
	, acceptContext(Operation::ACCEPT), acceptBuffer(), accceptResultSize(0)
	, nameSocket(), nameEndPoint()
	, gameSocket(), gameEndPoint()
	, everySession(), everyUser()
{}

void Framework::Awake()
{
	util::Println("서버를 초기화하는 중...");

	const int result_startup = Startup();
	if (result_startup != 0)
	{
		util::Println("WSAStartup에 실패했습니다. 오류 코드: {}", std::move(result_startup));

		util::err::RaiseSystemError(result_startup);
	}

	CompletionPort::Establish(concurrentHint).if_then(
		[this](CompletionPort&& port) noexcept {
		ioPort = std::move(port);
	}).else_then([this](int&& error_code) {
		util::Println("IOCP를 생성하는데 실패했습니다. 오류 코드: {}", error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if !_DEBUG
		return;
#endif // !_DEBUG
	});

	Socket::CreateTCP().if_then([this](Socket&& socket) noexcept {
		nameSocket = std::move(socket);
	}).else_then([](int&& error_code) {
		util::Println("서버의 TCP 소켓을 생성하는데 실패했습니다. 오류 코드: {}", error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if !_DEBUG
		return;
#endif // !_DEBUG
	});

	Socket::CreateUDP().if_then([this](Socket&& socket) noexcept {
		gameSocket = std::move(socket);
	}).else_then([](int&& error_code) {
		util::Println("서버의 UDP 소켓을 생성하는데 실패했습니다. 오류 코드: {}", error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if !_DEBUG
		return;
#endif // !_DEBUG
	});

	nameSocket.Listen().if_then([]() noexcept {
		util::Println("서버의 수용을 시작합니다.");
	}).else_then([](int&& error_code) {
		util::Println("서버의 수용 시작 단계가 실패했습니다. 오류 코드: {}", error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if !_DEBUG
		return;
#endif // !_DEBUG
	});

	nameSocket.Accept(gameSocket, acceptBuffer, acceptContext, accceptResultSize).else_then(
		[](int&& error_code) {
		util::Println("서버의 첫번째 비동기 수용 단계가 실패했습니다. 오류 코드: {}", error_code);
		util::err::RaiseSystemError(std::move(error_code));

		return;
	});
}

void Framework::Start() noexcept
{
	util::Println("서버를 시작합니다...");
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
