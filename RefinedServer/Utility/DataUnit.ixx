module;
#include <memory>

export module Utility.Datagram.Unit;
import Utility;
import Utility.Meta;
import Utility.Constraints;
import Utility.Memory;
import Utility.Array;
import Utility.Datagram.Serialization;

export namespace util::datagram
{
	template<typename T>
	class DataUnit;

	template<>
	class DataUnit<void>
	{
	public:
		constexpr DataUnit() noexcept = default;
		constexpr DataUnit(const DataUnit& other) noexcept = default;
		constexpr DataUnit(DataUnit&& other) noexcept = default;
		constexpr DataUnit& operator=(const DataUnit& other) noexcept = default;
		constexpr DataUnit& operator=(DataUnit&& other) noexcept = default;
		constexpr ~DataUnit() noexcept = default;
	};

	template<typename T>
	class DataUnit
	{
	public:
		static inline constexpr size_t size = sizeof(T);

		constexpr DataUnit() noexcept = default;
		constexpr DataUnit(const DataUnit& other) noexcept = default;
		constexpr DataUnit(DataUnit&& other) noexcept = default;
		constexpr DataUnit& operator=(const DataUnit& other) noexcept = default;
		constexpr DataUnit& operator=(DataUnit&& other) noexcept = default;
		constexpr ~DataUnit() noexcept = default;

		explicit constexpr DataUnit(const T& data)
			noexcept(noexcept(util::Serialize(declval<const T&>()))) requires copyable<T>
		{
			static_assert(serializables<T>, "T must be serializable");

			const Array<char, size> result = util::Serialize(data);
			result.CopyTo(myBuffer);
		}

		explicit constexpr DataUnit(T&& data)
			noexcept(noexcept(util::Serialize(declval<T&&>()))) requires movable<T>
		{
			static_assert(serializables<T>, "T must be serializable");

			const Array<char, size> result = util::Serialize(static_cast<T&&>(data));
			result.CopyTo(myBuffer);
		}

		char myBuffer[size]{};
	};

	template<typename T>
	DataUnit(T) -> DataUnit<T>;
}

#pragma warning(push, 1)
namespace util::test
{
#if true
	constexpr void test_dataunit_stack() noexcept
	{
		constexpr datagram::DataUnit<int> data0{ 5 };
		constexpr datagram::DataUnit<int> data1{};
		constexpr datagram::DataUnit<int> data2{ 0 };

		static_assert(data0.myBuffer[3] == 5);

		//static_assert((data3.myBuffer)[3] == 5);
	}
#endif
}
#pragma warning(pop)