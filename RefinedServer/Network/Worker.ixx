module;
#include <utility>

export module Net.Worker;
import Utility.Constraints;
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
import Net;
import Net.CompletionPort;
import Net.Context;

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

		template<typename Fn>
			requires (util::invocables<Fn, Context*, unsigned long long, unsigned long, bool>)
		inline void Start(Fn&& fn, CompletionPort& port)
			noexcept(noexcept(std::forward<Fn>(fn)(std::declval<Context*>(), std::declval<unsigned long long>(), std::declval<unsigned long>(), std::declval<bool>())))
		{
			while (true)
			{
				if (myThread.stop_requested()) [[unlikely]] {
					// Await upto 5 seconds
					(void)port.Wait(localHandle, std::addressof(localKey), std::addressof(localBytes), 5000);

					return;
				};

				// Await infinitely
				const auto result = port.Wait(localHandle, std::addressof(localKey), std::addressof(localBytes));

				if (nullptr == localHandle) [[unlikely]] {
					return;
				};

				Context* const& context = static_cast<Context*>(localHandle);
				if (!context) [[unlikely]] {
					return;
				};

				std::forward<Fn>(fn)(context, localKey, localBytes, result.IsSuccess());
			}
		}

		[[nodiscard]]
		inline util::thread::id get_id() const noexcept
		{
			return myThread.get_id();
		}

		[[nodiscard]]
		inline void* native_handle() noexcept
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
		WorkerUnit(WorkerUnit&& handle) noexcept = default;
		WorkerUnit& operator=(const WorkerUnit& handle) = delete;
		WorkerUnit& operator=(WorkerUnit&& handle) noexcept = default;

	private:
		util::ThreadUnit myThread;

		Context* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
