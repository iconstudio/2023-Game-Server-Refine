export module Net.Worker;
import Utility;
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
		WorkerUnit(util::thread&& thread, util::CancellationSource& ssource) noexcept
			: myUnit(static_cast<util::thread&&>(thread), ssource.get_token())
		{}

		WorkerUnit(util::thread&& thread, util::CancellationToken&& token) noexcept
			: myUnit(static_cast<util::thread&&>(thread), static_cast<util::CancellationToken&&>(token))
		{}

		~WorkerUnit() = default;

		template<util::invocables<Context*, unsigned long long, unsigned long> Fn>
		inline void Update(CompletionPort& port, Fn&& fn)
			noexcept(noexcept(util::forward<Fn>(fn)()))
		{
			while (UpdateOnce(port, fn));
		}

		WorkerUnit(const WorkerUnit& handle) = delete;
		WorkerUnit& operator=(const WorkerUnit& handle) = delete;

	private:
		template<util::invocables<Context*, unsigned long long, unsigned long> Fn>
		inline bool UpdateOnce(CompletionPort& port, Fn&& fn)
			noexcept(noexcept(util::forward<Fn>(fn)()))
		{
			if (myUnit.stop_requested()) [[unlikely]] {
				// Await upto 5 seconds
				(void)port.Wait(util::addressof(localHandle), util::addressof(localKey), util::addressof(localBytes), 5000);

				return false;
			};

			// Await infinitely
			const auto success = port.Wait(util::addressof(localHandle), util::addressof(localKey), util::addressof(localBytes));

			if (!success.IsSuccess()) [[unlikely]] {
				return false;
			};

			if (nullptr == localHandle) [[unlikely]] {
				return false;
			};

			Context* const& context = static_cast<Context*>(localHandle);
			if (!context) [[unlikely]] {
				return false;
			};

			util::forward<Fn>(fn)(context, localKey, localBytes);

			return true;
		}

		util::ThreadUnit myUnit;
		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
