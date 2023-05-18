export module Net.Worker;
import Utility;
import Utility.Constraints;
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
import Net;

export extern "C++" namespace net
{
	class [[nodiscard]] WorkerUnit
	{
	public:
		WorkerUnit(util::CancellationToken&& token) noexcept
			: stopToken(static_cast<util::CancellationToken&&>(token))
		{}

		WorkerUnit(const WorkerUnit& handle) = delete;
		WorkerUnit& operator=(const WorkerUnit& handle) = delete;

	private:
		util::CancellationToken stopToken;
		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
