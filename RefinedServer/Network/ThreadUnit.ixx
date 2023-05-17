module;
#include <exception>

export module Net.ThreadUnit;
import Utility;
import Utility.Constraints;
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
		template<typename Fn, typename... Args>
			requires (!util::same_as<util::clean_t<Fn>, util::thread, util::jthread>&& util::invocables<Fn, Args...>)
		ThreadUnit(Fn&& functor, util::CancellationToken&& token, Args&&... args) noexcept
			: ThreadUnit(util::thread{ functor, util::forward<Args>(args)... }, static_cast<util::CancellationToken&&>(token))
		{}

		template<typename Fn, typename... Args>
			requires (!util::same_as<util::clean_t<Fn>, util::thread, util::jthread>&& util::invocables<Fn, Args...>)
		ThreadUnit(Fn&& functor, util::CancellationSource& ssource, Args&&... args) noexcept
			: ThreadUnit(util::thread{ functor, util::forward<Args>(args)... }, ssource.get_token())
		{}

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

		ThreadUnit(const ThreadUnit& other) = delete;
		ThreadUnit(ThreadUnit&& other) noexcept = default;
		ThreadUnit& operator=(const ThreadUnit& other) = delete;
		ThreadUnit& operator=(ThreadUnit&& other) = default;

		util::thread myHandle;
		util::CancellationToken stopToken;
	};
}
