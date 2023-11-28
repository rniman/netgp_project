#include "ServerCharacter.h"
#include "TCPServer.h"

#define SERVERPORT 9000

HWND hWnd;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

extern MainCharacter mainCharacter, p2;
MainCharacter* tailTarget;
extern BossMonster bossMob;
extern BossBitData bossBitData;

ThreadParams p1ThreadParams, p2ThreadParams;
// THREAD HANDLE
extern HANDLE hP1Thread, hP2Thread;
HANDLE hUpdateThread;

// EVENT HANDLE
extern HANDLE hPlayer1Input, hPlayer2Input;
extern HANDLE hPlayer1Update, hPlayer2Update;
extern HANDLE hMainUpdate;

//���� ĳ���� �� �Ѿ�
extern BulletBitmap bulletBitmap;
//MainState oldState;
//int oldAnimationNum;

//����
extern BossMonster Boss;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //������ ���ν��� �����伱�� 

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
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK �Լ��� WinMain���� ȣ���ϴ� ���� �ƴ� ������ OS�� ȣ���Ѵ�.
{
	static RECT rect;
	static BOOL potalOn = FALSE;

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
			//����Ű Ȯ��
			if (GetKeyState(VK_RIGHT) >= 0 && mainCharacter.info.right)
			{
				mainCharacter.info.right = FALSE;
			}
			if (GetKeyState(VK_LEFT) >= 0 && mainCharacter.info.left)
			{
				mainCharacter.info.left = FALSE;
			}

			HitBoxMainChar(&mainCharacter);

			//��Ÿ�� 
			if (mainCharacter.info.coolTime > 0)mainCharacter.info.coolTime--;
			else mainCharacter.info.coolTime = 0;

			BossAttackMeteor(rect, &Boss, &mainCharacter); //oldState �����ʿ�

			if (mainCharacter.info.invincibleTime != 0)
			{
				mainCharacter.info.invincibleTime--;
			}

			if (mainCharacter.info.heart <= 0)
			{
				if (mainCharacter.info.responeTime == 0)
				{
					mainCharacter.info.responeTime = 500;
				}

				if (mainCharacter.info.responeTime > 280)
				{
					mainCharacter.info.animationNum++;
					if (mainCharacter.info.animationNum > 23)
					{
						mainCharacter.info.animationNum = 0;
					}
					mainCharacter.info.responeTime--;
				}
				else
				{
					if (mainCharacter.info.responeTime == 280)
					{
						mainCharacter.info.animationNum = 0;
					}
					else if (mainCharacter.info.responeTime % 10 == 0)
					{
						mainCharacter.info.animationNum++;
					}
					mainCharacter.info.responeTime -= 5;
				}


				if (mainCharacter.info.responeTime == 0)
				{
					mainCharacter.info.heart = 6;

					if (mainCharacter.info.oldState != MainState::JUMP) mainCharacter.info.state = MainState::IDLE;
					else mainCharacter.info.state = MainState::JUMP;

					mainCharacter.info.animationNum = 0;
					mainCharacter.info.right = FALSE;
					mainCharacter.info.left = FALSE;
					mainCharacter.info.direction = TRUE;
					mainCharacter.info.energy = -1;
					mainCharacter.info.invincibleTime = 100;
				}
			}
			else if (mainCharacter.info.state == MainState::HIT)
			{
				if (mainCharacter.info.invincibleTime % 10 == 0)
					mainCharacter.info.animationNum++;

				if (mainCharacter.info.animationNum == 5)
				{
					if (mainCharacter.info.oldState == MainState::EXSHOOT)
						mainCharacter.info.state = MainState::IDLE;
					else
						mainCharacter.info.state = mainCharacter.info.oldState;
					mainCharacter.info.animationNum = mainCharacter.info.oldAnimationNum;
				}
			}
			else if (mainCharacter.info.state == MainState::JUMP)
			{	//����
				mainCharacter.info.animationNum++;
				JumpMainChar(&mainCharacter, rect);
				if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.info.coolTime == 0)
				{
					CreateBullet(mainCharacter, bulletBitmap);
					mainCharacter.info.coolTime = 18;
				}

				if (mainCharacter.info.animationNum > 7)
				{
					mainCharacter.info.animationNum = 0;
				}
				mainCharacter.info.jumpTime++;

				//���� �� ���Ͻ� ������ ������ ���ǿ� ã��
				if (mainCharacter.info.jumpTime >= 40)
				{
					mainCharacter.info.state = MainState::IDLE;
					mainCharacter.info.jumpTime = 0;
				}
			}
			else if (mainCharacter.info.state == MainState::EXSHOOT)
			{
				mainCharacter.info.energy++;
				if (mainCharacter.info.energy == 0)mainCharacter.info.animationNum = 0;
				else if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < 4)mainCharacter.info.animationNum++;

				if (GetKeyState(VK_SHIFT) >= 0)
				{
					if (mainCharacter.info.animationNum < 4)
					{
						mainCharacter.info.state = MainState::IDLE;
						mainCharacter.info.energy = -1;
					}
					else if (mainCharacter.info.animationNum == 4)
					{
						CreateBullet(mainCharacter, bulletBitmap);
						mainCharacter.info.animationNum++;
					}
				}

				if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < EXSHOOTANI - 1 && mainCharacter.info.animationNum > 4)
				{
					mainCharacter.info.animationNum++;
				}
				else if (mainCharacter.info.animationNum == EXSHOOTANI - 1)
				{
					mainCharacter.info.state = MainState::IDLE;
					mainCharacter.info.energy = -1;
				}
			}
			else if (mainCharacter.info.left && mainCharacter.info.right)
			{
				if (GetKeyState(VK_CONTROL) >= 0)
				{
					mainCharacter.info.state = MainState::IDLE;
				}
				else
				{
					mainCharacter.info.state = MainState::SHOOT;
				}

				mainCharacter.info.Pos.right = mainCharacter.info.Pos.left + IDLEWIDTH;
			}
			else if (mainCharacter.info.right || mainCharacter.info.left)
			{
				mainCharacter.info.animationNum++;
				if (GetKeyState(VK_CONTROL) >= 0)
				{
					mainCharacter.info.state = MainState::RUN;
				}
				else
				{
					if (mainCharacter.info.coolTime == 0)
					{
						CreateBullet(mainCharacter, bulletBitmap);
						mainCharacter.info.coolTime = 18;
					}
					mainCharacter.info.state = MainState::RUNSHOOT;
				}

				MoveMainChar(&mainCharacter, rect);
				if (mainCharacter.info.animationNum > 15)
				{
					mainCharacter.info.animationNum = 0;
				}
			}

			//���� ��ȯ
			if (mainCharacter.info.state == MainState::HIT || mainCharacter.info.heart <= 0) {}
			else if (GetKeyState(VK_RIGHT) < 0)mainCharacter.info.direction = TRUE;
			else if (GetKeyState(VK_LEFT) < 0)mainCharacter.info.direction = FALSE;
			else if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.info.state != MainState::JUMP) mainCharacter.info.state = MainState::SHOOT;
			else if (mainCharacter.info.state != MainState::JUMP && mainCharacter.info.state != MainState::EXSHOOT) mainCharacter.info.state = MainState::IDLE;

			MoveBullet(mainCharacter, rect);
			DeathBullet(mainCharacter);

			// ���� ���� �簢�� ����
			SetBossAndBossAttackRect(Boss, bossBitData);
			// ���� �̺�Ʈ�� �߻���Ű��
			SetEvent(hMainUpdate);

			//InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:		//���� ���� �غ� �� �� Ÿ��
			//�׳� 0.01�� ���� �뵵�ε�?
			SetTimer(hWnd, 3, 10, NULL);
			// ���⼭ Ÿ�� �÷��̾� ��������
			if (rand() % 2 == 0)
			{
				tailTarget = &mainCharacter;
			}
			else
			{
				tailTarget = &mainCharacter;
				//tailTarget = &p2;
			}
			//Boss.AttackTailReady = TRUE;
			KillTimer(hWnd, 2);
			break;
		case 3:		//���� ���� �غ� Ÿ��
			Boss.AttackTailReady = TRUE;
			BossAttackTail(hWnd, rect, tailTarget, &Boss); //���� ���� ���� ���� + oldState�ٲٱ�
			break;
		case 5:
			//IDLE �� IDLE SHOOT ����
			if (mainCharacter.info.state == MainState::IDLE)
			{
				mainCharacter.info.animationNum++;
				if (mainCharacter.info.animationNum > 4)
				{
					mainCharacter.info.animationNum = 0;
				}
			}
			else if (mainCharacter.info.state == MainState::SHOOT)
			{
				mainCharacter.info.animationNum++;

				if (mainCharacter.info.animationNum == 1)
				{
					CreateBullet(mainCharacter, bulletBitmap);
					mainCharacter.info.coolTime = 18;
				}

				if (mainCharacter.info.animationNum > 2)
				{
					mainCharacter.info.animationNum = 0;
				}
			}
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
			mainCharacter.info.right = TRUE;
			mainCharacter.info.direction = TRUE;
			break;
		case VK_LEFT:
			mainCharacter.info.left = TRUE;
			mainCharacter.info.direction = FALSE;
			break;
		case VK_SPACE:
			if (mainCharacter.info.state != MainState::EXSHOOT && mainCharacter.info.state != MainState::HIT)
			{
				mainCharacter.info.state = MainState::JUMP;
			}
			break;
		case VK_CONTROL:
			if (mainCharacter.info.state != MainState::JUMP && mainCharacter.info.state != MainState::HIT)
			{
				if (mainCharacter.info.state == MainState::RUN)
				{
					mainCharacter.info.state = MainState::RUNSHOOT;
				}
				else if (mainCharacter.info.state == MainState::IDLE)
				{
					mainCharacter.info.state = MainState::SHOOT;
				}
			}
			break;
		case VK_SHIFT:
			if (mainCharacter.info.state != MainState::JUMP && mainCharacter.info.state != MainState::HIT)
			{
				mainCharacter.info.state = MainState::EXSHOOT;
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