#include "pch.hpp"
#include "Worker.hpp"
#include "Framework.hpp"

import Utility;
import Utility.Print;
import Utility.Error;
import Utility.Concurrency.Thread.Unit;
import Net;
import Net.CompletionPort;
import Net.Context;
import Net.Worker;

using namespace ::net;
using namespace ::core;

void Update(Context* const& context, const unsigned long long& key, const unsigned long& bytes)
{
	switch (context->GetOperation())
	{
		case Operation::ACCEPT:
		{}
		break;

		case Operation::WELCOME:
		{}
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
