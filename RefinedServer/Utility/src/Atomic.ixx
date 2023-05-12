module;
#include <utility>
#include <atomic>
export module Utility.Atomic;

export namespace util
{
	namespace detail
	{
		template<typename T, size_t Size>
		using atomic_stg_t = ::std::_Atomic_storage<T, Size>;
		template<typename T>
		using atomic_s1_t = ::std::_Atomic_storage<T, 1>;
		template<typename T>
		using atomic_s2_t = ::std::_Atomic_storage<T, 2>;
		template<typename T>
		using atomic_s4_t = ::std::_Atomic_storage<T, 4>;
		template<typename T>
		using atomic_s8_t = ::std::_Atomic_storage<T, 8>;
		template<typename T>
		using atomic_s16_t = ::std::_Atomic_storage<T, 16>;

		template<typename T, size_t Size>
		using atomic_int_t = ::std::_Atomic_integral<T, Size>;
		template<typename T>
		using atomic_i1_t = ::std::_Atomic_integral<T, 1>;
		template<typename T>
		using atomic_i2_t = ::std::_Atomic_integral<T, 2>;
		template<typename T>
		using atomic_i4_t = ::std::_Atomic_integral<T, 4>;
		template<typename T>
		using atomic_i8_t = ::std::_Atomic_integral<T, 8>;

		template<typename T>
		using atomic_float_t = ::std::_Atomic_floating<T>;
	}

	export using ::std::atomic;
	export using ::std::atomic_ref;
	export using ::std::atomic_flag;
	export using ::std::atomic_bool;
	export using ::std::atomic_char;
	export using ::std::atomic_schar;
	export using ::std::atomic_uchar;
	export using ::std::atomic_short;
	export using ::std::atomic_ushort;
	export using ::std::atomic_int;
	export using ::std::atomic_uint;
	export using ::std::atomic_long;
	export using ::std::atomic_ulong;
	export using ::std::atomic_llong;
	export using ::std::atomic_ullong;
	export using ::std::atomic_char8_t;
	export using ::std::atomic_char16_t;
	export using ::std::atomic_char32_t;
	export using ::std::atomic_wchar_t;
	export using ::std::atomic_ptrdiff_t;
	export using ::std::atomic_size_t;

	export using ::std::memory_order;
	export inline constexpr memory_order memory_order_relaxed = ::std::memory_order_relaxed;
	export inline constexpr memory_order memory_order_consume = ::std::memory_order_consume;
	export inline constexpr memory_order memory_order_acquire = ::std::memory_order_acquire;
	export inline constexpr memory_order memory_order_release = ::std::memory_order_release;
	export inline constexpr memory_order memory_order_acq_rel = ::std::memory_order_acq_rel;
	export inline constexpr memory_order memory_order_seq_cst = ::std::memory_order_seq_cst;

	export using ::std::atomic_is_lock_free;
	export using ::std::atomic_fetch_add;
	export using ::std::atomic_fetch_add_explicit;
	export using ::std::atomic_fetch_sub;
	export using ::std::atomic_fetch_sub_explicit;
	export using ::std::atomic_fetch_xor;
	export using ::std::atomic_fetch_xor_explicit;
	export using ::std::atomic_fetch_and;
	export using ::std::atomic_fetch_and_explicit;
	export using ::std::atomic_fetch_or;
	export using ::std::atomic_fetch_or_explicit;
	export using ::std::atomic_compare_exchange_strong;
	export using ::std::atomic_compare_exchange_strong_explicit;
	export using ::std::atomic_compare_exchange_weak;
	export using ::std::atomic_compare_exchange_weak_explicit;
	export using ::std::atomic_exchange;
	export using ::std::atomic_exchange_explicit;
	export using ::std::atomic_wait;
	export using ::std::atomic_wait_explicit;
	export using ::std::atomic_flag_clear;
	export using ::std::atomic_flag_clear_explicit;
	export using ::std::atomic_flag_test;
	export using ::std::atomic_flag_test_explicit;
	export using ::std::atomic_flag_test_and_set;
	export using ::std::atomic_flag_test_and_set_explicit;
	export using ::std::atomic_flag_wait;
	export using ::std::atomic_flag_wait_explicit;
	export using ::std::atomic_flag_notify_one;
	export using::std::atomic_flag_notify_all;

	template<std::copyable T, std::convertible_to<T> U>
	inline
		bool
		CompareAndSet(std::atomic<T>& atomic
		, T expected_value, const U& value
		, const std::memory_order& succ_order = std::memory_order_release
		, const std::memory_order& fail_order = std::memory_order_acquire)
		noexcept
	{
		if (!atomic.compare_exchange_strong(expected_value, value, succ_order, fail_order))
		{
			// expected_value == value;
			return atomic.compare_exchange_strong(expected_value, value, succ_order, fail_order);
		}
		else
		{
			return true;
		}
	}

	template<std::copyable T, std::convertible_to<T> U>
	inline
		bool
		CompareAndSet(volatile std::atomic<T>& atomic
		, T expected_value, const U& value
		, const std::memory_order& succ_order = std::memory_order_release
		, const std::memory_order& fail_order = std::memory_order_acquire)
		noexcept
	{
		if (!atomic.compare_exchange_strong(expected_value, value, succ_order, fail_order))
		{
			// expected_value == value;
			return atomic.compare_exchange_strong(expected_value, value, succ_order, fail_order);
		}
		else
		{
			return true;
		}
	}

	template<std::copyable T, std::convertible_to<T> U>
	inline
		bool
		CompareAndSet(std::atomic<T>* atomic
		, T expected_value, const U& value
		, const std::memory_order& succ_order = std::memory_order_release
		, const std::memory_order& fail_order = std::memory_order_acquire)
		noexcept
	{
		if (!atomic->compare_exchange_strong(expected_value, value, succ_order, fail_order))
		{
			// expected_value == value;
			return atomic->compare_exchange_strong(expected_value, value, succ_order, fail_order);
		}
		else
		{
			return true;
		}
	}

	template<std::copyable T, std::convertible_to<T> U>
	inline
		bool
		CompareAndSet(volatile std::atomic<T>* atomic
		, T expected_value, const U& value
		, const std::memory_order& succ_order = std::memory_order_release
		, const std::memory_order& fail_order = std::memory_order_acquire)
		noexcept
	{
		if (!atomic->compare_exchange_strong(expected_value, value, succ_order, fail_order))
		{
			// expected_value == value;
			return atomic->compare_exchange_strong(expected_value, value, succ_order, fail_order);
		}
		else
		{
			return true;
		}
	}
}
