#include "pch.hpp"
#include "Framework.hpp"
#include "Worker.hpp"

import Utility.String;
import Utility.Print;
import Utility.Error;

using namespace ::net;
using namespace ::core;

Framework::Framework() noexcept
	: Singleton(this)
	, ioPort()
	, nameSocket(), nameEndPoint()
	, gameSocket(), gameEndPoint()
	, userManager(nameSocket)
	, workerManager()
{}

class OnError
{
public:
	constexpr OnError(util::string_view msg) noexcept
		: myMsg(msg)
	{}

	inline void operator()(int&& error_code)
	{
		util::Println("{} (���� �ڵ�: {})", myMsg, error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if _DEBUG
		//std::unreachable();
#endif // !_DEBUG
	}

	util::string_view myMsg;
};

void Framework::Awake()
{
	util::Println("������ �ʱ�ȭ�ϴ� ��...");

	int result_startup = Startup();
	if (result_startup != 0)
	{
		OnError("WSAStartup�� �����߽��ϴ�.")(std::move(result_startup));

		util::unreachable();
	}

	CompletionPort::Establish(concurrentHint).if_then(
		[this](CompletionPort&& port) noexcept {
		ioPort = std::move(port);
	}).else_then(OnError("IOCP�� �����ϴµ� �����߽��ϴ�."));

	Socket::CreateTCP().if_then([this](Socket&& socket) noexcept {
		nameSocket = std::move(socket);
	}).else_then(OnError("������ TCP ������ �����ϴµ� �����߽��ϴ�."));

	nameEndPoint = EndPoint::CreateStaticTCP(AddressFamily::IPv4, tcpPort);
	nameSocket.Bind(nameEndPoint)
		.else_then(OnError("������ �ּҸ� �����ϴµ� �����߽��ϴ�."));

	ioPort.Link(nameSocket, serverTcpID)
		.else_then(OnError("������ TCP ������ IOCP�� �����ϴµ� �����߽��ϴ�."));

	Socket::CreateUDP().if_then([this](Socket&& socket) noexcept {
		gameSocket = std::move(socket);
	}).else_then(OnError("������ UDP ������ �����ϴµ� �����߽��ϴ�."));

	nameSocket.optDebug = true;
	nameSocket.optReuseAddress = true;
	nameSocket.optNoDelay = true;
	nameSocket.optUseLoopback = true;

	gameEndPoint = EndPoint::CreateStaticUDP(AddressFamily::IPv4, udpPort);
	gameSocket.Bind(gameEndPoint).else_then(OnError("������ UDP �ּҸ� �����ϴµ� �����߽��ϴ�."));

	ioPort.Link(gameSocket, serverUdpID)
		.else_then(OnError("������ UDP ������ IOCP�� �����ϴµ� �����߽��ϴ�."));

	gameSocket.optDebug = true;
	gameSocket.optReuseAddress = true;
	gameSocket.optUseLoopback = true;
	gameSocket.optBroadcast = true;
	gameSocket.optDontRoute = true;

	userManager.Awake(ioPort);
}

void Framework::Start() noexcept
{
	util::Println("������ �����մϴ�...");

	nameSocket.Listen().if_then([]() noexcept {
		util::Println("TCP ���Ͽ��� ������ ����մϴ�.");
	}).else_then(OnError("���� ���� �ܰ谡 �����߽��ϴ�."));

	userManager.Start();

	for (size_t i = 0; i < concurrentHint; i++)
	{
		workerManager.Push(::Worker, util::ref(ioPort));
	}

	BeginAccept(userid_t::begin).else_then(OnError("ù��° ���� �ܰ谡 �����߽��ϴ�."));
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

ioError Framework::BeginAccept(const userid_t& start)
{
	return userManager.BeginAccept(start);
}

void Framework::EndAccept(core::User* const& newbie) noexcept
{
	return userManager.EndAccept(newbie);
}

void Framework::EndFailedAccept(core::User* const& newbie) noexcept
{
	return userManager.EndFailedAccept(newbie);
}

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
