#include "stdafx.h"
#include "TCPServer.h"

#define SERVERPORT 9000

MainCharacter p1, p2;
BossMonster bossMob;

ThreadParams p1ThreadParams, p2ThreadParams;
HANDLE hP1Thread, hP2Thread;
HANDLE hPlayer1Input, hPlayer2Input;
HANDLE hPlayer1Update, hPlayer2Update;

DWORD WINAPI NetworkThread(LPVOID arg)
{
	ThreadParams threadParams = *(ThreadParams*)arg;

	int retval;
	SOCKET client_sock = threadParams.socket;
	struct sockaddr_in clientaddr;
	int addrlen;

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);

	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[Ŭ���̾�Ʈ ���� IP: %s, ��Ʈ ��ȣ: %d]", addr, ntohs(clientaddr.sin_port));

	int len;
	char buf[256];
	while (1)
	{
		// INPUT �����͸� �޴´�
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recv(client_sock, (char*)buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		
		if(hP1Thread == threadParams.hThread)
			SetEvent(hPlayer1Input);
		else if(hP2Thread == threadParams.hThread)
			SetEvent(hPlayer2Input);


		// ������Ʈ �����尡 �Ϸ�Ǳ⸦ ��ٸ���.
		if (hP1Thread == threadParams.hThread)
			WaitForSingleObject(hPlayer1Update, INFINITE);
		else if (hP2Thread == threadParams.hThread)
			WaitForSingleObject(hPlayer2Update, INFINITE);

		SendUpdateData updateData;
		updateData.player1 = p1.info;
		updateData.player2 = p2.info;

		// �������� ������Ʈ�� ������ �����ش�
		if (SendDefaultData(client_sock, updateData) == -1)
		{
			// ���� ó��
		}
	}
	printf("[Ŭ���̾�Ʈ ���� IP : %s, ��Ʈ ��ȣ : %d]", addr, ntohs(clientaddr.sin_port));
	
	return 0;
}

DWORD WINAPI UpdateThread(LPVOID arg)
{
	// INPUT�� �Ϸ�Ǳ⸦ ��ٸ���.
	WaitForSingleObject(hPlayer1Input, INFINITE);
	WaitForSingleObject(hPlayer2Input, INFINITE);

	printf("������Ʈ�� �����մϴ�!!!!");
	Sleep(5000);

	// ������Ʈ �Ϸ�
	SetEvent(hPlayer1Update);
	SetEvent(hPlayer2Update);
}


int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	struct sockaddr_in clientaddr;
	int addrlen;

	int nPlayer = 1;
	while (1) 
	{
		// �ϴ��� p1���� �޾ƺ���
		addrlen = sizeof(clientaddr);
		if(nPlayer == 1)
		{
			p1ThreadParams.socket = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
			if (p1ThreadParams.socket == INVALID_SOCKET)
			{
				err_display("accept()");
				break;
			}
		}
		else if (nPlayer == 2)
		{
			p2ThreadParams.socket = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
			if (p2ThreadParams.socket == INVALID_SOCKET)
			{
				err_display("accept()");
				break;
			}
		}

		//char addr[INET_ADDRSTRLEN];
		//inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

		if (nPlayer == 1)
		{
			p1ThreadParams.hThread = hP1Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p1ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p1ThreadParams.socket);
			}
		}
		else if (nPlayer == 2)
		{
			p2ThreadParams.hThread = hP2Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p2ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p2ThreadParams.socket);
			}
		}

		++nPlayer;
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}
