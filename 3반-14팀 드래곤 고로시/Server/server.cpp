#include "ServerCharacter.h"
#include "TCPServer.h"

#define SERVERPORT 9000
#define KEYBUFSIZE    8


HWND hWnd;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

ThreadParams p1ThreadParams, p2ThreadParams;
// THREAD HANDLE
extern HANDLE hP1Thread, hP2Thread;
HANDLE hUpdateThread;

// EVENT HANDLE
extern HANDLE hPlayer1Input, hPlayer2Input;
extern HANDLE hPlayer1Update, hPlayer2Update;
extern HANDLE hMainUpdate;

//메인 캐릭터 및 총알
extern MainCharacter mainPlayer1, mainPlayer2;
extern BulletBitmap bulletBitmap;

//보스
extern BossMonster Boss;
extern BossBitData bossBitData;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //윈도우 프로시저 프로토선언 

void processPlayerInput(SOCKET client_sock, const char* keyBuffer)
{
	// 데이터 보내기 (이 예제에서는 받은 데이터를 그대로 클라이언트에게 다시 보냄)
	send(client_sock, keyBuffer, KEYBUFSIZE, 0);
}

DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	hPlayer1Input = CreateEvent(NULL, FALSE, FALSE, NULL);
	hPlayer1Update = CreateEvent(NULL, FALSE, FALSE, NULL);
	hMainUpdate = CreateEvent(NULL, FALSE, FALSE, NULL);

	hUpdateThread = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	struct sockaddr_in clientaddr;
	int addrlen;

	int nPlayer = 1;
	while (1)
	{
		// 일단은 p1부터 받아보자
		addrlen = sizeof(clientaddr);
		if (nPlayer == 1)
		{
			// accept()
			p1ThreadParams.socket = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
			if (p1ThreadParams.socket == INVALID_SOCKET)
			{
				err_display("accept()");
				break;
			}
			srand(time(NULL));
			SetTimer(hWnd, 1, 15, NULL);
			SetTimer(hWnd, 5, 90, NULL);
			SetTimer(hWnd, 10, 100, NULL);

		}
		else if (nPlayer == 2)
		{
			// accept()
			p2ThreadParams.socket = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
			if (p2ThreadParams.socket == INVALID_SOCKET)
			{
				err_display("accept()");
				break;
			}
		}

		if (nPlayer == 1)
		{
			// 스레드 생성
			p1ThreadParams.hThread = hP1Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p1ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p1ThreadParams.socket);
			}
		}
		else if (nPlayer == 2)
		{
			// 스레드 생성
			p2ThreadParams.hThread = hP2Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p2ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p2ThreadParams.socket);
			}
		}
		++nPlayer;
	}

	closesocket(listen_sock);	// 소켓 닫기
	WSACleanup();				// 윈속 종료
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;			//윈도우 메시지 선언
	WNDCLASSEX WndClass;	//윈도우 클래스 선언 ->생성하느 윈도우의 형태를 설정하기위한 구조체
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);								//본 구조체의 크기
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//출력 스타일
	WndClass.lpfnWndProc = (WNDPROC)WndProc;						//프로시저 함수의 이름
	WndClass.cbClsExtra = 0;										//클래스 여분 메모리--사용은 안함
	WndClass.cbWndExtra = 0;										//윈도우 여분 메모리--마찬가지로 사용은 안함
	WndClass.hInstance = hInstance;									//윈도우 인스턴스 ->이 윈도우 클래스를 사용하는 프로그램의 인스터스 값,WinMain함수의 인수로 전달된 hInstance 값 사용
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//아이콘(작업창 아이콘 바뀜)
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//커서(커서 바뀜)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//배경색
	WndClass.lpszMenuName = NULL;									//메뉴이름
	WndClass.lpszClassName = lpszClass;								//클래스 이름
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				//작은 아이콘,캡션 아이콘
	RegisterClassEx(&WndClass);										//생성한 윈도우 클래스를 운영체제에 등록한다.

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 600, NULL, (HMENU)NULL, hInstance, NULL);

	//// 윈속 초기화
	//WSADATA wsa;
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	return 1;
	// 이벤트 생성
	// tbd

	// 소켓 통신 스레드 생성
	hNetworkThread = CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	CloseHandle(hPlayer1Input);
	CloseHandle(hPlayer2Input);
	CloseHandle(hPlayer1Update);
	CloseHandle(hPlayer2Update);
	CloseHandle(hMainUpdate);

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK 함수는 WinMain에서 호출하는 것이 아닌 윈도우 OS가 호출한다.
{
	static RECT rect;
	static BOOL potalOn = FALSE;

	static MainCharacter* tailTarget;

	static int victoryNum = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		Boss.rect = { 634, 50, 984, 561 };
		
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			BossAttackLoop(rect, &Boss, &mainPlayer1, &mainPlayer2); //oldState 수정필요
			MainLoop(rect, mainPlayer1, Boss, bulletBitmap);
			MainLoop(rect, mainPlayer2, Boss, bulletBitmap);
 
			// 보스 공격 사각형 설정
			SetBossAndBossAttackRect(Boss, bossBitData);
			// 이제 이벤트를 발생시키자
			SetEvent(hMainUpdate);

			//InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:		//꼬리 공격 준비 할 텀 타임
			//그냥 0.01초 쉬는 용도인듯?
			SetTimer(hWnd, 3, 10, NULL);
			// 여기서 타겟 플레이어 설정하자
			if(mainPlayer2.info.type == 0)
			{
				tailTarget = &mainPlayer1;
			}
			else if (rand() % 2 == 0)
			{
				tailTarget = &mainPlayer1;
			}
			else
			{
				tailTarget = &mainPlayer2;
			}
			KillTimer(hWnd, 2);
			break;
		case 3:		//꼬리 공격 준비 타임
			Boss.AttackTailReady = TRUE;
			BossAttackTail(hWnd, rect, tailTarget, &Boss); //보스 꼬리 공격 구조 + oldState바꾸기
			break;
		case 5:
			IdleAndShootStateMainChar(mainPlayer1, bulletBitmap);
			IdleAndShootStateMainChar(mainPlayer2, bulletBitmap);
			break;
		case 10:
			BossStateChange(&Boss, hWnd, rect);
			BossAttackStateChange(&Boss, rect);
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			mainPlayer1.info.right = TRUE;
			mainPlayer1.info.direction = TRUE;
			break;
		case VK_LEFT:
			mainPlayer1.info.left = TRUE;
			mainPlayer1.info.direction = FALSE;
			break;
		case VK_SPACE:
			if (mainPlayer1.info.state != MainState::EXSHOOT && mainPlayer1.info.state != MainState::HIT)
			{
				mainPlayer1.info.state = MainState::JUMP;
			}
			break;
		case VK_CONTROL:
			if (mainPlayer1.info.state != MainState::JUMP && mainPlayer1.info.state != MainState::HIT)
			{
				if (mainPlayer1.info.state == MainState::RUN)
				{
					mainPlayer1.info.state = MainState::RUNSHOOT;
				}
				else if (mainPlayer1.info.state == MainState::IDLE)
				{
					mainPlayer1.info.state = MainState::SHOOT;
				}
			}
			break;
		case VK_SHIFT:
			if (mainPlayer1.info.state != MainState::JUMP && mainPlayer1.info.state != MainState::HIT)
			{
				mainPlayer1.info.state = MainState::EXSHOOT;
			}
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 5);
		KillTimer(hWnd, 10);


		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}