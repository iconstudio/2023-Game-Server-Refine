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
				util::Println("����� {}��(��) ã�� �� �����ϴ�.", key);
				return;
			}


		}
		break;

		case Operation::WELCOME:
		{
			User* const& user = ToUser(context);
			if (nullptr == user)
			{
				util::Println("����� {}��(��) ã�� �� �����ϴ�.", key);
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

	util::Println("�۾��� ������ {}��(��) �����մϴ�.", raw_id);

	unit.Start(::Update, port);

	util::Println("�۾��� ������ {}��(��) �����մϴ�.", raw_id);
}
