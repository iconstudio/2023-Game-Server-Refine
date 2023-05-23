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
		struct Watcher
		{
			constexpr Watcher(char*& buffer, const size_t& size) noexcept
				: myBuffer(buffer), mySize(size)
			{}

			constexpr ~Watcher() noexcept(trivials<T>)
			{
				if (isEnabled)
				{
					delete[] myBuffer;
				}
			}

			constexpr void Disable() noexcept
			{
				isEnabled = false;
			}

			char*& myBuffer;
			size_t mySize = 0;
			bool isEnabled = true;
		};

	public:
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
			if (myBuffer)
			{
				Copy(other.myBuffer, other.myCapacity);
			}
			else
			{
				Allocate(other.myCapacity);
				myCapacity = other.myCapacity;

				Copy(other.myBuffer, other.myCapacity);
			}
		}

		constexpr DataUnit& operator=(DataUnit&& other) noexcept(trivials<T>)
		{
			if (myBuffer)
			{
				Move(move(other.myBuffer), other.myCapacity);
			}
			else
			{
				Allocate(other.myCapacity);
				myCapacity = other.myCapacity;

				Move(move(other.myBuffer), other.myCapacity);
			}

			other.Tidy();
		}

		constexpr ~DataUnit() noexcept(trivials<T>)
		{
			Tidy();
		}

		explicit constexpr DataUnit(const T& data)
			requires copyable<T>
		{
			static_assert(serializables<T>, "T must be serializable");

			const auto result = util::Serialize(data);
			result.CopyTo(myBuffer, myCapacity);
		}

		explicit constexpr DataUnit(T&& data)
			requires movable<T>
		{
			static_assert(serializables<T>, "T must be serializable");

			const auto result = util::Serialize(static_cast<T&&>(data));
			result.CopyTo(myBuffer, myCapacity);
		}

	private:
		constexpr void TryAllocate(const size_t& new_capacity) noexcept(trivials<T>)
		{
			if (!myBuffer)
			{
				Allocate(new_capacity);
			}
			else if (myCapacity < new_capacity)
			{
				Tidy();
				Allocate(new_capacity);
			}
		}

		constexpr char* Allocate(const size_t& new_capacity) noexcept(trivials<T>)
		{
			myBuffer = new char[new_capacity];
			myCapacity = new_capacity;
			return myBuffer;
		}

		constexpr void Copy(const char* const& buffer, const size_t& from_capacity)
		{
			if (std::is_constant_evaluated())
			{
				std::copy(buffer, buffer + from_capacity, myBuffer);
			}
			else
			{
				::memcpy_s(myBuffer, myCapacity, buffer, from_capacity);
			}
		}

		constexpr void Move(char* (&& buffer), const size_t& from_capacity)
		{
			if (std::is_constant_evaluated())
			{
				std::move(buffer, buffer + from_capacity, myBuffer);
			}
			else
			{
				::memcpy_s(myBuffer, myCapacity, buffer, from_capacity);
			}
		}

		constexpr void Tidy() noexcept
		{
			if (myBuffer)
			{
				delete[] myBuffer;
				myBuffer = nullptr;
				myCapacity = 0;
			}
		}

		char* myBuffer = nullptr;
		volatile size_t myCapacity = 0;
	};

	template<bool Heap, typename T>
	DataUnit(T) -> DataUnit<Heap, T>;
}

#pragma warning(push, 1)
namespace util::test
{
#if true
	void test_dataunit_stack()
	{
		constexpr datagram::DataUnit<false, int> data0{ 5 };
		constexpr datagram::DataUnit<false, int> data1{};
		constexpr datagram::DataUnit<false, int> data2{ 0 };

		static_assert(data0.myBuffer[3] == 5);
	}
#endif
}
#pragma warning(pop)