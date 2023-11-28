#include "TCPServer.h"

MainCharacter mainCharacter, p2;
BossMonster Boss;
HANDLE hP1Thread, hP2Thread;
HANDLE hPlayer1Input, hPlayer2Input;
HANDLE hPlayer1Update, hPlayer2Update;
HANDLE hMainUpdate;

BulletBitmap bulletBitmap;
BossBitData bossBitData;

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

void SetInitBitData(SendBitData& sendBitData, MainCharacterBitmap& maincharBitData, BulletBitmap& bulletBitData, BossBitData& bossBitData)
{
	for (int i = 0; i < IDLEANI; ++i)
	{
		maincharBitData.IDLEBitData[i] = sendBitData.IDLEBitData[i];
	}
	for (int i = 0; i < RUNANI; ++i)
	{
		maincharBitData.RUNBitData[i] = sendBitData.RUNBitData[i];
	}
	for (int i = 0; i < JUMPANI; ++i)
	{
		maincharBitData.JUMPBitData[i] = sendBitData.JUMPBitData[i];
	}
	for (int i = 0; i < SHOOTANI; ++i)
	{
		maincharBitData.SHOOTBitData[i] = sendBitData.SHOOTBitData[i];
	}
	for (int i = 0; i < RUNSHOOTANI; ++i)
	{
		maincharBitData.RUNSHOOTBitData[i] = sendBitData.RUNSHOOTBitData[i];
	}
	for (int i = 0; i < EXSHOOTANI; ++i)
	{
		maincharBitData.EXSHOOTBitData[i] = sendBitData.EXSHOOTBitData[i];
	}
	for (int i = 0; i < HITANI; ++i)
	{
		maincharBitData.HITBitData[i] = sendBitData.HITBitData[i];
	}
	for (int i = 0; i < GHOSTANI; ++i)
	{
		maincharBitData.GHOSTBitData[i] = sendBitData.GHOSTBitData[i];
	}
	for (int i = 0; i < REVIVEANI; ++i)
	{
		maincharBitData.REVIVEBitData[i] = sendBitData.REVIVEBitData[i];
	}

	bulletBitData.LOOPBitData = sendBitData.LOOPBitData;
	bulletBitData.EXBitData = sendBitData.EXBitData;
	for (int i = 0; i < 6; ++i)
	{
		bulletBitData.DEATHLOOPBitData[i] = sendBitData.DEATHLOOPBitData[i];
	}
	for (int i = 0; i < 9; ++i)
	{
		bulletBitData.DEATHEXBitData[i] = sendBitData.DEATHEXBitData[i];
	}

	for (int i = 0; i < 20; ++i)
	{
		bossBitData.ATTACKTAILBitData[i] = sendBitData.ATTACKTAILBitData[i];
	}
	for (int i = 0; i < 8; ++i)
	{
		bossBitData.ATTACKFIREBitData[i] = sendBitData.ATTACKFIREBitData[i];
	}
	for (int i = 0; i < 19; ++i)
	{
		bossBitData.ATTACKMETEORBitData[i] = sendBitData.ATTACKMETEORBitData[i];
	}
	for (int i = 0; i < 35; ++i)
	{
		bossBitData.METEOREXTINCTIONBitData[i] = sendBitData.METEOREXTINCTIONBitData[i];
	}
}

int RecvInitBitmapData(SOCKET client, MainCharacterBitmap& maincharBitData, BulletBitmap& bulletBitData, BossBitData& bossBitData)
{
	int retval;
	int len = sizeof(SendBitData);
	retval = recv(client, (char*)&len, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	SendBitData sendBitData;
	retval = recv(client, (char*)&sendBitData, len, MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	SetInitBitData(sendBitData, maincharBitData, bulletBitData, bossBitData);

	return 0;
}

int SendDefaultData(SOCKET client, const SendUpdateData& updateData)
{
	int retval;
	int len = sizeof(SendUpdateData);

	printf("����Ʈ ������ �۽�!");
	retval = send(client, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	retval = send(client, (char*)&updateData, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	return 0;
}


DWORD WINAPI NetworkThread(LPVOID arg)
{
	ThreadParams threadParams = *(ThreadParams*)arg;

	int retval;
	SOCKET client_sock = threadParams.socket;
	struct sockaddr_in clientaddr;
	int addrlen;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);

	// ������ Ŭ���̾�Ʈ ���� ���
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[Ŭ���̾�Ʈ ���� IP: %s, ��Ʈ ��ȣ: %d]", addr, ntohs(clientaddr.sin_port));

	// BITMAP WIDTH, HEIGHT�� ����
	// tbd

	// �ʱ�ȭ �۾�
	// tbd
	CreateMainChar(&mainCharacter);
	//LoadBullet(&bulletBitmap, g_hInst);
	Boss.rect = { 634, 50, 984, 561 };

	// INIT �����͸� ����
	// tbd
	RecvInitBitmapData(client_sock, mainCharacter.bitmap, bulletBitmap, bossBitData);

	int len;
	char buf[256];
	while (1)
	{
		// INPUT �����͸� �޴´�
		// �ӽ÷� �� ���۸� �޴´�.
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		retval = recv(client_sock, (char*)buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		if (hP1Thread == threadParams.hThread)
		{
			SetEvent(hPlayer1Input);
		}
		//else if(hP2Thread == threadParams.hThread)
		//	SetEvent(hPlayer2Input);


		// ������Ʈ �����尡 �Ϸ�Ǳ⸦ ��ٸ���.
		if (hP1Thread == threadParams.hThread)
		{
			WaitForSingleObject(hPlayer1Update, INFINITE);
		}
		//else if (hP2Thread == threadParams.hThread)
		//	WaitForSingleObject(hPlayer2Update, INFINITE);

		SendUpdateData updateData;
		updateData.player1 = mainCharacter.info;
		updateData.bossMonster = Boss;
		//updateData.player2 = p2.info;

		// �������� ������Ʈ�� ������ �����ش�
		if (SendDefaultData(client_sock, updateData) == -1)
		{
			// ���� ó��
			printf("Send Default Error\n");
		}
		else
		{
			char buf[256];
			sprintf_s(buf, sizeof(buf), "Debug: %d %d %d %d\n", updateData.player1.Pos.left, updateData.player1.Pos.top, updateData.player1.Pos.right, updateData.player1.Pos.bottom);
			OutputDebugStringA(buf);
		}
	}

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[Ŭ���̾�Ʈ ���� IP : %s, ��Ʈ ��ȣ : %d]", addr, ntohs(clientaddr.sin_port));

	return 0;
}

DWORD WINAPI UpdateThread(LPVOID arg)
{
	while (1)
	{
		// INPUT�� �Ϸ�Ǳ⸦ ��ٸ���.
		WaitForSingleObject(hPlayer1Input, INFINITE);
		//WaitForSingleObject(hPlayer2Input, INFINITE);

		//printf("������Ʈ�� �����մϴ�!!!!");
		// ������Ʈ �޽����� �ް� �ϸ�?
		WaitForSingleObject(hMainUpdate, INFINITE);

		// ������Ʈ �Ϸ�
		SetEvent(hPlayer1Update);
		//SetEvent(hPlayer2Update);
	}
	return 0;
}
