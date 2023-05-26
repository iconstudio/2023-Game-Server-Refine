module;
#include <exception>
#include <string_view>
#include <algorithm>
#include <ranges>

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
using namespace ::core;

UserManager::UserManager(Socket& listener) noexcept
	: Singleton(this)
	, nameListener(listener)
	, everySession(), everyUser(), everyNPCs()
	, lastUserID(userid_t::begin)
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
				util::Println("유저의 소켓 생성 중에 오류 {}이(가) 발생했습니다.", error_code);
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
		util::Println("유저 생성 중에 예외가 발생했습니다. (메시지: {})", e.what());
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
		util::Println("NPC 생성 중에 예외가 발생했습니다. (메시지: {})", e.what());
		util::unreachable();
	}
}

void UserManager::Start() noexcept
{}

ioError UserManager::BeginAccept(const userid_t& start)
{
	const size_t index = static_cast<size_t>(start - userid_t::begin);

	return UserAt(index).transform(
		[&](User* const& user) noexcept -> ioError {
		return nameListener.Accept(user->mySocket
			, acceptBuffer, accceptResultSize
			, acceptContext);
	}, [&start]() noexcept -> ioError {
		util::Println("유저 {}을(를) 찾을 수 없습니다.", start);

		return -1;
	});
}

void UserManager::EndAccept(User* const& newbie) noexcept
{
	if (!newbie->IsTaken()) [[unlikely]]
	{
		return EndFailedAccept(newbie);
	};

	util::Println("새로운 유저 {}이(가) 접속했습니다.", newbie->MyID());

	lastUserID = newbie->MyID() + 1;

	newbie->Welcome();
}

void UserManager::EndFailedAccept(User* const& newbie) noexcept
{
	util::Println("새로운 유저 {}에 문제가 있어 수용할 수 없습니다.", newbie->MyID());

	newbie->Cleanup();
}

util::Monad<BasicUser*> UserManager::SessionAt(const size_t& index) noexcept
{
	if (index < maxSessions)
		return everySession[index];
	else
		return util::nullopt;
}

util::Monad<User*> UserManager::UserAt(const size_t& index) noexcept
{
	if (index < maxUsers)
		return everyUser[index];
	else
		return util::nullopt;
}

util::Monad<BasicUser*> UserManager::NpcAt(const size_t& index) noexcept
{
	if (index < maxNPCs)
		return everyNPCs[index];
	else
		return util::nullopt;
}

util::Monad<BasicUser*> UserManager::SessionOf(const userid_t& id) noexcept
{
	return SessionAt(CastID(id));
}

util::Monad<User*> UserManager::UserOf(const userid_t& id) noexcept
{
	return UserAt(CastID(id));
}

util::Monad<BasicUser*> UserManager::NpcOf(const userid_t& id) noexcept
{
	return NpcAt(CastID(id));
}
