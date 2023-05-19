module;
#include <vector>

export module Core.Service.WorkerManager;
import Utility;
import Utility.Constraints;
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
import Net;
import Net.Context;
import Net.Worker;

export extern "C++" namespace core::service
{
	class [[nodiscard]] WorkerManager
	{
	public:
		WorkerManager() noexcept
			: myWorkers()
			, stopSource()
		{}

		WorkerManager(const size_t& reserved_size) noexcept
			: myWorkers()
			, stopSource()
		{
			myWorkers.reserve(reserved_size);
		}

		~WorkerManager()
		{
			stopSource.request_stop();

			for (auto* worker : myWorkers)
			{
				delete worker;
			}
		}

		template<typename Fn, typename... Args>
			requires util::invocables<Fn, net::WorkerUnit&, Args...>
		inline void Push(Fn&& fn, Args&&... args) noexcept
		{
			net::WorkerUnit* unit = new net::WorkerUnit{};

			util::ThreadUnit thread_unit = util::ThreadUnit{ util::forward<Fn>(fn), stopSource, util::ref(*unit), util::forward<Args>(args)... };

			*unit = net::WorkerUnit{ static_cast<util::ThreadUnit&&>(thread_unit) };

			myWorkers.push_back(unit);
		}

		WorkerManager(WorkerManager&&) noexcept = default;
		WorkerManager& operator=(WorkerManager&&) noexcept = default;
		WorkerManager(const WorkerManager&) = delete;
		WorkerManager& operator=(const WorkerManager&) = delete;

	private:
		std::vector<net::WorkerUnit*> myWorkers;
		util::CancellationSource stopSource;
	};
}
