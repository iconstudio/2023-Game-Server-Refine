module;
#include <vector>

export module Core.Service.WorkerManager;
import Utility;
import Utility.Constraints;
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
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

			for (util::thread& thread : myWorkers)
			{
				thread.join();
			}
		}

		template<typename Fn, typename... Args>
			requires util::invocables<Fn, util::CancellationToken, Args...>
		inline void Push(Fn&& fn, Args&&... args) noexcept
		{
			myWorkers.emplace_back(util::thread{ util::forward<Fn>(fn), stopSource.get_token(), util::forward<Args>(args)... });
		}

		inline void Push(util::thread&& thread) noexcept
		{
			myWorkers.emplace_back(static_cast<util::thread&&>(thread));
		}

		WorkerManager(WorkerManager&&) noexcept = default;
		WorkerManager& operator=(WorkerManager&&) noexcept = default;
		WorkerManager(const WorkerManager&) = delete;
		WorkerManager& operator=(const WorkerManager&) = delete;

	private:
		std::vector<util::thread> myWorkers;
		util::CancellationSource stopSource;
	};
}
