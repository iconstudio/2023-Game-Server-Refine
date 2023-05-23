export module Utility.Datagram.DataUnit;
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
		constexpr DataUnit() noexcept = default;
		constexpr DataUnit(const DataUnit& other) noexcept = default;
		constexpr DataUnit(DataUnit&& other) noexcept = default;
		constexpr DataUnit& operator=(const DataUnit& other) noexcept = default;
		constexpr DataUnit& operator=(DataUnit&& other) noexcept = default;
		constexpr ~DataUnit() noexcept = default;

		explicit constexpr DataUnit(const T& data)
			noexcept requires(trivials<T>)
		{
			static_assert(serializables<T>, "T must be serializable");

			const Array<char, sizeof(T)> result = util::Serialize(data);
			result.CopyTo(myBuffer);
		}

		explicit constexpr DataUnit(T&& data)
			noexcept requires(trivials<T>)
		{
			static_assert(serializables<T>, "T must be serializable");

			const Array<char, sizeof(T)> result = util::Serialize(static_cast<T&&>(data));
			result.CopyTo(myBuffer);
		}

		explicit inline DataUnit(const T& data)
			noexcept(noexcept(util::Serialize(declval<const T&>()))) requires(!trivials<T>)
		{
			static_assert(serializables<T>, "T must be serializable");

			const auto result = util::Serialize(data);
			result.CopyTo(myBuffer);
		}

		explicit inline DataUnit(T&& data)
			noexcept(noexcept(util::Serialize(declval<T&&>()))) requires(!trivials<T>)
		{
			static_assert(serializables<T>, "T must be serializable");

			const auto result = util::Serialize(static_cast<T&&>(data));
			result.CopyTo(myBuffer);
		}

		char myBuffer[sizeof(T)]{};
	};


	template<bool Heap, typename T>
	DataUnit(T) -> DataUnit<Heap, T>;
}
