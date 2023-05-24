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

	template<notvoids Fty, notvoids... Rty>
	class [[nodiscard]] DataPacker<Fty, Rty...>
	{
	public:
		static inline constexpr size_t myLength = 1 + sizeof...(Rty);
		static inline constexpr size_t mySize = meta::bsize_v<Fty, Rty...>;

		constexpr DataPacker() noexcept = default;
		constexpr DataPacker(const DataPacker& other) noexcept = default;
		constexpr DataPacker(DataPacker&& other) noexcept = default;
		constexpr DataPacker& operator=(const DataPacker& other) noexcept = default;
		constexpr DataPacker& operator=(DataPacker&& other) noexcept = default;

		constexpr DataPacker(const Fty& first) noexcept
			: myData(first)
		{}

		constexpr DataPacker(Fty&& first) noexcept
			: myData(static_cast<Fty&&>(first))
		{}

		template<typename U, typename... Args>
			requires argument_available<Args...>
		explicit
			constexpr DataPacker(U&& first, Args&&... args) noexcept
			: myData(forward<U>(first))
			, nextNode(forward<Args>(args)...)
		{}

		constexpr ~DataPacker() noexcept
		{}

		DataUnit<Fty> myData{};
		DataPacker<Rty...> nextNode;
	};

	template<>
	class DataPacker<>
	{
	public:
		static inline constexpr size_t myLength = 0;
		static inline constexpr size_t mySize = 0;

		constexpr DataPacker() noexcept = default;
		constexpr ~DataPacker() noexcept = default;

		DataUnit<void> myData{};
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
		static_assert(test_pk5.nextNode.nextNode.myData.size == 4);
	}
#endif // true
}
#pragma warning(pop)
