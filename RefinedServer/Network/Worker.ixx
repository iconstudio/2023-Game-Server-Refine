export module Net.Worker;
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

		[[nodiscard]]
		inline auto get_id() const noexcept
		{
			return myThread.get_id();
		}

		[[nodiscard]]
		inline auto native_handle() noexcept
		{
			return myThread.native_handle();
		}

		[[nodiscard]]
		inline bool stop_requested() const noexcept
		{
			return myThread.stop_requested();
		}

		[[nodiscard]]
		inline bool stop_possible() const noexcept
		{
			return myThread.stop_possible();
		}

		WorkerUnit(const WorkerUnit& handle) = delete;
		WorkerUnit(WorkerUnit&& handle) = default;
		WorkerUnit& operator=(const WorkerUnit& handle) = delete;
		WorkerUnit& operator=(WorkerUnit&& handle) = default;

	private:
		util::ThreadUnit myThread;

		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
