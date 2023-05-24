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
	template<bool Heap, typename T>
	class DataUnit;

	template<typename T>
	class DataUnit<false, T>
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
	class DataUnit<true, T>
	{
	public:
		static inline constexpr size_t size = sizeof(T);

		constexpr DataUnit() noexcept = default;

		constexpr DataUnit(const DataUnit& other) noexcept(trivials<T>)
		{
			this->operator=(other);
		}

		constexpr DataUnit(DataUnit&& other) noexcept(trivials<T>)
		{
			this->operator=(move(other));
		}

		constexpr DataUnit& operator=(const DataUnit& other) noexcept(trivials<T>)
		{
			TryAllocate();

			Copy(other.myBuffer.get(), size);

			other.Tidy();
		}

		constexpr DataUnit& operator=(DataUnit&& other) noexcept(trivials<T>)
		{
			TryAllocate();

			Move(other.myBuffer.get(), size);

			other.Tidy();
		}

		constexpr ~DataUnit() noexcept(trivials<T>)
		{
			Tidy();
		}

		explicit constexpr DataUnit(const T& data)
		{
			static_assert(serializables<T>, "T must be serializable");

			TryAllocate();

			const Array<char, size> result = util::Serialize(data);
			char* (ptr) = myBuffer.get();

			result.CopyTo(ptr, size);
		}

		unique_ptr<char[]> myBuffer = nullptr;

	private:
		constexpr void TryAllocate()
		{
			if (!myBuffer)
			{
				Allocate();
			}
		}

		constexpr void Allocate()
		{
			myBuffer = unique_ptr<char[]>(new char[size] {});
		}

		constexpr void Copy(const char* const& buffer, const size_t& from_capacity)
		{
			if (std::is_constant_evaluated())
			{
				std::copy(buffer, buffer + from_capacity, myBuffer.get());
			}
			else
			{
				::memcpy_s(myBuffer.get(), size, buffer, from_capacity);
			}
		}

		constexpr void Move(char* (&& buffer), const size_t& from_capacity)
		{
			if (std::is_constant_evaluated())
			{
				std::move(buffer, buffer + from_capacity, myBuffer.get());
			}
			else
			{
				::memcpy_s(myBuffer.get(), size, buffer, from_capacity);
			}
		}

		constexpr void Tidy() noexcept
		{
			if (myBuffer)
			{
				myBuffer.reset();
			}
		}
	};

	template<bool Heap, typename T>
	DataUnit(T) -> DataUnit<Heap, T>;
}

#pragma warning(push, 1)
namespace util::test
{
#if true
	constexpr datagram::DataUnit<true, int> test_dataunit_heap() noexcept
	{
		return datagram::DataUnit<true, int>{ 5 };
	}

	constexpr void test_dataunit_stack() noexcept
	{
		constexpr datagram::DataUnit<false, int> data0{ 5 };
		constexpr datagram::DataUnit<false, int> data1{};
		constexpr datagram::DataUnit<false, int> data2{ 0 };

		static_assert(data0.myBuffer[3] == 5);

		constexpr datagram::DataUnit<true, int> data3 = test_dataunit_heap();
		constexpr datagram::DataUnit<true, int> data4{};
		const datagram::DataUnit<true, int> data5{ 0 };

		//static_assert((data3.myBuffer)[3] == 5);
	}
#endif
}
#pragma warning(pop)