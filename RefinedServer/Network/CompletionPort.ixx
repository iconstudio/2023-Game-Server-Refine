module;
#define NOMINMAX
#include <winsock2.h>
#include <chrono>

export module Net.CompletionPort;
import Net;
import Net.Promise;
import Net.Socket;

using ulong = unsigned long;
using ullong = unsigned long long;

export using ::UINT_PTR;
export using ::SOCKET;

export namespace net
{
	using registerPromise = Promise<void, int, void>;
	using PortPromise = Promise<void, void, void>;

	namespace abi
	{
		[[nodiscard]]
		inline registerPromise
			RegisterIoPort(const HANDLE& io_port, const HANDLE& handle, const ullong& key) noexcept;

		[[nodiscard]]
		inline PortPromise
			PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept;

		[[nodiscard]]
		inline PortPromise
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
			Promise<CompletionPort, int, void>
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

		inline registerPromise Link(Socket& socket, const ullong& unique_id) noexcept
		{
			return _Link(socket.Handle(), unique_id);
		}

		inline registerPromise Link(Socket& socket, ullong&& unique_id) noexcept
		{
			return _Link(socket.Handle(), static_cast<ullong&&>(unique_id));
		}

		template<typename Represent, typename Period>
		inline auto Wait(OVERLAPPED* ctx, ullong* key_handle, ulong* bytes_handle, const std::chrono::duration<Represent, Period>& await_time)
		{
			return abi::GetIoPortResult(rawHandle, key_handle, bytes_handle, &ctx, std::chrono::duration_cast<std::chrono::milliseconds>(await_time).count());
		}

		inline PortPromise Wait(OVERLAPPED* ctx, ullong* key_handle, ulong* bytes_handle, const ulong& await_time = abi::infinite)
		{
			return abi::GetIoPortResult(rawHandle, key_handle, bytes_handle, &ctx, await_time);
		}

		inline PortPromise Post(OVERLAPPED* ctx, const ullong& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, key, bytes, ctx);
		}

		inline PortPromise Post(OVERLAPPED* ctx, ullong&& key, const ulong& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), bytes, ctx);
		}

		inline PortPromise Post(OVERLAPPED* ctx, const ullong& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, key, static_cast<ulong&&>(bytes), ctx);
		}

		inline PortPromise Post(OVERLAPPED* ctx, ullong&& key, ulong&& bytes)
		{
			return abi::PostIoPort(rawHandle, static_cast<ullong&&>(key), static_cast<ulong&&>(bytes), ctx);
		}

		[[nodiscard]]
		inline bool IsValid() const noexcept
		{
			return nullptr != rawHandle && 0 < reinterpret_cast<long long>(rawHandle);
		}

		CompletionPort(const CompletionPort& handle) = delete;
		CompletionPort& operator=(const CompletionPort& handle) = delete;

	private:
		inline registerPromise _Link(const SOCKET& target, const ullong& unique_id) noexcept
		{
			return _Link(reinterpret_cast<HANDLE>(target), unique_id);
		}

		inline registerPromise _Link(const SOCKET& target, ullong&& unique_id) noexcept
		{
			return _Link(reinterpret_cast<HANDLE>(target), static_cast<ullong&&>(unique_id));
		}

		inline registerPromise _Link(const HANDLE& handle, const ullong& unique_id) noexcept
		{
			return abi::RegisterIoPort(rawHandle, handle, unique_id);
		}

		inline registerPromise _Link(const HANDLE& handle, ullong&& unique_id) noexcept
		{
			return abi::RegisterIoPort(rawHandle, handle, static_cast<ullong&&>(unique_id));
		}

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
			return registerPromise{ io::success };
		}
	}

	PortPromise
		abi::PostIoPort(const HANDLE& io_port, const ullong& key, const ulong& bytes, OVERLAPPED* const& overlapped) noexcept
	{
		if (0 != ::PostQueuedCompletionStatus(io_port, bytes, key, overlapped))
		{
			return io::success;
		}
		else
		{
			return io::failure;
		}
	}

	PortPromise
		abi::GetIoPortResult(const HANDLE& io_port, ullong* const& key_handle, ulong* const& bytes_handle, OVERLAPPED** const& overlapped_handle, const ulong& await_time) noexcept
	{
		if (0 != ::GetQueuedCompletionStatus(io_port, bytes_handle, key_handle, overlapped_handle, await_time))
		{
			return io::success;
		}
		else
		{
			return io::failure;
		}
	}
}
