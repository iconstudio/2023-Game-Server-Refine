module;
#include <utility>
#include <initializer_list>
#include <optional>
#include <memory>
#include <string>
#include <string_view>
export module Utility;

export extern "C++" namespace util
{
	using ::std::type_identity;
	using ::std::type_identity_t;
	using ::std::identity;
	using ::std::declval;

	using ::std::invoke_result;
	using ::std::invoke_result_t;
	using ::std::invoke;

	using ::std::min;
	using ::std::max;

	using ::std::compare_three_way;
	using ::std::compare_three_way_result;
	using ::std::compare_three_way_result_t;
	using ::std::weak_ordering;
	using ::std::weak_order;
	using ::std::partial_ordering;
	using ::std::partial_order;
	using ::std::strong_ordering;
	using ::std::strong_order;

	using ::std::underlying_type;
	using ::std::underlying_type_t;
	using ::std::to_underlying;

	using ::std::piecewise_construct_t;
	using ::std::piecewise_construct;
	using ::std::optional;
	using ::std::bad_optional_access;
	using ::std::nullopt_t;
	using ::std::nullopt;
	using ::std::make_optional;

	using ::std::initializer_list;

	using ::std::default_delete;
	using ::std::destroying_delete_t;
	using ::std::destroying_delete;

	using ::std::move;
	using ::std::forward;
	using ::std::move_if_noexcept;
	//using ::std::addressof;

	template <class T>
	[[nodiscard]] __forceinline constexpr T* addressof(T& _Val) noexcept
	{
		return __builtin_addressof(_Val);
	}

	template <class T>
	const T* addressof(const T&&) = delete;

	using ::std::is_constant_evaluated;
	using ::std::unreachable;

	using ::std::copy;
	using ::std::copy_n;
	using ::std::uninitialized_copy;
	using ::std::uninitialized_copy_n;
	using ::std::memcpy;
	using ::std::memmove;
	using ::memcpy_s;
	using ::memmove_s;

	using ::std::begin;
	using ::std::end;
	using ::std::cbegin;
	using ::std::cend;
	using ::std::size;
	using ::std::ssize;
}
