#include <WS2tcpip.h>
#include <charconv>
#include <chrono>

#define __TEST__ 0

#if __TEST__

int main()
{
	constexpr std::chrono::duration<int, std::ratio<1, 1>> dr1(1);

	short v_i16;
	unsigned short v_u16;
	int v_i32;
	unsigned v_u32n;

	constexpr char str[] = "123123";
	constexpr auto ch1 = std::from_chars(str, str + 7, v_i16);

	constexpr auto ec = ch1.ec;
	constexpr auto ptr = ch1.ptr;
	constexpr auto vptr = *ptr;
}
#endif
