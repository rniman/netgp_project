#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <conio.h>  // _getch 함수를 사용하기 위한 헤더
#include <windows.h>

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000
#define BUFSIZE    8

char* SERVERIP = (char*)"127.0.0.1";

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	// 키 값을 서버로 전송
	send(clientSocket, keyBuffer, BUFSIZE, 0);
}

void receiveData(SOCKET clientSocket) {
	char buffer[BUFSIZE];
	int bytesRead;

	// 서버로부터 데이터를 읽음
	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
		// 받은 데이터를 처리하는 로직을 추가해야 함
		printf("받은 데이터: %s\n", buffer);
	}
}

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	printf("서버에 연결 성공\n");

	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");

		// esc 키를 누르면 루프를 탈출
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;
		}

		// 키 입력 감지
		if (_kbhit()) {
			// 키 입력 감지
			char keyBuffer[BUFSIZE] = "0000000"; // 초기화

			// 화살표 키
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

			// Space 키
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				keyBuffer[4] = '1';
			}

			// Shift 키
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
				keyBuffer[5] = '1';
			}

			// Ctrl 키
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				keyBuffer[6] = '1';
			}

			printf("keyBuffer : %s\n", keyBuffer);

			// 키 입력을 서버로 전송
			sendPlayerInput(sock, keyBuffer);
		}
		else 
		{
			char keyBuffer[BUFSIZE];
			snprintf(keyBuffer, BUFSIZE, "%d", 0); // 0을 문자열로 변환하여 버퍼에 저장
			printf("key : %s\n", keyBuffer);

			// 키 입력을 서버로 전송
			sendPlayerInput(sock, keyBuffer);
		}

		receiveData(sock);

		// 일정 시간 동안 대기
		Sleep(100);  // 100 밀리초 대기
	}

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
