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
	, userManager()
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
	nameSocket.Bind(nameEndPoint).else_then(OnError("������ �ּҸ� �����ϴµ� �����߽��ϴ�."));

	Socket::CreateUDP().if_then([this](Socket&& socket) noexcept {
		gameSocket = std::move(socket);
	}).else_then(OnError("������ UDP ������ �����ϴµ� �����߽��ϴ�."));

	nameSocket.optDebug = true;
	nameSocket.optReuseAddress = true;
	nameSocket.optNoDelay = true;
	nameSocket.optUseLoopback = true;

	gameEndPoint = EndPoint::CreateStaticUDP(AddressFamily::IPv4, udpPort);
	gameSocket.Bind(gameEndPoint).else_then(OnError("������ UDP �ּҸ� �����ϴµ� �����߽��ϴ�."));

	gameSocket.optDebug = true;
	gameSocket.optReuseAddress = true;
	gameSocket.optUseLoopback = true;
	gameSocket.optBroadcast = true;
	gameSocket.optDontRoute = true;

	nameSocket.Listen().if_then([]() noexcept {
		util::Println("TCP ���Ͽ��� ������ ����մϴ�.");
	}).else_then(OnError("������ ���� ���� �ܰ谡 �����߽��ϴ�."));
}

void Framework::Start() noexcept
{
	util::Println("������ �����մϴ�...");

	auto test_socket = Socket::CreateTCP();

	nameSocket.Accept(*test_socket.GetResult(), acceptBuffer, acceptContext, accceptResultSize)
		.else_then(OnError("ù��° �񵿱� ���� �ܰ谡 �����߽��ϴ�."));
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
