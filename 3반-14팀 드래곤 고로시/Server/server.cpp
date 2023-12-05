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

//���� ĳ���� �� �Ѿ�
extern MainCharacter mainPlayer1, mainPlayer2;
extern BulletBitmap bulletBitmap;

//����
extern BossMonster Boss;
extern BossBitData bossBitData;

extern char p1KeyBuffer[KEYBUFSIZE];
extern char p1OldKeyBuffer[KEYBUFSIZE];
extern char p2KeyBuffer[KEYBUFSIZE];
extern char p2OldKeyBuffer[KEYBUFSIZE];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //������ ���ν��� �����伱�� 

void processPlayerInput(SOCKET client_sock, const char* p1KeyBuffer)
{
	// ������ ������ (�� ���������� ���� �����͸� �״�� Ŭ���̾�Ʈ���� �ٽ� ����)
	send(client_sock, p1KeyBuffer, KEYBUFSIZE, 0);
}

DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	hPlayer1Input = CreateEvent(NULL, FALSE, FALSE, NULL);
	hPlayer1Update = CreateEvent(NULL, FALSE, FALSE, NULL);
	hMainUpdate = CreateEvent(NULL, FALSE, FALSE, NULL);

	hUpdateThread = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
	struct sockaddr_in clientaddr;
	int addrlen;

	int nPlayer = 1;
	while (1)
	{
		// �ϴ��� p1���� �޾ƺ���
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
			// ������ ����
			p1ThreadParams.hThread = hP1Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p1ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p1ThreadParams.socket);
			}
		}
		else if (nPlayer == 2)
		{
			// ������ ����
			p2ThreadParams.hThread = hP2Thread = CreateThread(NULL, 0, NetworkThread, (LPVOID)&p2ThreadParams, 0, NULL);
			if (p1ThreadParams.hThread == NULL)
			{
				closesocket(p2ThreadParams.socket);
			}
		}
		++nPlayer;
	}

	closesocket(listen_sock);	// ���� �ݱ�
	WSACleanup();				// ���� ����
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;			//������ �޽��� ����
	WNDCLASSEX WndClass;	//������ Ŭ���� ���� ->�����ϴ� �������� ���¸� �����ϱ����� ����ü
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);								//�� ����ü�� ũ��
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//��� ��Ÿ��
	WndClass.lpfnWndProc = (WNDPROC)WndProc;						//���ν��� �Լ��� �̸�
	WndClass.cbClsExtra = 0;										//Ŭ���� ���� �޸�--����� ����
	WndClass.cbWndExtra = 0;										//������ ���� �޸�--���������� ����� ����
	WndClass.hInstance = hInstance;									//������ �ν��Ͻ� ->�� ������ Ŭ������ ����ϴ� ���α׷��� �ν��ͽ� ��,WinMain�Լ��� �μ��� ���޵� hInstance �� ���
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������(�۾�â ������ �ٲ�)
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Ŀ��(Ŀ�� �ٲ�)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//����
	WndClass.lpszMenuName = NULL;									//�޴��̸�
	WndClass.lpszClassName = lpszClass;								//Ŭ���� �̸�
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				//���� ������,ĸ�� ������
	RegisterClassEx(&WndClass);										//������ ������ Ŭ������ �ü���� ����Ѵ�.

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 600, NULL, (HMENU)NULL, hInstance, NULL);

	//// ���� �ʱ�ȭ
	//WSADATA wsa;
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	return 1;
	// �̺�Ʈ ����
	// tbd

	// ���� ��� ������ ����
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK �Լ��� WinMain���� ȣ���ϴ� ���� �ƴ� ������ OS�� ȣ���Ѵ�.
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
			BossAttackLoop(rect, &Boss, &mainPlayer1, &mainPlayer2); //oldState �����ʿ�

			if ( (strcmp(p1OldKeyBuffer, p1KeyBuffer) != 0) )
			{

				if (p1KeyBuffer[1] == '1')				//	���� ȭ��ǥŰ
				{
					mainPlayer1.info.left = TRUE;
					mainPlayer1.info.direction = FALSE;
				}
				else if (p1KeyBuffer[2] == '1')		// ������ ȭ��ǥ Ű
				{
					mainPlayer1.info.right = TRUE;
					mainPlayer1.info.direction = TRUE;
				}
				if (p1KeyBuffer[4] == '1')				// Space Ű
				{
					if (mainPlayer1.info.state != MainState::EXSHOOT && mainPlayer1.info.state != MainState::HIT)
					{
						mainPlayer1.info.state = MainState::JUMP;
					}
				}
				if (p1KeyBuffer[5] == '1')			// Shift Ű
				{
					if (mainPlayer1.info.state != MainState::JUMP && mainPlayer1.info.state != MainState::HIT)
					{
						mainPlayer1.info.state = MainState::EXSHOOT;
					}
				}
				if (p1KeyBuffer[6] == '1')			// Ctrl Ű
				{
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
				}
			}

			if ((strcmp(p2OldKeyBuffer, p2KeyBuffer) != 0))
			{

				if (p2KeyBuffer[1] == '1')				//	���� ȭ��ǥŰ
				{
					mainPlayer2.info.left = TRUE;
					mainPlayer2.info.direction = FALSE;
				}
				else if (p2KeyBuffer[2] == '1')		// ������ ȭ��ǥ Ű
				{
					mainPlayer2.info.right = TRUE;
					mainPlayer2.info.direction = TRUE;
				}
				if (p2KeyBuffer[4] == '1')				// Space Ű
				{
					if (mainPlayer2.info.state != MainState::EXSHOOT && mainPlayer2.info.state != MainState::HIT)
					{
						mainPlayer2.info.state = MainState::JUMP;
					}
				}
				if (p2KeyBuffer[5] == '1')			// Shift Ű
				{
					if (mainPlayer2.info.state != MainState::JUMP && mainPlayer2.info.state != MainState::HIT)
					{
						mainPlayer2.info.state = MainState::EXSHOOT;
					}
				}
				if (p2KeyBuffer[6] == '1')			// Ctrl Ű
				{
					if (mainPlayer2.info.state != MainState::JUMP && mainPlayer2.info.state != MainState::HIT)
					{
						if (mainPlayer2.info.state == MainState::RUN)
						{
							mainPlayer2.info.state = MainState::RUNSHOOT;
						}
						else if (mainPlayer2.info.state == MainState::IDLE)
						{
							mainPlayer2.info.state = MainState::SHOOT;
						}
					}
				}
			}


			MainLoop(rect, mainPlayer1, Boss, bulletBitmap, p1KeyBuffer);
			MainLoop(rect, mainPlayer2, Boss, bulletBitmap, p2KeyBuffer);
 
			// ���� ���� �簢�� ����
			SetBossAndBossAttackRect(Boss, bossBitData);
			// ���� �̺�Ʈ�� �߻���Ű��
			SetEvent(hMainUpdate);
			break;
		case 2:		//���� ���� �غ� �� �� Ÿ��
			//�׳� 0.01�� ���� �뵵�ε�?
			SetTimer(hWnd, 3, 10, NULL);
			// ���⼭ Ÿ�� �÷��̾� ��������
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
		case 3:		//���� ���� �غ� Ÿ��
			Boss.AttackTailReady = TRUE;
			BossAttackTail(hWnd, rect, tailTarget, &Boss); //���� ���� ���� ���� + oldState�ٲٱ�
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

	/*case WM_KEYDOWN:
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
		break;*/
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