#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	printf("wVersion : %d\n", wsa.wVersion);
	printf("wHighVersion : %d\n", wsa.wHighVersion);
	printf("szDescription : %s\n", wsa.szDescription);
	printf("szSystemStatus : %s\n", wsa.szSystemStatus);
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// 윈속 종료
	WSACleanup();
	return 0;
}
