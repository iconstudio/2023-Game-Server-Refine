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
		util::Println("{} (오류 코드: {})", myMsg, error_code);
		util::err::RaiseSystemError(std::move(error_code));

#if _DEBUG
		//std::unreachable();
#endif // !_DEBUG
	}

	util::string_view myMsg;
};

void Framework::Awake()
{
	util::Println("서버를 초기화하는 중...");

	int result_startup = Startup();
	if (result_startup != 0)
	{
		OnError("WSAStartup에 실패했습니다.")(std::move(result_startup));

		util::unreachable();
	}

	CompletionPort::Establish(concurrentHint).if_then(
		[this](CompletionPort&& port) noexcept {
		ioPort = std::move(port);
	}).else_then(OnError("IOCP를 생성하는데 실패했습니다."));

	Socket::CreateTCP().if_then([this](Socket&& socket) noexcept {
		nameSocket = std::move(socket);
	}).else_then(OnError("서버의 TCP 소켓을 생성하는데 실패했습니다."));

	nameEndPoint = EndPoint::CreateStaticTCP(AddressFamily::IPv4, tcpPort);
	nameSocket.Bind(nameEndPoint)
		.else_then(OnError("서버의 주소를 지정하는데 실패했습니다."));

	ioPort.Link(nameSocket, serverTcpID)
		.else_then(OnError("서버의 TCP 소켓을 IOCP에 연결하는데 실패했습니다."));

	Socket::CreateUDP().if_then([this](Socket&& socket) noexcept {
		gameSocket = std::move(socket);
	}).else_then(OnError("서버의 UDP 소켓을 생성하는데 실패했습니다."));

	nameSocket.optDebug = true;
	nameSocket.optReuseAddress = true;
	nameSocket.optNoDelay = true;
	nameSocket.optUseLoopback = true;

	gameEndPoint = EndPoint::CreateStaticUDP(AddressFamily::IPv4, udpPort);
	gameSocket.Bind(gameEndPoint).else_then(OnError("서버의 UDP 주소를 지정하는데 실패했습니다."));

	ioPort.Link(gameSocket, serverUdpID)
		.else_then(OnError("서버의 UDP 소켓을 IOCP에 연결하는데 실패했습니다."));

	gameSocket.optDebug = true;
	gameSocket.optReuseAddress = true;
	gameSocket.optUseLoopback = true;
	gameSocket.optBroadcast = true;
	gameSocket.optDontRoute = true;

	userManager.Awake(ioPort);
}

void Framework::Start() noexcept
{
	util::Println("서버를 시작합니다...");

	nameSocket.Listen().if_then([]() noexcept {
		util::Println("TCP 소켓에서 수용을 대기합니다.");
	}).else_then(OnError("수용 시작 단계가 실패했습니다."));

	userManager.Start();

	for (size_t i = 0; i < concurrentHint; i++)
	{
		workerManager.Push(::Worker, util::ref(ioPort));
	}

	BeginAccept(userid_t::begin).else_then(OnError("첫번째 수용 단계가 실패했습니다."));
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
