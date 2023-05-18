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
		WorkerUnit(util::CancellationSource& ssource) noexcept
			: cancelToken(ssource.get_token())
		{}

		WorkerUnit(util::CancellationToken&& token) noexcept
			: cancelToken(static_cast<util::CancellationToken&&>(token))
		{}

		~WorkerUnit() = default;

		template<util::invocables<Context*, unsigned long long, unsigned long> Fn>
		inline void Start(CompletionPort& port, Fn&& fn)
			noexcept(noexcept(util::forward<Fn>(fn)(util::declval<Context*>(), util::declval<unsigned long long>(), util::declval<unsigned long>())))
		{
			while (Update(port, util::forward<Fn>(fn)));
		}

		WorkerUnit(const WorkerUnit& handle) = delete;
		WorkerUnit& operator=(const WorkerUnit& handle) = delete;

	private:
		template<util::invocables<Context*, unsigned long long, unsigned long> Fn>
		inline bool Update(CompletionPort& port, Fn&& fn)
			noexcept(noexcept(util::forward<Fn>(fn)()))
		{
			if (cancelToken.stop_requested()) [[unlikely]] {
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

		util::CancellationToken cancelToken;
		OVERLAPPED* localHandle{};
		unsigned long long localKey{};
		unsigned long localBytes{};
	};
}
