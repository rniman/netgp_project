#include "ClientCharacter.h"

MainCharacter mainCharacter;
BossMonster Boss;
BOSSCIMAGE bossImage;

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

int SendInputData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss)
{
	//�ӽ÷� �� ���۸� ������.
	int retval;
	int len = 256;
	retval = send(remote, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}

	char buf[256];
	ZeroMemory(buf, 256);
	retval = send(remote, (char*)&buf, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}
}

int RecvDefaultData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss)
{
	int retval, len;
	retval = recv(remote, (char*)&len, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	MainCharacterInfo updateData;
	retval = recv(remote, (char*)&updateData, sizeof(MainCharacterInfo), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}
	p1Update.info = updateData;

	return 0;
}

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;
	SOCKET sock;

	// ���� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// BITMAP WIDTH, HEIGHT �۽�
	// tbd

	// INIT ����
	// tbd

	// ������ ������ ���
	while (1)
	{
		// INPUT �۽�
		// tbd
		if (SendInputData(sock, mainCharacter/*, mainCharacter*/, Boss) == -1)
		{
			//����
			err_quit("send()");
		}

		// UPDATE ����
		// tbd
		RecvDefaultData(sock, mainCharacter/*, mainCharacter*/, Boss);
	}

	return 0;
}
