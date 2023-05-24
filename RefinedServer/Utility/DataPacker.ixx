export module Utility.Datagram.Packer;
import Utility;
import Utility.Meta;
import Utility.Constraints;
import Utility.Memory;
import Utility.Array;
import Utility.Union;
import Utility.Datagram.Unit;
import Utility.Datagram.Serialization;

export namespace util::datagram
{
	template<bool Heap, typename... Ts>
	class [[nodiscard]] DataPacker;

	template<typename... Ts>
	class [[nodiscard]] DataPacker;

	template<>
	class DataPacker<>
	{
	public:
		static inline constexpr size_t myLength = sizeof...(Ts);
		static inline constexpr size_t mySize = meta::bsize_v<Ts...>;

		constexpr DataPacker() noexcept = default;
		constexpr ~DataPacker() noexcept = default;

		template<typename Arg>
		constexpr DataPacker(Arg&&) noexcept
		{}

		DataUnit<void> myData{};
	};

	template<notvoids Fty, notvoids... Rty>
	class [[nodiscard]] DataPacker<Fty, Rty...>
	{
	public:
		constexpr DataPacker() noexcept = default;

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

	template<bool Heap, typename... Ts>
	DataPacker(Ts...) -> DataPacker<Heap, Ts...>;
}

#pragma warning(push, 1)
namespace util::test
{
#if true
	void test_datapacker()
	{
		constexpr DataPacker test_pk1{};
		constexpr DataPacker<int> test_pk2{};
		constexpr DataPacker test_pk3{ 5000 };
		constexpr DataPacker<int, long, float, short, unsigned char, unsigned, bool> test_pk4{ 1230, 40L };

	}
#endif // true
}
#pragma warning(pop)
