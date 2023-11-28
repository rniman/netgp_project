#include "TCPClient.h"

HWND hWnd;

MainCharacter mainCharacter;
BossMonster Boss;
BossCImage bossImage;
BulletBitmap bulletBitmap;

HANDLE hInitEvent;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define KEYBUFSIZE    8

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

int SendInputData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, 
	BossMonster& boss, const char* keyBuffer)
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

void sendPlayerInput(SOCKET clientSocket, const char* keyBuffer) {
	// Ű ���� ������ ����
	send(clientSocket, keyBuffer, KEYBUFSIZE, 0);
}

void receiveData(SOCKET clientSocket) {
	char buffer[KEYBUFSIZE];
	int bytesRead;

	// �����κ��� �����͸� ����
	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
		// ���� �����͸� ó���ϴ� ������ �߰��ؾ� ��
		printf("���� ������: %s\n", buffer);
	}
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

	if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
		err_quit("connect()");
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	// BITMAP WIDTH, HEIGHT �۽�
	// tbd

	// INIT ����
	// tbd

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		printf("\n[���� ������] ");

		// esc Ű�� ������ ������ Ż��
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;
		}

		// Ű �Է� ����
		if (_kbhit()) {
			// Ű �Է� ����
			char keyBuffer[KEYBUFSIZE] = "0000000"; // �ʱ�ȭ

			// ȭ��ǥ Ű
			if (GetAsyncKeyState(VK_UP) & 0x8000) {
				keyBuffer[0] = '1';
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				keyBuffer[1] = '1';
			}
			else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				keyBuffer[2] = '1';
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
				keyBuffer[3] = '1';
			}

			// Space Ű
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				keyBuffer[4] = '1';
			}

			// Shift Ű
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
				keyBuffer[5] = '1';
			}

			// Ctrl Ű
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				keyBuffer[6] = '1';
			}

			printf("keyBuffer : %s\n", keyBuffer);

			// Ű �Է��� ������ ����
			if (SendInputData(sock, mainCharacter, Boss, keyBuffer) == -1)
			{
				err_quit("send()");
			}

			// Ű �Է��� ������ ����
			sendPlayerInput(sock, keyBuffer);
		}
		else
		{
			char keyBuffer[KEYBUFSIZE];
			snprintf(keyBuffer, KEYBUFSIZE, "%d", 0); // 0�� ���ڿ��� ��ȯ�Ͽ� ���ۿ� ����
			printf("key : %s\n", keyBuffer);

			// Ű �Է��� ������ ����
			if (SendInputData(sock, mainCharacter, Boss, keyBuffer) == -1)
			{
				err_quit("send()");
			}

			// Ű �Է��� ������ ����
			sendPlayerInput(sock, keyBuffer);
		}

		receiveData(sock);

		// ���� �ð� ���� ���
		Sleep(100);  // 100 �и��� ���
	}

		//// INPUT �۽�
		//// tbd
		//if (SendInputData(sock, mainCharacter/*, mainCharacter*/, Boss) == -1)
		//{
		//	//����
		//	err_quit("send()");
		//}

		//// UPDATE ����
		//// tbd
		//RecvDefaultData(sock, mainCharacter/*, mainCharacter*/, Boss);
	return 0;
}
