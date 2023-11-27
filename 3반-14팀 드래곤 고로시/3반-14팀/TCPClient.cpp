#include "TCPClient.h"
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

HWND hWnd;

MainCharacter mainCharacter;
BossMonster Boss;
BossCImage bossImage;
BulletBitmap bulletBitmap;

HANDLE hInitEvent;

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

void SetSendBitmapData(SendBitData& sendBitData, const MainCharacterBitmap& maincharBitData, const BulletBitmap& bulletBitData, const BossCImage& bossBitData)
{
	for (int i = 0; i < IDLEANI; ++i)
	{
		sendBitData.IDLEBitData[i] = maincharBitData.IDLEBitData[i];
	}
	for (int i = 0; i < RUNANI; ++i)
	{
		sendBitData.RUNBitData[i] = maincharBitData.RUNBitData[i];
	}
	for (int i = 0; i < JUMPANI; ++i)
	{
		sendBitData.JUMPBitData[i] = maincharBitData.JUMPBitData[i];
	}
	for (int i = 0; i < SHOOTANI; ++i)
	{
		sendBitData.SHOOTBitData[i] = maincharBitData.SHOOTBitData[i];
	}
	for (int i = 0; i < RUNSHOOTANI; ++i)
	{
		sendBitData.RUNSHOOTBitData[i] = maincharBitData.RUNSHOOTBitData[i];
	}
	for (int i = 0; i < EXSHOOTANI; ++i)
	{
		sendBitData.EXSHOOTBitData[i] = maincharBitData.EXSHOOTBitData[i];
	}
	for (int i = 0; i < HITANI; ++i)
	{
		sendBitData.HITBitData[i] = maincharBitData.HITBitData[i];
	}
	for (int i = 0; i < GHOSTANI; ++i)
	{
		sendBitData.GHOSTBitData[i] = maincharBitData.GHOSTBitData[i];
	}
	for (int i = 0; i < REVIVEANI; ++i)
	{
		sendBitData.REVIVEBitData[i] = maincharBitData.REVIVEBitData[i];
	}

	sendBitData.LOOPBitData = bulletBitData.LOOPBitData;
	sendBitData.EXBitData = bulletBitData.EXBitData;
	for (int i = 0; i < 6; ++i)
	{
		sendBitData.DEATHLOOPBitData[i] = bulletBitData.DEATHLOOPBitData[i];
	}
	for (int i = 0; i < 9; ++i)
	{
		sendBitData.DEATHEXBitData[i] = bulletBitData.DEATHEXBitData[i];
	}


	for (int i = 0; i < 20; ++i)
	{
		sendBitData.ATTACKTAILBitData[i].bmHeight = bossBitData.AttackTail[i].GetHeight();
		sendBitData.ATTACKTAILBitData[i].bmWidth = bossBitData.AttackTail[i].GetWidth();
	}
	for (int i = 0; i < 8; ++i)
	{
		sendBitData.ATTACKFIREBitData[i].bmHeight = bossBitData.AttackFire[i].GetHeight();
		sendBitData.ATTACKFIREBitData[i].bmWidth = bossBitData.AttackFire[i].GetWidth();
	}
	for (int i = 0; i < 19; ++i)
	{
		sendBitData.ATTACKMETEORBitData[i].bmHeight = bossBitData.AttackMeteor[i].GetHeight();
		sendBitData.ATTACKMETEORBitData[i].bmWidth = bossBitData.AttackMeteor[i].GetWidth();
	}
	for (int i = 0; i < 35; ++i)
	{
		sendBitData.METEOREXTINCTIONBitData[i].bmHeight = bossBitData.MeteorExtinction[i].GetHeight();
		sendBitData.METEOREXTINCTIONBitData[i].bmWidth = bossBitData.MeteorExtinction[i].GetWidth();
	}

}

int SendInitBitmapData(SOCKET remote, const MainCharacterBitmap& maincharBitData, const BulletBitmap& bulletBitData, const BossCImage& bossBitData)
{
	SendBitData sendBitData;
	SetSendBitmapData(sendBitData, maincharBitData, bulletBitData, bossBitData);

	int retval;
	int len = sizeof(SendBitData);
	retval = send(remote, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}

	retval = send(remote, (char*)&sendBitData, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}

	return 0;
}

int SendInputData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss)
{
	//임시로 빈 버퍼를 보낸다.
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

	return 0;
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

	RecvUpdateData updateData;
	retval = recv(remote, (char*)&updateData, len, MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}
	p1Update.info = updateData.player1;
	boss = updateData.bossMonster;

	return 0;
}

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;
	SOCKET sock;

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return 0;
		//err_quit("socket()");
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
		return 0;
		//err_quit("connect()");
	}

	
	// BITMAP WIDTH, HEIGHT 송신
	// tbd
	// 클라이언트에서 비트맵, PNG데이터를 불러와야 실행가능 -> 이벤트로 
	WaitForSingleObject(hInitEvent, INFINITE);
	SendInitBitmapData(sock, mainCharacter.bitmap, bulletBitmap, bossImage);

	// INIT 수신
	// tbd

	// 서버와 데이터 통신
	while (1)
	{
		// INPUT 송신
		// tbd
		if (SendInputData(sock, mainCharacter/*, mainCharacter*/, Boss) == -1)
		{
			//오류
			err_quit("send()");
		}

		// UPDATE 수신
		// tbd
		RecvDefaultData(sock, mainCharacter/*, mainCharacter*/, Boss);

		InvalidateRect(hWnd, NULL, FALSE);
	}

	return 0;
}
