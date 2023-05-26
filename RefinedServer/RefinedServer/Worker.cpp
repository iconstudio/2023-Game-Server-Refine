#include "pch.hpp"
#include "Worker.hpp"
#include "Framework.hpp"

import Utility;
import Utility.Print;
import Utility.Error;
import Net;
import Net.CompletionPort;
import Net.Context;
import Net.Worker;
import Core.User;
import Core.Packet.Identifier;

using namespace ::net;
using namespace ::core;

NODISCARD constexpr User* ToUser(Context* const& context)
{
	try
	{
		return static_cast<User*>(context);
	}
	catch (...)
	{
		return nullptr;
	}
}

void Update(Context* const& context, const unsigned long long& key, const unsigned long& bytes)
{
	switch (context->GetOperation())
	{
		case Operation::ACCEPT:
		{
			User* const& user = ToUser(context);
			if (nullptr == user)
			{
				util::Println("사용자 {}을(를) 찾을 수 없습니다.", key);
				return;
			}


		}
		break;

		case Operation::WELCOME:
		{
			User* const& user = ToUser(context);
			if (nullptr == user)
			{
				util::Println("사용자 {}을(를) 찾을 수 없습니다.", key);
				return;
			}


		}
		break;

		case Operation::CLOSE:
		{}
		break;

		case Operation::RECV:
		{}
		break;

		case Operation::SEND:
		{}
		break;
	}
}

void Worker(net::WorkerUnit& unit, net::CompletionPort& port)
{
	const auto id = unit.get_id();
	const unsigned long raw_id = *reinterpret_cast<const unsigned long*>(&id);

	util::Println("작업자 스레드 {}을(를) 시작합니다.", raw_id);

	unit.Start(::Update, port);

	util::Println("작업자 스레드 {}을(를) 종료합니다.", raw_id);
}
