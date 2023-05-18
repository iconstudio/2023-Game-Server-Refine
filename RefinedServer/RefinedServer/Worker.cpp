#include "pch.hpp"
#include "Worker.hpp"
#include "Framework.hpp"

import Utility.Print;
import Utility.Error;
import Net.Worker;

using namespace ::net;
using namespace ::core;

void Update(Context* const& context, const unsigned long long& key, const unsigned long& bytes)
{
	
}

void Worker(util::CancellationToken&& cancel_token, CompletionPort& completion_port)
{
	net::WorkerUnit unit{ std::move(cancel_token) };

	// loop
	unit.Start(completion_port, Update);
}
