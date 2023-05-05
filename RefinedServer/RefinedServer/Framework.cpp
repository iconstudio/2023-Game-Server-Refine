#include "pch.hpp"
#include "Framework.hpp"

using namespace net;

Framework::Framework() noexcept
	: Singleton(this)
{}

void Framework::Awake()
{}

void Framework::Start() noexcept
{}

void Framework::Update() noexcept
{
	while (true)
	{
		UpdateOnce(0);
	}
}

void Framework::UpdateOnce(const float& delta_time) noexcept
{}

util::Monad<int> Framework::BeginAccept()
{
	return util::Monad<int>();
}

void Framework::EndAccept() noexcept
{}

void Framework::EndFailedAccept() noexcept
{}

util::Monad<int> Framework::BeginWelcome()
{
	return util::Monad<int>();
}

void Framework::EndWelcome() noexcept
{}

void Framework::EndFailedWelcome() noexcept
{}

util::Monad<int> Framework::BeginDispose()
{
	return util::Monad<int>();
}

void Framework::EndDispose() noexcept
{}

void Framework::EndFailedDispose() noexcept
{}
