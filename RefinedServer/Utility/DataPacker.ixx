module;
#include <tuple>

export module Utility.Datagram.Packer;
import Utility;
import Utility.Meta;
import Utility.Constraints;
import Utility.Memory;
import Utility.Array;
import Utility.Monad;
import Utility.Datagram.Unit;
import Utility.Datagram.Serialization;

export namespace util::datagram
{
	template<typename... Ts>
	class [[nodiscard]] DataPacker;
}

namespace util::detail
{
	template <size_t Index, typename Variant>
	struct get_node_at;

	template <size_t Index, typename T, typename... Ts>
	struct get_node_at<Index, datagram::DataPacker<T, Ts...>>
	{
		using type = typename get_node_at<Index - 1, datagram::DataPacker<Ts...>>::type;
	};

	template <typename T, typename... Ts>
	struct get_node_at<0, datagram::DataPacker<T, Ts...>>
	{
		using seq_type = datagram::DataPacker<T, Ts...>;
		using type = T;
	};
}

export namespace util::datagram
{
	template<notvoids Fty, notvoids... Rty>
	class [[nodiscard]] DataPacker<Fty, Rty...>
	{
		struct Enumerator
		{

		};

	public:
		using Pack = meta::MetaList<Fty, Rty...>;

		static inline constexpr size_t myLength = 1 + sizeof...(Rty);
		static inline constexpr size_t mySize = meta::byte_size_v<Fty, Rty...>;

		constexpr DataPacker(const DataPacker& other) noexcept = default;
		constexpr DataPacker(DataPacker&& other) noexcept = default;
		constexpr DataPacker& operator=(const DataPacker& other) noexcept = default;
		constexpr DataPacker& operator=(DataPacker&& other) noexcept = default;

		constexpr DataPacker() noexcept
		{}

		template<typename... Args>
		explicit constexpr DataPacker(Args&&... args)
			noexcept
		{
			static_assert(serializables<clean_t<Args>...>);

			Writes(std::forward_as_tuple(forward<Args>(args)...), std::index_sequence_for<Args...>{});
		}

		template<size_t Index, typename... Args>
		consteval size_t Summarize() noexcept
		{

		}

		template<typename... Args, size_t... Indices>
		constexpr void Writes(std::tuple<Args...>&& data, std::index_sequence<Indices...>)
			noexcept
		{
			size_t offset = 0;

		}

		template<typename T>
		constexpr void Write(T&& value, const size_t& offset)
		{
		}

		constexpr ~DataPacker() noexcept
		{}

		constexpr void Serialize(char* const& output, const size_t& out_length) const
		{

		}

		constexpr char* Serialize()
		{

		}

		char internalBuffer[mySize]{};
	};

	template<>
	class DataPacker<>
	{
	public:
		static inline constexpr size_t myLength = 0;
		static inline constexpr size_t mySize = 0;

		constexpr DataPacker() noexcept = default;
		constexpr ~DataPacker() noexcept = default;

		//[[msvc::no_unique_address]] DataUnit<void> myData{};
	};

	template<typename... Ts>
	DataPacker(Ts...) -> DataPacker<Ts...>;
}

#pragma warning(push, 1)
namespace util::test
{
#if true
	void test_datapacker()
	{
		constexpr datagram::DataPacker test_pk1{};

		constexpr datagram::DataPacker<int> test_pk2{};

		constexpr datagram::DataPacker test_pk3{ 5000 };

		constexpr datagram::DataPacker<int, long, float, short, unsigned char, unsigned, bool> test_pk4{};

		constexpr datagram::DataPacker<int, long, float, short, unsigned char, unsigned, bool> test_pk5{ 1230, 40L, 0.0174124983f };

		static_assert(test_pk5.myLength == 7);
		static_assert(test_pk5.mySize == sizeof(int) + sizeof(long) + sizeof(float) + sizeof(short) + sizeof(unsigned char) + sizeof(unsigned) + sizeof(bool));
		//static_assert(test_pk5.nextNode.nextNode.myData.size != 0.0174124983f);
		//static_assert(test_pk5.nextNode.nextNode.myData.size == 4);
	}
#endif // true
}
#pragma warning(pop)
