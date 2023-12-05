#include "TCPServer.h"

#define KEYBUFSIZE    8

MainCharacter mainPlayer1, mainPlayer2;
BossMonster Boss;
HANDLE hP1Thread, hP2Thread;
HANDLE hPlayer1Input, hPlayer2Input;
HANDLE hPlayer1Update, hPlayer2Update;
HANDLE hMainUpdate;

BulletBitmap bulletBitmap;
BossBitData bossBitData;

char p1KeyBuffer[KEYBUFSIZE];
char p1OldKeyBuffer[KEYBUFSIZE];

char p2KeyBuffer[KEYBUFSIZE];
char p2OldKeyBuffer[KEYBUFSIZE];

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
	printf("[오류] %s\n", (char*)lpMsgBuf);
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

	printf("디폴트 데이터 송신!");
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

	// 클라이언트 정보 얻기
	//addrlen = sizeof(clientaddr);
	//getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);

	// 접속한 클라이언트 정보 출력
	//char addr[INET_ADDRSTRLEN];
	//inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	//printf("[클라이언트 접속 IP: %s, 포트 번호: %d]", addr, ntohs(clientaddr.sin_port));

	// BITMAP WIDTH, HEIGHT값 수신
	// tbd

	// 초기화 작업
	// tbd

	if (hP1Thread == threadParams.hThread)
	{
		mainPlayer1.info.type = 1;
		CreateMainChar(&mainPlayer1);

		// INIT 데이터를 송신
		// tbd
		sendData.player1 = mainPlayer1.info;
		sendData.player2 = mainPlayer2.info;
		sendData.bossMonster = Boss;

		SendInitData(client_sock, 1, sendData);
	}
	if(hP2Thread == threadParams.hThread)
	{
		mainPlayer2.info.type = 2;
		CreateMainChar(&mainPlayer2);

		// INIT 데이터를 송신
		// tbd
		sendData.player1 = mainPlayer1.info;
		sendData.player2 = mainPlayer2.info;
		sendData.bossMonster = Boss;

		SendInitData(client_sock, 2, sendData);
	}

	// BitmapData 수신 - 한 번만 수행
	if (hP1Thread == threadParams.hThread)
	{
		RecvInitBitmapData(client_sock, mainPlayer1.bitmap, bulletBitmap, bossBitData);
	}
	if (hP2Thread == threadParams.hThread)
		SetSecPlayerInitBitData(mainPlayer1.bitmap, mainPlayer2.bitmap);

	int len;
	// char keyBuffer[KEYBUFSIZE];
	while (1)
	{
		// INPUT 데이터를 받는다
		// 임시로 빈 버퍼를 받는다.
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		if(hP1Thread == threadParams.hThread)
		{
			retval = recv(client_sock, (char*)p1KeyBuffer, len, MSG_WAITALL);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
		}
		else if (hP2Thread == threadParams.hThread)
		{
			retval = recv(client_sock, (char*)p2KeyBuffer, len, MSG_WAITALL);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}

			//char buffer[32];
			//snprintf(buffer, sizeof(buffer), "보낸 데이터 : %s\n", p2KeyBuffer);
			//OutputDebugStringA(buffer);
		}

		if (hP1Thread == threadParams.hThread)
		{
			SetEvent(hPlayer1Input);
		}
		else if(hP2Thread == threadParams.hThread)
		{
			SetEvent(hPlayer2Input);
		}

		// 업데이트 스레드가 완료되기를 기다린다.
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

		// 서버에서 업데이트한 내용을 보내준다
		if (SendDefaultData(client_sock, sendData) == -1)
		{
			// 오류 처리
			printf("Send Default Error\n");
		}

	}

	// 소켓 닫기
	closesocket(client_sock);
	//printf("[클라이언트 종료 IP : %s, 포트 번호 : %d]", addr, ntohs(clientaddr.sin_port));

	return 0;
}

DWORD WINAPI UpdateThread(LPVOID arg)
{
	while (1)
	{
		// INPUT이 완료되기를 기다린다.
		WaitForSingleObject(hPlayer1Input, INFINITE);
		WaitForSingleObject(hPlayer2Input, INFINITE);

		//printf("업데이트를 수행합니다!!!!");
		// 업데이트 메시지를 받게 하면?
		WaitForSingleObject(hMainUpdate, INFINITE);

		// 업데이트 완료
		SetEvent(hPlayer1Update);
		SetEvent(hPlayer2Update);
	}
	return 0;
}
