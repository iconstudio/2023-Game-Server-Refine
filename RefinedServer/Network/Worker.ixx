export module Net.Worker;
import Utility.Concurrency.Thread;
import Net;
import Net.CompletionPort;
import Net.Context;

export namespace net
{
	class WorkerUnit
	{
	public:
		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
