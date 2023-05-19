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
		WorkerUnit() noexcept
			: myThread()
		{}

		explicit WorkerUnit(util::ThreadUnit&& unit) noexcept
			: myThread(static_cast<util::ThreadUnit&&>(unit))
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
