#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <thread>

#include "Packet.h"

#define SERVERPORT 11021
#define BUFSIZE    512

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int	CalcuReturn(int n1, int n2, short op) {
	int ret;

	ret = 0;
	switch (op) {
	case 0:
		ret = n1 + n2;
		break;
	case 1:
		ret = n1 - n2;
		break;
	case 2:
		ret = n1 * n2;
		break;
	case 3:
		ret = n1 / n2;
	}
	return ret;
}

void PacketProcess(SOCKET sock, char* pPacket)
{
	PktCalcuRes res;
	res.Id = (short)PACKET_ID::CALCU_RES;
	res.TotalSize = (short)sizeof(PktCalcuRes);
	res.num = 0;

	PktHeader* pHeader = (PktHeader*)pPacket;

	if (pHeader->Id == (short)PACKET_ID::CALCU_2_REQ)
	{
		PktCalcu2Req* req = (PktCalcu2Req*)pPacket;	

		res.num = CalcuReturn(req->n1, req->n2, req->op1);
	} 
	else if (pHeader->Id == (short)PACKET_ID::CALCU_3_REQ)
	{
		PktCalcu3Req* req = (PktCalcu3Req*)pPacket;

		if (req->op1 < 2 && req->op2 > 1) {
			res.num = CalcuReturn(req->n2, req->n3, req->op2);
			res.num = CalcuReturn(req->n1, res.num, req->op1);
		}
		else {
			res.num = CalcuReturn(req->n1, req->n2, req->op1);
			res.num = CalcuReturn(res.num, req->n3, req->op2);
		}
	}
	send(sock, (char*)&res, res.TotalSize, 0);
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
	}
		
	while(1){
		SOCKADDR_IN clientaddr;		
		int addrlen = sizeof(clientaddr);
		
		SOCKET client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
		
		// ������ Ŭ���̾�Ʈ ���� ���
		char clientIP[33] = { 0, };
		inet_ntop(AF_INET, &(clientaddr.sin_addr), clientIP, 33 - 1);
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", clientIP, ntohs(clientaddr.sin_port));
				
		
		// Ŭ���̾�Ʈ�� ������ ���
		char buf[BUFSIZE + 1];

		while(1){
			int recvSize = recv(client_sock, buf, BUFSIZE, 0);
			if(recvSize == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if (recvSize == 0) {
				break;
			}
						
			// ���� ũ�� ���
			printf("[recv:%d]\n", recvSize);

			int readPos = 0;
			while (recvSize >= PACKET_HEADER_SIZE)
			{
				PktHeader* pHeader = (PktHeader*)&buf[readPos];

				if (pHeader->TotalSize > recvSize)
				{
					break;
				}
				
				PacketProcess(client_sock, &buf[readPos]);
				
				readPos += pHeader->TotalSize;
				recvSize -= pHeader->TotalSize;
			}

			// �׽�Ʈ�� ���� �Ϻη� ��⸦ �մϴ�
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", clientIP, ntohs(clientaddr.sin_port));
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}




