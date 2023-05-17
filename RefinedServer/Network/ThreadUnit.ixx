module;
#include <exception>

export module Net.ThreadUnit;
import Utility;
import Utility.Concurrency.Thread;
import Net;

export namespace net
{
	/// <summary>
	/// Implementation of std::thread + std::stop_token
	/// </summary>
	class ThreadUnit
	{
	public:
		ThreadUnit(util::thread&& thread, util::CancellationSource& ssource) noexcept
			: ThreadUnit(static_cast<util::thread&&>(thread), ssource.get_token())
		{}

		ThreadUnit(util::thread&& thread, util::CancellationToken&& token) noexcept
			: myHandle(static_cast<util::thread&&>(thread))
			, stopToken(static_cast<util::CancellationToken&&>(token))
		{}

		~ThreadUnit()
		{
			join();
		}

		inline void swap(ThreadUnit& other) noexcept
		{
			myHandle.swap(other.myHandle);
			stopToken.swap(other.stopToken);
		}

		inline void join()
		{
			myHandle.join();
		}

		inline void detach()
		{
			myHandle.detach();
		}

		[[nodiscard]]
		inline std::thread::id get_id() const noexcept
		{
			return myHandle.get_id();
		}

		[[nodiscard]]
		inline std::thread::native_handle_type native_handle() noexcept
		{
			return myHandle.native_handle();
		}

		[[nodiscard]]
		inline bool joinable() const noexcept
		{
			return myHandle.joinable();
		}

		[[nodiscard]]
		inline bool stop_requested() const noexcept
		{
			return stopToken.stop_requested();
		}

		[[nodiscard]]
		inline bool stop_possible() const noexcept
		{
			return stopToken.stop_possible();
		}

		util::thread myHandle;
		util::CancellationToken stopToken;
	};
}
