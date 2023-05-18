module;
#define NOMINMAX
#include <winsock2.h>
#include <chrono>

export module Net.CompletionPort;
import Net.Promise;

using ulong = unsigned long;
using ullong = unsigned long long;

export namespace net
{
	using registerPromise = ErrorHandler<void>;

	namespace abi
	{
		[[nodiscard]]
		inline registerPromise
			RegisterIoPort(const HANDLE& io_port, const HANDLE& handle, const ullong& key) noexcept;

		[[nodiscard]]
		inline Proxy
			PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept;

		[[nodiscard]]
		inline Proxy
			GetIoPortResult(const HANDLE& io_port, ullong* const& key_handle, ulong* const& bytes_handle, OVERLAPPED** const& overlapped_handle, const ulong& await_time = INFINITE) noexcept;

		inline constinit HANDLE invalidHandle = INVALID_HANDLE_VALUE;
		inline constexpr unsigned long infinite = INFINITE;
	}

	class [[nodiscard]] alignas(ullong) CompletionPort
	{
	private:
		explicit constexpr CompletionPort(const HANDLE& handle) noexcept
			: rawHandle(handle)
		{}

		explicit constexpr CompletionPort(HANDLE&& handle) noexcept
			: rawHandle(static_cast<HANDLE&&>(handle))
		{}

	public:
		constexpr CompletionPort() noexcept
			: rawHandle(abi::invalidHandle)
		{}

		constexpr CompletionPort(CompletionPort&& handle) noexcept
			: rawHandle(static_cast<HANDLE&&>(handle.rawHandle))
		{}

		constexpr CompletionPort& operator=(CompletionPort&& handle) noexcept
		{
			rawHandle = static_cast<HANDLE&&>(handle.rawHandle);
			return *this;
		}

		~CompletionPort() noexcept
		{
#if !_DEBUG
			::CloseHandle(rawHandle);
#endif // !_DEBUG
		}

		[[nodiscard]]
		static inline
			ErrorHandler<CompletionPort>
			Establish(const ulong& concurrent_hint) noexcept
		{
			const HANDLE handle = ::CreateIoCompletionPort(abi::invalidHandle, nullptr, 0, concurrent_hint);

			if (NULL == handle)
			{
				return ::WSAGetLastError();
			}
			else
			{
				return CompletionPort{ handle };
			}
		}

		inline registerPromise Link(const SOCKET& target, const ullong& unique_id) noexcept
		{
			return Link(reinterpret_cast<HANDLE>(target), unique_id);
		}

		inline registerPromise Link(const SOCKET& target, ullong&& unique_id) noexcept
		{
			return Link(reinterpret_cast<HANDLE>(target), static_cast<ullong&&>(unique_id));
		}

		inline registerPromise Link(const HANDLE& handle, const ullong& unique_id) noexcept
		{
			return abi::RegisterIoPort(rawHandle, handle, unique_id);
		}

		inline registerPromise Link(const HANDLE& handle, ullong&& unique_id) noexcept
		{
			return abi::RegisterIoPort(rawHandle, handle, static_cast<ullong&&>(unique_id));
		}

		template<typename Represent, typename Period>
		inline Proxy Wait(OVERLAPPED** overlapped_handle, ullong* key_handle, ulong* bytes_handle, const std::chrono::duration<Represent, Period>& await_time)
		{
			return abi::GetIoPortResult(rawHandle, key_handle, bytes_handle, overlapped_handle, std::chrono::duration_cast<std::chrono::milliseconds>(await_time).count());
		}

		inline Proxy Wait(OVERLAPPED** overlapped_handle, ullong* key_handle, ulong* bytes_handle, const ulong& await_time = abi::infinite)
		{
			return abi::GetIoPortResult(rawHandle, key_handle, bytes_handle, overlapped_handle, await_time);
		}

		inline Proxy Post(OVERLAPPED* context, const ullong& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, key, bytes, context);
		}

		inline Proxy Post(OVERLAPPED* context, ullong&& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), bytes, context);
		}

		inline Proxy Post(OVERLAPPED* context, const ullong& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, key, static_cast<ulong&&>(bytes), context);
		}

		inline Proxy Post(OVERLAPPED* context, ullong&& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), static_cast<ulong&&>(bytes), context);
		}

		[[nodiscard]]
		inline bool IsValid() const noexcept
		{
			return nullptr != rawHandle && 0 < reinterpret_cast<long long>(rawHandle);
		}

		CompletionPort(const CompletionPort& handle) = delete;
		CompletionPort& operator=(const CompletionPort& handle) = delete;

	private:
		HANDLE rawHandle;
	};

	registerPromise
		abi::RegisterIoPort(const HANDLE& io_port, const HANDLE& handle, const ullong& key)
		noexcept
	{
		const HANDLE result = ::CreateIoCompletionPort(handle, io_port, key, 0);

		if (NULL == result)
		{
			return ::WSAGetLastError();
		}
		else
		{
			return registerPromise{ io::make_success() };
		}
	}

	Proxy
		abi::PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept
	{
		if (0 != ::PostQueuedCompletionStatus(io_port, bytes, key, overlapped))
		{
			return Proxy{ io::make_success() };
		}
		else
		{
			return Proxy{ io::make_failure() };
		}
	}

	Proxy
		abi::GetIoPortResult(const HANDLE& io_port, ullong* const& key_handle, ulong* const& bytes_handle, OVERLAPPED** const& overlapped_handle, const ulong& await_time) noexcept
	{
		if (0 != ::GetQueuedCompletionStatus(io_port, bytes_handle, key_handle, overlapped_handle, await_time))
		{
			return Proxy{ io::make_success() };
		}
		else
		{
			return Proxy{ io::make_failure() };
		}
	}
}
