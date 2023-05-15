#include "pch.hpp"
#include "UserManager.hpp"
#include "Framework.hpp"

using namespace service;
using namespace net;

UserManager::UserManager() noexcept
	: Singleton(this)
	, everySession(), everyUser(), everyNPCs()
{}

UserManager::~UserManager() noexcept
{}

void UserManager::Awake()
{
	for (auto& session : everySession)
	{
		session = new net::BasicUser(userid_t::begin);
	}

	for (auto& session : everyUser)
	{
		session = new net::User(userid_t::begin);
	}
}

void UserManager::Start() noexcept
{}
