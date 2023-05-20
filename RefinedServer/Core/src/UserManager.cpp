module;
#include <exception>
#include <string_view>

module Core.Service.UserManager;
import Utility;
import Utility.Error;
import Utility.Print;
import Net.Socket;
import Net.CompletionPort;
import Core.User;
import Core.User.Basic;
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

void UserManager::Awake(net::CompletionPort& port)
{
	size_t index = 0;
	userid_t user_id = userid_t::begin;

	try
	{
		for (auto& session : everyUser)
		{
			session = Socket::CreateTCP().else_then([](int&& error_code) {
				util::Println("������ ���� ���� �߿� ���� {}��(��) �߻��߽��ϴ�.", error_code);
				util::err::RaiseSystemError(util::move(error_code));
			}).and_then(
				[&](Socket&& socket) noexcept -> User* {
				socket.optKeepAlive = false;
				socket.optReuseAddress = true;
				socket.optNoDelay = true;
				nameListener.optUpdateContext = socket;

				port.Link(socket, CastID(user_id));

				return new User(user_id++, util::move(socket));
			});

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
{}

ioError UserManager::BeginAccept(const userid_t& start)
{
	auto test_socket = Socket::CreateTCP();

	return nameListener.Accept(test_socket.GetResult(), acceptBuffer, acceptContext, accceptResultSize);
}

void UserManager::EndAccept() noexcept
{}

void UserManager::EndFailedAccept() noexcept
{}

util::Monad<core::BasicUser*> UserManager::SessionAt(const size_t& index) noexcept
{
	if (index < maxSessions)
		return everySession[index];
	else
		return util::nullopt;
}

util::Monad<core::User*> UserManager::UserAt(const size_t& index) noexcept
{
	if (index < maxUsers)
		return everyUser[index];
	else
		return util::nullopt;
}

util::Monad<core::BasicUser*> UserManager::NpcAt(const size_t& index) noexcept
{
	if (index < maxNPCs)
		return everyNPCs[index];
	else
		return util::nullopt;
}

util::Monad<core::BasicUser*> UserManager::SessionOf(const userid_t& id) noexcept
{
	return SessionAt(CastID(id));
}

util::Monad<core::User*> UserManager::UserOf(const userid_t& id) noexcept
{
	return UserAt(CastID(id));
}

util::Monad<core::BasicUser*> UserManager::NpcOf(const userid_t& id) noexcept
{
	return NpcAt(CastID(id));
}
