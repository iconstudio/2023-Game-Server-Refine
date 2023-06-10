#include <WS2tcpip.h>
#include <charconv>

#if __TEST__
int main()
{
	constexpr char str[] = "123123";

	short v_i16;
	unsigned short v_u16;
	int v_i32;
	unsigned v_u32n;

	std::from_chars(str, str + 7, v_i16); // (1)
}
#endif
