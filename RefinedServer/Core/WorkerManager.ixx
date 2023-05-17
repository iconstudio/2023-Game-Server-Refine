module;
#include <vector>

export module Core.Service.WorkerManager;
import Utility;
import Utility.Constraints;
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
import Net.Context;
import Net.Worker;

export namespace core::service
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

		template<util::invocables Fn>
		inline void Push(Fn&& fn) noexcept
		{
			myWorkers.emplace_back(util::thread{ util::forward<Fn>(fn) });
		}

		inline void Push(util::thread&& thread) noexcept
		{
			myWorkers.emplace_back(static_cast<util::thread&&>(thread), stopSource.get_token());
		}

		inline void Push(net::WorkerUnit&& worker) noexcept
		{
			myWorkers.emplace_back(static_cast<net::WorkerUnit&&>(worker));
		}

		WorkerManager(WorkerManager&&) noexcept = default;
		WorkerManager& operator=(WorkerManager&&) noexcept = default;
		WorkerManager(const WorkerManager&) = delete;
		WorkerManager& operator=(const WorkerManager&) = delete;
		~WorkerManager() = default;

	private:
		std::vector<net::WorkerUnit> myWorkers;
		util::CancellationSource stopSource;
	};
}
