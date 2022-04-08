#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

void	err_display(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Error: give me a address!\n");
		return 1;
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	struct hostent* HOST;
	printf("도메인 이름(변환 전) = %s\n", argv[1]);

	// 도메인 이름 -> IP주소
	HOST = gethostbyname(argv[1]);
	printf("공식 도메인 이름 : %s\n", HOST->h_name);
	printf("도메인 별칭 :\n");
	for (int i = 0; HOST->h_aliases[i]; i++) {
		printf("%s\n", HOST->h_aliases[i]);
	}
	printf("\n 도메인 주소\n");
	for (int i = 0; HOST->h_addr_list[i]; i++) {
		printf("%s\n", (inet_ntoa(*(IN_ADDR*)HOST->h_addr_list[i])));
	}
	WSACleanup();
	return 0;
}