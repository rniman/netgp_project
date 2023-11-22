#include "TCPClient.h"

MainCharacter mainCharacter;
BossMonster Boss;

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
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int SendInputData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, 
	BossMonster& boss, const char* keyBuffer)
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
	// 키 값을 서버로 전송
	send(clientSocket, keyBuffer, KEYBUFSIZE, 0);
}

void receiveData(SOCKET clientSocket) {
	char buffer[KEYBUFSIZE];
	int bytesRead;

	// 서버로부터 데이터를 읽음
	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead > 0) {
		// 받은 데이터를 처리하는 로직을 추가해야 함
		printf("받은 데이터: %s\n", buffer);
	}
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

	// BITMAP WIDTH, HEIGHT 송신
	// tbd

	// INIT 수신
	// tbd

	// 서버와 데이터 통신
	while (1)
	{
		// 데이터 입력
		printf("\n[보낼 데이터] ");

		// esc 키를 누르면 루프를 탈출
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;
		}

		// 키 입력 감지
		if (_kbhit()) {
			// 키 입력 감지
			char keyBuffer[KEYBUFSIZE] = "0000000"; // 초기화

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
			if (SendInputData(sock, mainCharacter, Boss, keyBuffer) == -1)
			{
				err_quit("send()");
			}
			else
			{
				char keyBuffer[KEYBUFSIZE];
				snprintf(keyBuffer, KEYBUFSIZE, "%d", 0); // 0을 문자열로 변환하여 버퍼에 저장
				printf("key : %s\n", keyBuffer);

				// 키 입력을 서버로 전송
				if (SendInputData(sock, mainCharacter, Boss, keyBuffer) == -1)
				{
					err_quit("send()");
				}
			}

			receiveData(sock);

			// 일정 시간 동안 대기
			Sleep(100);  // 100 밀리초 대기
		}

		//// INPUT 송신
		//// tbd
		//if (SendInputData(sock, mainCharacter/*, mainCharacter*/, Boss) == -1)
		//{
		//	//오류
		//	err_quit("send()");
		//}

		//// UPDATE 수신
		//// tbd
		//RecvDefaultData(sock, mainCharacter/*, mainCharacter*/, Boss);
	}

	return 0;
}
