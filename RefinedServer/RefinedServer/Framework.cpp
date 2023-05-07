#include "pch.hpp"
#include "Framework.hpp"

import Utility.Print;

using namespace net;

Framework::Framework() noexcept
	: Singleton(this)
{}

void Framework::Awake()
{}

void Framework::Start() noexcept
{
	util::Println("서버를 시작합니다.");
}

void Framework::Update() noexcept
{
	while (true)
	{
		UpdateOnce(0);
	}
}

void Framework::UpdateOnce(const float& delta_time) noexcept
{}

Proxy Framework::BeginAccept()
{
	return {};
}

void Framework::EndAccept() noexcept
{}

void Framework::EndFailedAccept() noexcept
{}

Proxy Framework::BeginWelcome()
{
	return {};
}

void Framework::EndWelcome() noexcept
{}

void Framework::EndFailedWelcome() noexcept
{}

Proxy Framework::BeginDispose()
{
	return {};
}

void Framework::EndDispose() noexcept
{}

void Framework::EndFailedDispose() noexcept
{}
