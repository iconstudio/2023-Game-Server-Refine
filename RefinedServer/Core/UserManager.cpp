module;
#include <exception>
#include <string_view>

module Core.Service.UserManager;
import Utility;
import Utility.Error;
import Utility.Print;
import Core.User.Identifier;

using namespace ::net;
using namespace ::core::service;

UserManager::UserManager(Socket& listener) noexcept
	: Singleton(this)
	, nameListener(listener)
	, everySession(), everyUser(), everyNPCs()
	, acceptContext(Operation::ACCEPT), acceptBuffer(), accceptResultSize(0)
{}

UserManager::~UserManager() noexcept
{}

void UserManager::Awake()
{
	size_t index = 0;
	userid_t user_id = userid_t::begin;

	try
	{
		for (auto& session : everyUser)
		{
			session = new User(user_id++);

			everySession[index++] = session;
		}
	}
	catch (std::exception& e)
	{
		util::Println("���� ���� �߿� ���ܰ� �߻��߽��ϴ�. (�޽���: {})", e.what());
		util::unreachable();
	}

	try
	{
		for (auto& session : everyNPCs)
		{
			session = new BasicUser(user_id++);

			everySession[index++] = session;
		}
	}
	catch (std::exception& e)
	{
		util::Println("NPC ���� �߿� ���ܰ� �߻��߽��ϴ�. (�޽���: {})", e.what());
		util::unreachable();
	}
}

void UserManager::Start() noexcept
{
	BeginAccept(userid_t::begin).else_then(OnError("ù��° �񵿱� ���� �ܰ谡 �����߽��ϴ�."));
}

net::ioError core::service::UserManager::BeginAccept(const core::userid_t& start)
{
	auto test_socket = Socket::CreateTCP();

	return nameListener.Accept(*test_socket.GetResult(), acceptBuffer, acceptContext, accceptResultSize);
}

void core::service::UserManager::EndAccept() noexcept
{}

void core::service::UserManager::EndFailedAccept() noexcept
{}
