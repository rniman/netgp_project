#include "TCPClient.h"
#include "resource.h"
//#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define KEYBUFSIZE    8

HWND hWnd;

DWORD playerNum;
MainCharacter mainPlayer1;
MainCharacter mainPlayer2;
BossMonster Boss;
BossCImage bossImage;
BulletBitmap bulletBitmap;

HANDLE hInitEvent;

char g_cIpAddress[16];

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

void ConvertLPWSTRToChar(LPWSTR lpwstr, char* dest, int destSize)
{
	// WideCharToMultiByte �Լ��� ����Ͽ� LPWSTR�� char*�� ��ȯ
	WideCharToMultiByte(
		CP_UTF8,
		0,                   // ��ȯ �ɼ�
		lpwstr,              // ��ȯ�� �����ڵ� ���ڿ�
		-1,                  // �ڵ����� ���ڿ� ���� ���
		dest,                // ��� ����
		destSize,            // ��� ������ ũ��
		NULL,                // �⺻ ���� ��� �� ��
		NULL                 // �⺻ ���� ��� ���θ� ������ ������ �ּ�
	);
}

INT_PTR CALLBACK IpDialogProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:

		// �⺻ IP ����
		SetDlgItemText(hWnd, IDC_IPADDRESS, L"127.0.0.1");
		return IDOK;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// IP �ּ� ����
			WCHAR m_lpIpAddress[16];
			GetDlgItemText(hWnd, IDC_IPADDRESS, m_lpIpAddress, 16);

			ConvertLPWSTRToChar(m_lpIpAddress, g_cIpAddress, 16);

			EndDialog(hWnd, IDOK);
			return IDOK;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, IDCANCEL);
			return IDCANCEL;
		}

		break;
	}
	return FALSE;
}

int RecvInitData(SOCKET remote, MainCharacter& p1Update, MainCharacter& p2Update, BossMonster& boss)
{
	int retval, len;
	retval = recv(remote, (char*)&len, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	retval = recv(remote, (char*)&playerNum, len, MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	retval = recv(remote, (char*)&len, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}

	RecvUpdateData recvData;
	retval = recv(remote, (char*)&recvData, len, MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}
	p1Update.info = recvData.player1;
	p2Update.info = recvData.player2;
	boss = recvData.bossMonster;

	return 0;
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

int SendInputData(SOCKET remote, const char* p1KeyBuffer, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss)
{
	//�ӽ÷� �� ���۸� ������.
	int retval;
	//int len = 256;
	int len = 8;
	retval = send(remote, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}

	/*char buf[256];
	ZeroMemory(buf, 256);
	retval = send(remote, (char*)&buf, len, 0);*/
	retval = send(remote, p1KeyBuffer, len, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return -1;
	}

	return 0;
}

int RecvDefaultData(SOCKET remote, MainCharacter& p1Update, MainCharacter& p2Update, BossMonster& boss)
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
	p2Update.info = updateData.player2;
	boss = updateData.bossMonster;

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
		return 0;
		//err_quit("socket()");
	}

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(g_cIpAddress);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
		closesocket(sock);
		return 0;
	}
	
	// INIT ����
	// tbd
	RecvInitData(sock, mainPlayer1, mainPlayer2, Boss);
	
	// BITMAP WIDTH, HEIGHT �۽� - �ѹ��� ����
	// tbd
	// Ŭ���̾�Ʈ���� ��Ʈ��, PNG�����͸� �ҷ��;� ���డ�� -> �̺�Ʈ�� 
	WaitForSingleObject(hInitEvent, INFINITE);
	if(playerNum == 1)
	{
		SendInitBitmapData(sock, mainPlayer1.bitmap, bulletBitmap, bossImage);
	}

	//// ������ ������ ���
	//while (1)
	//{
	//	// INPUT �۽�
	//	// tbd

	//	// esc Ű�� ������ ������ Ż��
	//	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
	//		break;
	//	}

	//	// Ű �Է� ����
	//	if (_kbhit()) {
	//		// Ű �Է� ����
	//		char keyBuffer[KEYBUFSIZE] = "0000000"; // �ʱ�ȭ

	//		// ȭ��ǥ Ű
	//		if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//			keyBuffer[0] = '1';
	//		}
	//		else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//			keyBuffer[1] = '1';
	//		}
	//		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//			keyBuffer[2] = '1';
	//		}
	//		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//			keyBuffer[3] = '1';
	//		}

	//		// Space Ű
	//		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
	//			keyBuffer[4] = '1';
	//		}

	//		// Shift Ű
	//		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
	//			keyBuffer[5] = '1';
	//		}

	//		// Ctrl Ű
	//		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
	//			keyBuffer[6] = '1';
	//		}

	//		char buffer[32];
	//		snprintf(buffer, sizeof(buffer), "���� ������ : %s\n", keyBuffer);
	//		OutputDebugStringA(buffer);

	//		// Ű �Է��� ������ ����
	//		if (SendInputData(sock, keyBuffer, mainPlayer1/*, mainCharacter*/, Boss) == -1)
	//		{
	//			//����
	//			err_quit("send()");
	//		}
	//	}
	//	else
	//	{
	//		char keyBuffer[KEYBUFSIZE];
	//		snprintf(keyBuffer, KEYBUFSIZE, "%d", 0); // 0�� ���ڿ��� ��ȯ�Ͽ� ���ۿ� ����
	//		//printf("keyBuffer : %s\n", keyBuffer);
	//		char buffer[32];
	//		snprintf(buffer, sizeof(buffer), "���� ������ : %s\n", keyBuffer);
	//		OutputDebugStringA(buffer);

	//		// Ű �Է��� ������ ����
	//		if (SendInputData(sock, keyBuffer, mainPlayer1/*, mainCharacter*/, Boss) == -1)
	//		{
	//			//����
	//			err_quit("send()");
	//		}
	//	}

	//	// UPDATE ����
	//	// tbd
	//	RecvDefaultData(sock, mainPlayer1, mainPlayer2, Boss);

	//	InvalidateRect(hWnd, NULL, FALSE);
	//}

	// ������ ������ ���
	while (1)
	{

		// INPUT �۽�
		char pKeyBuffer[KEYBUFSIZE] = "0000000"; // �ʱ�ȭ

		// ȭ��ǥ Ű
		if (GetAsyncKeyState(VK_UP) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[0] = '1';
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[1] = '1';
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[2] = '1';
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[3] = '1';
		}
		// Space Ű
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[4] = '1';
		}
		// Shift Ű
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[5] = '1';
		}
		// Ctrl Ű
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetForegroundWindow() == hWnd)
		{
			pKeyBuffer[6] = '1';
		}

		//char buffer[32];
		//snprintf(buffer, sizeof(buffer), "���� ������ : %s\n", keyBuffer);
		//OutputDebugStringA(buffer);

		// Ű �Է��� ������ ����
		if (SendInputData(sock, pKeyBuffer, mainPlayer1, Boss) == -1)
		{
			//����
			err_quit("send()");
		}

		// UPDATE ����
		// tbd
		RecvDefaultData(sock, mainPlayer1, mainPlayer2, Boss);

		InvalidateRect(hWnd, NULL, FALSE);

		//Sleep(16); // �� CPU ��뷮�� ���ϱ� ���� ���� ���� �߰�
	}

	return 0;
}
