export module Utility.String.Literal;
import <string_view>;

export namespace util
{
	template <typename Char, size_t N>
	struct basic_fixed_string
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval basic_fixed_string(const Char* const& buffer) noexcept
		{
			for (size_t i = 0; i < N - 1; ++i)
			{
				const Char& elem = buffer[i];
				intBuffer[i] = elem;
			}
		}

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(intBuffer, N - 1);
		}

		Char intBuffer[N - 1];
	};

	template <typename Char>
	struct basic_fixed_string<Char, 1>
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval basic_fixed_string(const Char* const& buffer) noexcept
		{
			intBuffer = buffer[0];
		}

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(intBuffer, 1);
		}

		Char intBuffer;
	};

	template <typename Char>
	struct basic_fixed_string<Char, 0>
	{
		consteval basic_fixed_string() noexcept = default;
		constexpr ~basic_fixed_string() noexcept = default;

		consteval operator std::basic_string_view<Char>() const noexcept
		{
			return std::basic_string_view<Char>(nullptr, 0);
		}
	};

	template <typename Char, size_t N>
	basic_fixed_string(const Char(&str)[N]) -> basic_fixed_string<Char, N>;

	template <size_t N>
	using fixed_string = basic_fixed_string<char, N>;

	template <size_t N>
	using fixed_u8string = basic_fixed_string<char8_t, N>;

	template <size_t N>
	using fixed_u16string = basic_fixed_string<char16_t, N>;

	template <size_t N>
	using fixed_u32string = basic_fixed_string<char32_t, N>;

	template <size_t N>
	using fixed_wstring = basic_fixed_string<wchar_t, N>;
}
