module;
#include "Serialiazation.inl"

export module Utility.Serialization;
import Utility;
import Utility.String;
import Utility.Array;
import Utility.Constraints;

export namespace util
{
	template<typename T>
	struct [[nodiscard]] Serializer
	{
		Serializer() = delete;
		~Serializer() = delete;
	};

	template<typename T, size_t Length>
	struct [[nodiscard]] Serializer<T[Length]>
	{
		Serializer() = delete;
		~Serializer() = delete;
	};

	template<typename T>
	concept Serializable = std::is_object_v<T>
		&& requires(const T & value, Serializer<T>&parser)
	{
		parser.Parse(value);
	};

	template<typename T>
	struct [[nodiscard]] Deserializer
	{
		Deserializer() = delete;
		~Deserializer() = delete;
	};
}

export template<>
struct util::Serializer<int>
{
	[[nodiscard]]
	static constexpr util::Array<char, 4> Parse(const int& value) noexcept
	{
		const unsigned int safe_value = static_cast<unsigned int>(value);

		util::Array<char, 4> result{};
		result[0] = (safe_value >> 24) & 0xFF;
		result[1] = (safe_value >> 16) & 0xFF;
		result[2] = (safe_value >> 8) & 0xFF;
		result[3] = safe_value & 0xFF;

		return result;
	}

	template<size_t Offset, size_t Length>
	static constexpr void ParseTo(const int& value, util::Array<char, Length>& dest)
		noexcept(Offset + sizeof(int) <= Length)
	{
		if constexpr (Length < Offset + sizeof(int))
		{
			static_assert(util::always_false<util::Array<char, Length>>, "The buffer is too small to contain an int.");
		}

		const auto result = Parse(value);
		dest[Offset] = result[0];
		dest[Offset + 1] = result[1];
		dest[Offset + 2] = result[2];
		dest[Offset + 3] = result[3];
	}
};

export template<>
struct util::Deserializer<int>
{
	template<size_t Offset, size_t Length>
	[[nodiscard]]
	static constexpr int Parse(const util::Array<char, Length>& buffer)
		noexcept
	{
		if constexpr (Length < Offset + sizeof(int))
		{
			static_assert(util::always_false<util::Array<char, Length>>, "The buffer is too small to contain an int.");
		}

		const unsigned int result = (static_cast<unsigned>(buffer[Offset]) << 24)
			| (static_cast<unsigned>(buffer[Offset + 1]) << 16)
			| (static_cast<unsigned>(buffer[Offset + 2]) << 8)
			| static_cast<unsigned>(buffer[Offset + 3]);

		return static_cast<int>(result);
	}

	template<size_t Length>
	[[nodiscard]]
	static constexpr int Parse(const char(&buffer)[Length])
		noexcept(sizeof(int) <= Length)
	{
		if constexpr (Length < sizeof(int))
		{
			static_assert(util::always_false<util::Array<char, Length>>, "The buffer is too small to contain an int.");
		}

		const unsigned int result = (static_cast<unsigned>(buffer[0]) << 24)
			| (static_cast<unsigned>(buffer[1]) << 16)
			| (static_cast<unsigned>(buffer[2]) << 8)
			| static_cast<unsigned>(buffer[3]);

		return static_cast<int>(result);
	}

	[[nodiscard]]
	static constexpr int Parse(const char* buffer, const size_t& length)
	{
		if (length < sizeof(int))
		{
			throw "The buffer is too small to contain an int.";
		}

		const unsigned int result = (static_cast<unsigned>(buffer[0]) << 24)
			| (static_cast<unsigned>(buffer[1]) << 16)
			| (static_cast<unsigned>(buffer[2]) << 8)
			| static_cast<unsigned>(buffer[3]);

		return static_cast<int>(result);
	}
};

#pragma warning(push, 0)
namespace util::test
{
#if true
	struct test_struct
	{
		int a;
		int b;
		int c;
	};
}

template<>
struct util::Serializer<util::test::test_struct>
{
	[[nodiscard]]
	static constexpr util::Array<char, 12> Parse(const util::test::test_struct& value) noexcept
	{
		util::Array<char, 12> result{};

		util::Serializer<int>::ParseTo<0>(value.a, result);
		util::Serializer<int>::ParseTo<4>(value.b, result);
		util::Serializer<int>::ParseTo<8>(value.c, result);

		return result;
	}
};

namespace util::test
{
	void test_serializer() noexcept
	{
		static_assert(util::Serializable<test_struct>);

		constexpr test_struct test{ 1, 2, 3 };

		constexpr auto result = util::Serializer<test_struct>::Parse(test);

		static_assert(result[0] == 0);
	}
#endif // false
}
#pragma warning(pop)
