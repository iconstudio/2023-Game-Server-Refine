export module Net.Worker;
import Utility;
import Utility.Concurrency.Thread;
import Net;
import Net.CompletionPort;
import Net.Context;
import Net.ThreadUnit;

export namespace net
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

		inline void Update(CompletionPort& port)
		{
			while (UpdateOnce(port));
		}

		inline bool UpdateOnce(CompletionPort& port)
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


			return true;
		}

		ThreadUnit myUnit;
		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
