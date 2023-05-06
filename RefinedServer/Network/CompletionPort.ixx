module;
#define NOMINMAX
#include <winsock2.h>

export module Net.CompletionPort;
import Utility.Monad;
import Net.Intrinsics;

using ulong = unsigned long;
using ullong = unsigned long long;

export namespace net
{
	namespace abi
	{
		[[nodiscard]]
		FORCEINLINE HANDLE RegisterIoPort(const HANDLE& io_port, const HANDLE& handle, const ullong& key) noexcept;
		[[nodiscard]]
		FORCEINLINE bool PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept;
		[[nodiscard]]
		FORCEINLINE bool GetIoPortResult(const HANDLE& io_port, ullong* const& key_handle, ulong* const& bytes_handle, OVERLAPPED** const& overlapped_handle, const ulong& await_time = INFINITE) noexcept;
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
		constexpr CompletionPort() noexcept = default;

		~CompletionPort()
		{
#if !_DEBUG
			::CloseHandle(rawHandle);
#endif // !_DEBUG
		}

		[[nodiscard]]
		static inline util::Monad<CompletionPort> Create(const ulong& concurrent_hint) noexcept
		{
			const HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, concurrent_hint);

			return static_cast<CompletionPort&&>(CompletionPort{ handle });
		}

		inline bool Link(const SOCKET& target, const ullong& unique_id) noexcept
		{
			return Link(reinterpret_cast<HANDLE>(target), unique_id);
		}

		inline bool Link(const SOCKET& target, ullong&& unique_id) noexcept
		{
			return Link(reinterpret_cast<HANDLE>(target), static_cast<ullong&&>(unique_id));
		}

		inline bool Link(const HANDLE& handle, const ullong& unique_id) noexcept
		{
			return nullptr != abi::RegisterIoPort(rawHandle, handle, unique_id);
		}

		inline bool Link(const HANDLE& handle, ullong&& unique_id) noexcept
		{
			return nullptr != abi::RegisterIoPort(rawHandle, handle, static_cast<ullong&&>(unique_id));
		}

		inline bool Poll(OVERLAPPED** overlapped_handle, ullong* key_handle, ulong* bytes_handle, const ulong& await_time = INFINITE)
		{
			return abi::GetIoPortResult(rawHandle, key_handle, bytes_handle, overlapped_handle, await_time);
		}

		inline bool Post(OVERLAPPED* overlapped_handle, const ullong& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, key, bytes, overlapped_handle);
		}

		inline bool Post(OVERLAPPED* overlapped_handle, ullong&& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), bytes, overlapped_handle);
		}

		inline bool Post(OVERLAPPED* overlapped_handle, const ullong& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, key, static_cast<ulong&&>(bytes), overlapped_handle);
		}

		inline bool Post(OVERLAPPED* overlapped_handle, ullong&& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), static_cast<ulong&&>(bytes), overlapped_handle);
		}

		inline bool IsValid() const noexcept
		{
			return nullptr != rawHandle && 0 < reinterpret_cast<long long>(rawHandle);
		}

		CompletionPort(const CompletionPort& handle) = delete;
		CompletionPort& operator=(const CompletionPort& handle) = delete;
		CompletionPort(CompletionPort&& handle) = default;
		CompletionPort& operator=(CompletionPort&& handle) = default;

	private:
		HANDLE rawHandle;
	};

	HANDLE abi::RegisterIoPort(const HANDLE& io_port, const HANDLE& handle, const ullong& key) noexcept
	{
		return ::CreateIoCompletionPort(handle, io_port, key, 0);
	}

	bool abi::PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept
	{
		return 0 != ::PostQueuedCompletionStatus(io_port, bytes, key, overlapped);
	}

	bool abi::GetIoPortResult(const HANDLE& io_port, ullong* const& key_handle, ulong* const& bytes_handle, OVERLAPPED** const& overlapped_handle, const ulong& await_time) noexcept
	{
		return 0 != ::GetQueuedCompletionStatus(io_port, bytes_handle, key_handle, overlapped_handle, await_time);
	}
}
