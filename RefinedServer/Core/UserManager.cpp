module;
#include <exception>
#include <string_view>

module Core.Service.UserManager;
import Utility;
import Utility.Error;
import Utility.Print;
import Core.User.Identifier;

using namespace ::core::service;

UserManager::UserManager() noexcept
	: Singleton(this)
	, everySession(), everyUser(), everyNPCs()
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
{}
