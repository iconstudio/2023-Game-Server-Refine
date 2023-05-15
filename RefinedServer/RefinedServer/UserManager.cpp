#include "pch.hpp"
#include "UserManager.hpp"
#include "Framework.hpp"

import Utility;
import Utility.Error;
import Utility.Print;

using namespace ::service;
using namespace ::net;

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

void UserManager::Start() noexcept
{}
