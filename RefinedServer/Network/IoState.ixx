export module Net.IoState;
import Utility;
import Utility.Constraints;
export import Utility.Identity;

export namespace net::io
{
	namespace tags
	{
		enum class io_success {};
		enum class io_failure {};
		enum class io_defered {};
	}

	namespace detail
	{
		template<typename T> // T: Success Handle
		struct success : util::type_identity<T>
		{};

		template<>
		struct success<void>
		{
			using type = tags::io_success;
		};

		template<typename E> // E: Error
		struct failure : util::type_identity<E>
		{};

		template<>
		struct failure<void>
		{
			using type = tags::io_failure;
		};

		template<typename C> // C: Cause of Defer
		struct defered : util::type_identity<C>
		{};

		template<>
		struct defered<void>
		{
			using type = tags::io_defered;
		};
	}

	template<typename T> // T: Success Handle
	using success_t = typename detail::template success<T>::type;
	template<typename E> // E: Error
	using failure_t = typename detail::template failure<E>::type;
	template<typename C> // C: Cause of Defer
	using defered_t = typename detail::template defered<C>::type;

	using just_success_t = success_t<void>;
	using just_failure_t = failure_t<void>;
	using just_defered_t = defered_t<void>;

	inline constexpr just_success_t success{ };
	inline constexpr just_failure_t failure{ };
	inline constexpr just_defered_t defered{ };
}
