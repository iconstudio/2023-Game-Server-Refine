module;
#include <cstdio>
#include <format>

export module Utility.Print;
import Utility.Concurrency.Locks;
import Utility;

#define FORCEIN __forceinline

#if 1935 <= _MSC_VER
template<typename Char, typename... Args>
using basic_format_string = std::basic_format_string<Char, Args...>;
template<typename Char>
using format_string_view = util::basic_string_view<Char>;

template<typename... Args>
using format_string = std::format_string<Args...>;
template<typename... Args>
using format_wstring = std::wformat_string<Args...>;

template<typename Char, typename... Args>
static
FORCEIN constexpr
format_string_view<Char>
internal_fmt(basic_format_string<Char, Args...> fmt) noexcept
{
	return fmt.get();
}
#else
template<typename... Args>
using format_string = std::_Fmt_string<Args...>;
template<typename... Args>
using format_wstring = std::_Fmt_wstring<Args...>;

#define internal_fmt(fmt) fmt._Str
#endif

using std::FILE;
template<typename = char>
FORCEIN void internal_vprint(FILE* stream, util::string_view fmt, std::format_args&& args) noexcept;

template<typename = char>
FORCEIN void internal_vprintln(FILE* stream, util::string_view fmt, std::format_args&& args) noexcept;

template<typename = wchar_t>
FORCEIN void internal_wprint(FILE* stream, util::wstring_view fmt, std::wformat_args&& args) noexcept;

template<typename = wchar_t>
FORCEIN void internal_wprintln(FILE* stream, util::wstring_view fmt, std::wformat_args&& args) noexcept;

extern "C++" template<bool = false, typename Char, typename ...Args>
static FORCEIN
void
internal_print_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept;

extern "C++" template<bool = false, typename Char, typename ...Args>
static FORCEIN
void
internal_println_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept;

extern "C++" template<bool = true, typename Char, typename ...Args>
static FORCEIN void
internal_sync_print_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept;

extern "C++" template<bool = true, typename Char, typename ...Args>
static FORCEIN void
internal_sync_println_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept;

extern util::mutex syncedIo{};

export namespace util
{
	namespace print
	{
		inline void Initialize() noexcept
		{
			//syncedIo = new util::mutex{};
		}
	}

	using ::std::FILE;

