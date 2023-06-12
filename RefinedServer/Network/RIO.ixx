module;
#include <WS2tcpip.h>
#include <MSWSock.h>
export module RIO;
import Net.Socket;

export namespace net
{
	void init_rio()
	{
		RIO_EXTENSION_FUNCTION_TABLE rio; // 함수 테이블
		constexpr GUID functionTableId = WSAID_MULTIPLE_RIO; // 테이블 아이디

		DWORD dwBytes = 0; // 더미 변수
		SOCKET mListenSocket{};

		if (WSAIoctl(mListenSocket,
			SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER,

			&functionTableId, /* 테이블 아이디 */
			sizeof(GUID),

			(void**)&rio, /* 테이블 */
			sizeof(RIO_EXTENSION_FUNCTION_TABLE), /* 테이블 크기 */

			&dwBytes, NULL, NULL)
		)
		{
			throw L"WSAIoctl()"; // 예외
		}


		LPFN_RIODEREGISTERBUFFER;
	}
}
