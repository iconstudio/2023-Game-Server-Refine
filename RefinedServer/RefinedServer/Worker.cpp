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

thread_local util::CancellationToken stopToken;
thread_local OVERLAPPED* localHandle{};
thread_local unsigned long long localKey{};
thread_local unsigned long localBytes{};

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

void Worker(net::WorkerUnit&& unit, net::CompletionPort& port)
{
	if (stopToken.stop_requested()) [[unlikely]] {
		// Await upto 5 seconds
		(void)port.Wait(util::addressof(localHandle), util::addressof(localKey), util::addressof(localBytes), 5000);

		//return false;
	};

	// Await infinitely
	const auto success = port.Wait(util::addressof(localHandle), util::addressof(localKey), util::addressof(localBytes));

	if (!success.IsSuccess()) [[unlikely]] {
		//return false;
	};

	if (nullptr == localHandle) [[unlikely]] {
		//return false;
	};

	Context* const& context = static_cast<Context*>(localHandle);
	if (!context) [[unlikely]] {
		//return false;
	};

	//util::forward<Fn>(fn)(context, localKey, localBytes);
}