	template<bool = false, typename ...Args>
	FORCEIN
		void Print(FILE* stream, format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_print_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Println(FILE* stream, format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_println_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Print(FILE* stream, format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_print_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Println(FILE* stream, format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_println_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Print(format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_print_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Println(format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_println_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Print(format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_print_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = false, typename ...Args>
	FORCEIN
		void Println(format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_println_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintSynced(FILE* stream, format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_print_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintlnSynced(FILE* stream, format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_println_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintSynced(FILE* stream, format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_print_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintlnSynced(FILE* stream, format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_println_format(stream, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintSynced(format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_print_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintlnSynced(format_string<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_println_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintSynced(format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_print_format(stdout, fmt, std::forward<Args>(args)...);
	}

	template<bool = true, typename... Args>
	FORCEIN
		void PrintlnSynced(format_wstring<Args...> fmt, Args&& ...args) noexcept
	{
		return internal_sync_println_format(stdout, fmt, std::forward<Args>(args)...);
	}

	namespace debug
	{
		template<typename... Args>
		inline void Print(FILE* stream, format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::Print(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Print(format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::Print(fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Println(FILE* stream, format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::Println(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Println(format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return Println(stdout, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Print(FILE* stream, format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::Print(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Print(format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return Print(stdout, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Println(FILE* stream, format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::Println(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void Println(format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return Println(stdout, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintSynced(FILE* stream, format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::PrintSynced(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintSynced(format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::PrintSynced(fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintlnSynced(FILE* stream, format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::PrintlnSynced(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintlnSynced(format_string<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return PrintlnSynced(stdout, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintSynced(FILE* stream, format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::PrintSynced(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintSynced(format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return PrintSynced(stdout, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintlnSynced(FILE* stream, format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return util::PrintlnSynced(stream, fmt, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		inline void PrintlnSynced(format_wstring<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			return PrintlnSynced(stdout, fmt, std::forward<Args>(args)...);
#endif
		}
	}
}

namespace util
{
	static void test_print()
	{
		Print("Hello, world!\n", 0);
		Print("Hello, {}!\n", "world");

		Print("Hello, {}!\n", string("world"));
		Print("Hello, {}!\n", string_view("world"));
		Print(L"Hello, {}!\n", wstring(L"world"));
		Print(L"Hello, {}!\n", wstring_view(L"world"));

		Println("Hello, world!");
		Println("Hello, {}!", "world");
		Println("Hello, {}!", string("world"));
		Println("Hello, {}!", string_view("world"));
		Println(L"Hello, {}!", wstring(L"world"));
		Println(L"Hello, {}!", wstring_view(L"world"));

		PrintSynced("Hello, world!\n");
		PrintSynced("Hello, {}!\n", "world");
		PrintSynced("Hello, {}!\n", string("world"));
		PrintSynced("Hello, {}!\n", string_view("world"));
		PrintSynced(L"Hello, {}!\n", wstring(L"world"));
		PrintSynced(L"Hello, World!\n");
	}
}

extern "C++" template<bool, typename Char, typename ...Args>
static FORCEIN void
internal_print_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept
{
	if constexpr (0 == sizeof...(Args))
	{
		if constexpr (std::is_same_v<Char, char>)
		{
			return std::fputs(fmt.get().data(), stream);
		}
		else if constexpr (std::is_same_v<Char, wchar_t>)
		{
			return std::fputws(fmt.get().data(), stream);
		}
	}
	else if constexpr (std::is_same_v<Char, char>)
	{
		return internal_vprint(stream, fmt.get(), std::make_format_args(std::forward<Args>(args)...));
	}
	else if constexpr (std::is_same_v<Char, wchar_t>)
	{
		return internal_wprint(stream, fmt.get(), std::make_wformat_args(std::forward<Args>(args)...));
	}
}

extern "C++" template<bool, typename Char, typename ...Args>
static FORCEIN
void
internal_println_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept
{
	if constexpr (0 == sizeof...(Args))
	{
		if constexpr (std::is_same_v<Char, char>)
		{
			std::string_view str = fmt.get();
			const std::string& buffer{ str.cbegin(), str.cend() };

			std::fputs((buffer + '\n').data(), stream);
		}
		else if constexpr (std::is_same_v<Char, wchar_t>)
		{
			std::wstring_view str = fmt.get();
			const std::wstring& buffer{ str.cbegin(), str.cend() };

			std::fputws((buffer + L'\n').data(), stream);
		}
	}
	else if constexpr (std::is_same_v<Char, char>)
	{
		return internal_vprintln(stream, fmt.get(), std::make_format_args(std::forward<Args>(args)...));
	}
	else if constexpr (std::is_same_v<Char, wchar_t>)
	{
		return internal_wprintln(stream, fmt.get(), std::make_wformat_args(std::forward<Args>(args)...));
	}
}

extern "C++" template<bool, typename Char, typename ...Args>
static FORCEIN
void
internal_sync_print_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args&& ...args)
noexcept
{
	if constexpr (0 == sizeof...(Args))
	{
		if constexpr (std::is_same_v<Char, char>)
		{
			std::string_view buffer = fmt.get();

			util::lock_guard guard{ syncedIo };
			std::fputs(buffer.data(), stream);
		}
		else if constexpr (std::is_same_v<Char, wchar_t>)
		{
			std::wstring_view buffer = fmt.get();

			util::lock_guard guard{ syncedIo };
			std::fputws(buffer.data(), stream);
		}
	}
	else if constexpr (std::is_same_v<Char, char>)
	{
		std::string_view buff = fmt.get();
		std::format_args pack = std::make_format_args(std::forward<Args>(args)...);

		util::lock_guard guard{ syncedIo };
		return internal_vprint(stream, buff, std::move(pack));
	}
	else if constexpr (std::is_same_v<Char, wchar_t>)
	{
		std::wstring_view buff = fmt.get();
		std::wformat_args pack = std::make_wformat_args(std::forward<Args>(args)...);

		util::lock_guard guard{ syncedIo };
		return internal_wprint(stream, buff, std::move(pack));
	}
}

extern "C++" template<bool, typename Char, typename ...Args>
static FORCEIN
void
internal_sync_println_format(FILE * stream, const basic_format_string<Char, Args...>&fmt, Args && ...args) noexcept
{
	if constexpr (0 == sizeof...(Args))
	{
		if constexpr (std::is_same_v<Char, char>)
		{
			std::string_view fwd = fmt.get();
			const std::string& buffer = std::string{ fwd.cbegin(), fwd.cend() } + '\n';

			util::lock_guard guard{ syncedIo };
			std::fputs(buffer.data(), stream);
		}
		else if constexpr (std::is_same_v<Char, wchar_t>)
		{
			std::wstring_view fwd = fmt.get();
			const std::wstring& buffer = std::wstring{ fwd.cbegin(), fwd.cend() } + L'\n';

			util::lock_guard guard{ syncedIo };
			std::fputws(buffer.data(), stream);
		}
	}
	else if constexpr (std::is_same_v<Char, char>)
	{
		std::string_view buff = fmt.get();
		std::format_args pack = std::make_format_args(std::forward<Args>(args)...);

		util::lock_guard guard{ syncedIo };
		return internal_vprintln(stream, buff, std::move(pack));
	}
	else if constexpr (std::is_same_v<Char, wchar_t>)
	{
		std::wstring_view buff = fmt.get();
		std::wformat_args pack = std::make_wformat_args(std::forward<Args>(args)...);

		util::lock_guard guard{ syncedIo };
		return internal_wprintln(stream, buff, std::move(pack));
	}
}

template<typename>
FORCEIN
void
internal_vprint(FILE* stream, std::string_view fmt, std::format_args&& args) noexcept
{
	std::fputs(std::vformat(fmt, std::move(args)).data(), stream);
}

template<typename>
FORCEIN
void
internal_vprintln(FILE* stream, std::string_view fmt, std::format_args&& args) noexcept
{
	std::fputs((std::vformat(fmt, std::move(args)) + '\n').data(), stream);
}

template<typename>
FORCEIN
void
internal_wprint(FILE* stream, std::wstring_view fmt, std::wformat_args&& args) noexcept
{
	std::fputws(std::vformat(fmt, std::move(args)).data(), stream);
}

template<typename>
FORCEIN
void
internal_wprintln(FILE* stream, std::wstring_view fmt, std::wformat_args&& args) noexcept
{
	std::fputws((std::vformat(fmt, std::move(args)) + L'\n').data(), stream);
}