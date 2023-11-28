#include "TCPServer.h"

MainCharacter mainPlayer1, mainPlayer2;
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

int SendInitData(SOCKET client, DWORD playerNum, const SendUpdateData& sendData)
{
	int retval;
	int len;
	
	len = sizeof(DWORD);
	retval = send(client, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_quit("INit send!");
		return -1;
	}

	retval = send(client, (char*)&playerNum, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_quit("INit send!");
		return -1;
	}

	len = sizeof(SendUpdateData);
	retval = send(client, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_quit("INit send!");
		return -1;
	}

	retval = send(client, (char*)&sendData, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_quit("INit send!");
		return -1;
	}

	return 0;
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

void SetSecPlayerInitBitData(const MainCharacterBitmap& player1BitData, MainCharacterBitmap& player2BitData)
{
	for (int i = 0; i < IDLEANI; ++i)
	{
		player2BitData.IDLEBitData[i] = player1BitData.IDLEBitData[i];
	}
	for (int i = 0; i < RUNANI; ++i)
	{
		player2BitData.RUNBitData[i] = player1BitData.RUNBitData[i];
	}
	for (int i = 0; i < JUMPANI; ++i)
	{
		player2BitData.JUMPBitData[i] = player1BitData.JUMPBitData[i];
	}
	for (int i = 0; i < SHOOTANI; ++i)
	{
		player2BitData.SHOOTBitData[i] = player1BitData.SHOOTBitData[i];
	}
	for (int i = 0; i < RUNSHOOTANI; ++i)
	{
		player2BitData.RUNSHOOTBitData[i] = player1BitData.RUNSHOOTBitData[i];
	}
	for (int i = 0; i < EXSHOOTANI; ++i)
	{
		player2BitData.EXSHOOTBitData[i] = player1BitData.EXSHOOTBitData[i];
	}
	for (int i = 0; i < HITANI; ++i)
	{
		player2BitData.HITBitData[i] = player1BitData.HITBitData[i];
	}
	for (int i = 0; i < GHOSTANI; ++i)
	{
		player2BitData.GHOSTBitData[i] = player1BitData.GHOSTBitData[i];
	}
	for (int i = 0; i < REVIVEANI; ++i)
	{
		player2BitData.REVIVEBitData[i] = player1BitData.REVIVEBitData[i];
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
	SendUpdateData sendData;

	ThreadParams threadParams = *(ThreadParams*)arg;

	int retval;
	SOCKET client_sock = threadParams.socket;
	struct sockaddr_in clientaddr;
	int addrlen;

	// Ŭ���̾�Ʈ ���� ���
	//addrlen = sizeof(clientaddr);
	//getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);

	// ������ Ŭ���̾�Ʈ ���� ���
	//char addr[INET_ADDRSTRLEN];
	//inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	//printf("[Ŭ���̾�Ʈ ���� IP: %s, ��Ʈ ��ȣ: %d]", addr, ntohs(clientaddr.sin_port));

	// BITMAP WIDTH, HEIGHT�� ����
	// tbd

	// �ʱ�ȭ �۾�
	// tbd
	if (hP1Thread == threadParams.hThread)
	{
		mainPlayer1.info.type = 1;
		CreateMainChar(&mainPlayer1);

		// INIT �����͸� �۽�
		// tbd
		sendData.player1 = mainPlayer1.info;
		sendData.player2 = mainPlayer2.info;
		sendData.bossMonster = Boss;

		SendInitData(client_sock, 1, sendData);
	}
	else if(hP2Thread == threadParams.hThread)
	{
		mainPlayer2.info.type = 2;
		CreateMainChar(&mainPlayer2);

		// INIT �����͸� �۽�
		// tbd
		sendData.player1 = mainPlayer1.info;
		sendData.player2 = mainPlayer2.info;
		sendData.bossMonster = Boss;

		SendInitData(client_sock, 2, sendData);
	}

	// BitmapData ���� - �� ���� ����
	if (hP1Thread == threadParams.hThread)
	{
		RecvInitBitmapData(client_sock, mainPlayer1.bitmap, bulletBitmap, bossBitData);
	}
	if (hP2Thread == threadParams.hThread)
		SetSecPlayerInitBitData(mainPlayer1.bitmap, mainPlayer2.bitmap);

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
		else if(hP2Thread == threadParams.hThread)
		{
			SetEvent(hPlayer2Input);
		}


		// ������Ʈ �����尡 �Ϸ�Ǳ⸦ ��ٸ���.
		if (hP1Thread == threadParams.hThread)
		{
			WaitForSingleObject(hPlayer1Update, INFINITE);
		}
		else if (hP2Thread == threadParams.hThread)
		{
			WaitForSingleObject(hPlayer2Update, INFINITE);
		}

		sendData.player1 = mainPlayer1.info;
		sendData.player2 = mainPlayer2.info;
		sendData.bossMonster = Boss;

		// �������� ������Ʈ�� ������ �����ش�
		if (SendDefaultData(client_sock, sendData) == -1)
		{
			// ���� ó��
			printf("Send Default Error\n");
		}
		else
		{
			//char buf[256];
			//sprintf_s(buf, sizeof(buf), "Debug: %d %d %d %d\n", sendData.player1.Pos.left, sendData.player1.Pos.top, sendData.player1.Pos.right, sendData.player1.Pos.bottom);
			//OutputDebugStringA(buf);
		}
	}

	// ���� �ݱ�
	closesocket(client_sock);
	//printf("[Ŭ���̾�Ʈ ���� IP : %s, ��Ʈ ��ȣ : %d]", addr, ntohs(clientaddr.sin_port));

	return 0;
}

DWORD WINAPI UpdateThread(LPVOID arg)
{
	while (1)
	{
		// INPUT�� �Ϸ�Ǳ⸦ ��ٸ���.
		WaitForSingleObject(hPlayer1Input, INFINITE);
		WaitForSingleObject(hPlayer2Input, INFINITE);

		//printf("������Ʈ�� �����մϴ�!!!!");
		// ������Ʈ �޽����� �ް� �ϸ�?
		WaitForSingleObject(hMainUpdate, INFINITE);

		// ������Ʈ �Ϸ�
		SetEvent(hPlayer1Update);
		SetEvent(hPlayer2Update);
	}
	return 0;
}
