#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define TESTNAME "www.example.com"

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

BOOL GetIPAddr6(char* name, IN6_ADDR* addr) {
	struct addrinfo hint;
	ZeroMemory(&hint, sizeof(hint));
	struct addrinfo* res = NULL;
	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(name, NULL, &hint, &res) != 0) {
		printf("enter here\n");
		err_display("getaddrinfo()");
		return FALSE;
	}
	if (res->ai_family != AF_INET6)
		return FALSE;
	memcpy(addr, res->ai_addr, res->ai_addrlen);
	return TRUE;
}


int main(int argc, char** argv) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름(변환 전) = %s\n", TESTNAME);

	// 도메인 이름 -> IP주소
	IN6_ADDR addr;
	if (GetIPAddr6(TESTNAME, &addr)) {
		for (int i = 0; i < 16; i++)
			printf("IP 주소 (변환 후) = %02x\n", addr.u.Byte[i]);
	}
	WSACleanup();
	return 0;
}