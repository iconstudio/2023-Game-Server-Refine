module;
#include <string>
#include <string_view>
export module Utility.String;

export namespace util
{
	using ::std::basic_string;
	using ::std::basic_string_view;

	using string = ::std::string;
	using wstring = ::std::wstring;
	using u8string = ::std::u8string;
	using u16string = ::std::u16string;
	using u32string = ::std::u32string;

	using string_view = ::std::string_view;
	using wstring_view = ::std::wstring_view;
	using u8string_view = ::std::u8string_view;
	using u16string_view = ::std::u16string_view;
	using u32string_view = ::std::u32string_view;

	using ::std::to_string;
	using ::std::to_wstring;
}
