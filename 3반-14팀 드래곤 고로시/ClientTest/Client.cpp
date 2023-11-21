#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <conio.h>  // _getch �Լ��� ����ϱ� ���� ���
#include <windows.h>

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define SERVERPORT 9000
#define BUFSIZE    8

char* SERVERIP = (char*)"127.0.0.1";

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

void sendPlayerInput(SOCKET clientSocket, const char* keyBuffer) {
	// Ű ���� ������ ����
	send(clientSocket, keyBuffer, BUFSIZE, 0);
}

void receiveData(SOCKET clientSocket) {
	char buffer[BUFSIZE];
	int bytesRead;

	// �����κ��� �����͸� ����
	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
		// ���� �����͸� ó���ϴ� ������ �߰��ؾ� ��
		printf("���� ������: %s\n", buffer);
	}
}

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
		err_quit("connect()");
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("������ ���� ����\n");

	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");

		// esc Ű�� ������ ������ Ż��
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;
		}

		// Ű �Է� ����
		if (_kbhit()) {
			// Ű �Է� ����
			char keyBuffer[BUFSIZE] = "0000000"; // �ʱ�ȭ

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
			sendPlayerInput(sock, keyBuffer);
		}
		else 
		{
			char keyBuffer[BUFSIZE];
			snprintf(keyBuffer, BUFSIZE, "%d", 0); // 0�� ���ڿ��� ��ȯ�Ͽ� ���ۿ� ����
			printf("key : %s\n", keyBuffer);

			// Ű �Է��� ������ ����
			sendPlayerInput(sock, keyBuffer);
		}

		receiveData(sock);

		// ���� �ð� ���� ���
		Sleep(100);  // 100 �и��� ���
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
