#pragma once
#include <type_traits>
#include <utility>

import Utility.Array;

namespace util::serialization
{
	/// <summary>
	/// �� ���� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(const bool& value) noexcept
	{
		return Array<char, 1>{ value };
	}

	/// <summary>
	/// �� ���� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(bool&& value) noexcept
	{
		return Array<char, 1>{ static_cast<char>(value) };
	}

	/// <summary>
	/// ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(const char& value) noexcept
	{
		return Array<char, 1>{ value };
	}

	/// <summary>
	/// ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(char&& value) noexcept
	{
		return Array<char, 1>{ static_cast<char&&>(value) };
	}

	/// <summary>
	/// ��ȣ ���� 8��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(const unsigned char& value) noexcept
	{
		return Array<char, 1>{ static_cast<char>(value) };
	}

	/// <summary>
	/// ��ȣ ���� 8��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(unsigned char&& value) noexcept
	{
		return Array<char, 1>{ static_cast<char>(value) };
	}

	/// <summary>
	/// UTF-8 ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(const char8_t& value) noexcept
	{
		return Array<char, 1>{ static_cast<char>(value) };
	}

	/// <summary>
	/// UTF-8 ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 1> Serialize(char8_t&& value) noexcept
	{
		return Array<char, 1>{ static_cast<char>(value) };
	}

	/// <summary>
	/// 16��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(const short& value) noexcept
	{
		return Serialize(static_cast<unsigned short>(value));
	}

	/// <summary>
	/// 16��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(short&& value) noexcept
	{
		return Serialize(static_cast<unsigned short&&>(value));
	}

	/// <summary>
	/// ��ȣ ���� 16��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(const unsigned short& value) noexcept
	{
		return Array<char, 2>
		{
			static_cast<char>((value >> 8) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// ��ȣ ���� 16��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(unsigned short&& value) noexcept
	{
		return Array<char, 2>
		{
			static_cast<char>((value >> 8) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// ���̵� ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(const wchar_t& value) noexcept
	{
		return Serialize(static_cast<unsigned short>(value));
	}

	/// <summary>
	/// UTF-16 ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 2> Serialize(const char16_t& value) noexcept
	{
		return Serialize(static_cast<unsigned short>(value));
	}

	/// <summary>
	/// 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(const int& value) noexcept
	{
		return Serialize(static_cast<unsigned int>(value));
	}

	/// <summary>
	/// 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(int&& value) noexcept
	{
		return Serialize(static_cast<unsigned int&&>(value));
	}

	/// <summary>
	/// ��ȣ ���� 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(const unsigned int& value) noexcept
	{
		return Array<char, 4>
		{
			static_cast<char>((value >> 24U) & 0xFF),
				static_cast<char>((value >> 16U) & 0xFF),
				static_cast<char>((value >> 8U) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// ��ȣ ���� 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(unsigned int&& value) noexcept
	{
		return Array<char, 4>
		{
			static_cast<char>((value >> 24U) & 0xFF),
				static_cast<char>((value >> 16U) & 0xFF),
				static_cast<char>((value >> 8U) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(const long& value) noexcept
	{
		return Serialize(static_cast<unsigned long>(value));
	}

	/// <summary>
	/// 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(long&& value) noexcept
	{
		return Serialize(static_cast<unsigned long&&>(value));
	}

	/// <summary>
	/// ��ȣ ���� 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(const unsigned long& value) noexcept
	{
		return Array<char, 4>
		{
			static_cast<char>((value >> 24UL) & 0xFF),
				static_cast<char>((value >> 16UL) & 0xFF),
				static_cast<char>((value >> 8UL) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// ��ȣ ���� 32��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(unsigned long&& value) noexcept
	{
		return Array<char, 4>
		{
			static_cast<char>((value >> 24UL) & 0xFF),
				static_cast<char>((value >> 16UL) & 0xFF),
				static_cast<char>((value >> 8UL) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}

	/// <summary>
	/// UTF-32 ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(const char32_t& value) noexcept
	{
		return Serialize(static_cast<unsigned>(value));
	}

	/// <summary>
	/// UTF-32 ���ڸ� ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 4> Serialize(char32_t&& value) noexcept
	{
		return Serialize(static_cast<unsigned&&>(value));
	}

	/// <summary>
	/// 64��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 8> Serialize(const long long& value) noexcept
	{
		return Serialize(static_cast<unsigned long long>(value));
	}

	/// <summary>
	/// ��ȣ ���� 64��Ʈ ������ ����ȭ�մϴ�.
	/// </summary>
	constexpr Array<char, 8> Serialize(const unsigned long long& value) noexcept
	{
		return Array<char, 8>
		{
			static_cast<char>((value >> 56ULL) & 0xFF),
				static_cast<char>((value >> 48ULL) & 0xFF),
				static_cast<char>((value >> 40ULL) & 0xFF),
				static_cast<char>((value >> 32ULL) & 0xFF),
				static_cast<char>((value >> 24ULL) & 0xFF),
				static_cast<char>((value >> 16ULL) & 0xFF),
				static_cast<char>((value >> 8ULL) & 0xFF),
				static_cast<char>((value) & 0xFF)
		};
	}
}
