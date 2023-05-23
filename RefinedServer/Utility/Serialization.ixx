module;
#include "Serialization.inl"

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
}

export template<> struct
util::Serializer<bool>
{
	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(const bool& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(bool&& value) noexcept
	{
		return serialization::Serialize(static_cast<bool&&>(value));
	}
};

export template<> struct
util::Serializer<char>
{
	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(const char& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(char&& value) noexcept
	{
		return serialization::Serialize(static_cast<char&&>(value));
	}
};

export template<> struct
util::Serializer<unsigned char>
{
	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(const unsigned char& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(unsigned char&& value) noexcept
	{
		return serialization::Serialize(static_cast<unsigned char&&>(value));
	}
};

export template<> struct
util::Serializer<char8_t>
{
	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(const char8_t& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 1> Parse(char8_t&& value) noexcept
	{
		return serialization::Serialize(static_cast<char8_t&&>(value));
	}
};

export template<> struct
util::Serializer<short>
{
	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(const short& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(short&& value) noexcept
	{
		return serialization::Serialize(static_cast<short&&>(value));
	}
};

export template<> struct
util::Serializer<unsigned short>
{
	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(const unsigned short& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(unsigned short&& value) noexcept
	{
		return serialization::Serialize(static_cast<unsigned short&&>(value));
	}
};

export template<> struct
util::Serializer<char16_t>
{
	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(const char16_t& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(char16_t&& value) noexcept
	{
		return serialization::Serialize(static_cast<char16_t&&>(value));
	}
};

export template<> struct
util::Serializer<wchar_t>
{
	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(const wchar_t& value) noexcept
	{
		return serialization::Serialize(value);
	}

	[[nodiscard]]
	static constexpr util::Array<char, 2> Parse(wchar_t&& value) noexcept
	{
		return serialization::Serialize(static_cast<wchar_t&&>(value));
	}
};

export template<> struct
util::Serializer<int>
{
	[[nodiscard]]
	static constexpr util::Array<char, 4> Parse(const int& value) noexcept
	{
		return serialization::Serialize(value);
	}

	template<size_t Offset, size_t Length>
	static constexpr void ParseTo(const int& value, util::Array<char, Length>& dest)
		noexcept(Offset + sizeof(int) <= Length)
	{
		if constexpr (Length < Offset + sizeof(int))
		{
			static_assert(util::always_false<util::Array<char, Length>>, "The buffer is too small to contain an int.");
		}

		const util::Array<char, 4> result = Parse(value);
		dest[Offset] = result[0];
		dest[Offset + 1] = result[1];
		dest[Offset + 2] = result[2];
		dest[Offset + 3] = result[3];
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

		constexpr test_struct test1{ 1, 2, 3 };
		constexpr auto rs1 = util::Serializer<test_struct>::Parse(test1);
		static_assert(rs1[0] == 0);

		constexpr test_struct test2{ 302830631, 68212049, 1205071 };
		constexpr auto rs2 = util::Serializer<test_struct>::Parse(test2);

		constexpr test_struct test3{ 255, 256, 512 };
		constexpr auto rs3 = util::Serializer<test_struct>::Parse(test3);

		constexpr auto test_str1 = serialization::Serialize("¤¡¤¤¤§");
		constexpr auto test_str2 = serialization::Serialize(L"¤¡");
		constexpr auto test_str3 = serialization::Serialize("¤·³Ä¤¡¤µ¹õÁ¹¿î¤·¤©¿õ³¯¤§¤·À¸¤Àµå¤À¤ÑÃ÷¾Ö¤ÌÃ¤¾ßÃß¾Ö¤Ì¤º´©¤Å¤Ì¤º¤§Àú¤Ã¤¸¤§");
		constexpr auto test_str4 = serialization::Serialize(L"¤·³Ä¤¡¤µ¹õÁ¹¿î¤·¤©¿õ³¯¤§¤·À¸¤Àµå¤À¤ÑÃ÷¾Ö¤ÌÃ¤¾ßÃß¾Ö¤Ì¤º´©¤Å¤Ì¤º¤§Àú¤Ã¤¸¤§");

	}
#endif // false
}
#pragma warning(pop)
