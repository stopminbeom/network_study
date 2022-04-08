#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char** argv) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET	tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	u_short	x1 = 0x1234;
	u_long	y1 = 0x12345678;
	u_short x2;
	u_long	y2;
	u_short x3;
	u_long	y3;

	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	WSAHtons(tcp_sock, x1, &x2);
	WSAHtonl(tcp_sock, y1, &y2);
	printf("0x%x -> 0x%x\n", x1, x2);
	printf("0x%x -> 0x%x\n", y1, y2);

	printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	WSANtohs(tcp_sock, x2, &x3);
	WSANtohl(tcp_sock, y2, &y3);
	printf("0x%x -> 0x%x\n", x2, x3);
	printf("0x%x -> 0x%x\n", y2, y3);

	printf("\n[잘못된 사용 예]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));
	
	closesocket(tcp_sock);
	WSACleanup();
	return 0;
}