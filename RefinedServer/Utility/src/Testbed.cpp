#include <WS2tcpip.h>
#include <charconv>
import Utility.String.Literal;

#define __TEST__ 0

#if __TEST__

int main()
{
	//util::basic_fixed_string str1{ "1" };

	//constexpr std::chrono::duration<int, std::ratio<1, 1>> dr1(1);

	short v_i16;

	constexpr char str[] = "123123";
	constexpr auto ch1 = std::from_chars(str, str + 7, v_i16);

	constexpr auto ec = ch1.ec;
	constexpr auto ptr = ch1.ptr;
	constexpr auto vptr = *ptr;
}
#endif
