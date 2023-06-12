module;
#include <WS2tcpip.h>
#include <MSWSock.h>
export module RIO;
import Net.Socket;

export namespace net
{
	void init_rio()
	{
		RIO_EXTENSION_FUNCTION_TABLE rio; // �Լ� ���̺�
		constexpr GUID functionTableId = WSAID_MULTIPLE_RIO; // ���̺� ���̵�

		DWORD dwBytes = 0; // ���� ����
		SOCKET mListenSocket{};

		if (WSAIoctl(mListenSocket,
			SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER,

			&functionTableId, /* ���̺� ���̵� */
			sizeof(GUID),

			(void**)&rio, /* ���̺� */
			sizeof(RIO_EXTENSION_FUNCTION_TABLE), /* ���̺� ũ�� */

			&dwBytes, NULL, NULL)
		)
		{
			throw L"WSAIoctl()"; // ����
		}


		LPFN_RIODEREGISTERBUFFER;
	}
}
