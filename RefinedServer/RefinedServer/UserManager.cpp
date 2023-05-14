#include "pch.hpp"
#include "UserManager.hpp"
#include "Framework.hpp"

service::UserManager::UserManager() noexcept
	: Singleton(this)
	, everySession(), everyUser()
{}

service::UserManager::~UserManager() noexcept
{}

void service::UserManager::Awake()
{}

void service::UserManager::Start() noexcept
{}
