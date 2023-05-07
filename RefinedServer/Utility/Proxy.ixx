module;
#include <utility>

export module Utility.Monad.Proxy;
import Utility;
import Utility.Constraints;
import Utility.Meta;

export namespace util
{
	template<typename... Ts>
	class Proxy
	{
	public:
		using base_type = meta::MetaList<Ts...>;

		template<size_t Index>
		using element_type = std::tuple_element_t<Index, base_type>;
		template<size_t Index>
		using const_element_type = const element_type<Index>;
		template<size_t Index>
		using reference_type = element_type<Index>&;
		template<size_t Index>
		using const_reference_type = const element_type<Index>&;
		template<size_t Index>
		using rvalue_type = element_type<Index>&&;
		template<size_t Index>
		using const_rvalue_type = const element_type<Index>&&;

		constexpr Proxy() noexcept
		{}

		constexpr Proxy(nullopt_t) noexcept
		{}

	};
}