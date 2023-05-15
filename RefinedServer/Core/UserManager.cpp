module;
#include <exception>
#include <string_view>

module Core.UserManager;
import Utility;
import Utility.Error;
import Utility.Print;
import Net.User.Identifier;

using namespace ::net;

core::UserManager::UserManager() noexcept
	: Singleton(this)
	, everySession(), everyUser(), everyNPCs()
{}

core::UserManager::~UserManager() noexcept
{}

void core::UserManager::Awake()
{
	size_t index = 0;
	userid_t user_id = userid_t::begin;

	try
	{
		for (auto& session : everyUser)
		{
			session = new net::User(user_id++);

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
			session = new net::BasicUser(user_id++);

			everySession[index++] = session;
		}
	}
	catch (std::exception& e)
	{
		util::Println("NPC ���� �߿� ���ܰ� �߻��߽��ϴ�. (�޽���: {})", e.what());
		util::unreachable();
	}
}

void core::UserManager::Start() noexcept
{}
