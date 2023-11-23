#include "ClientCharacter.h"
#include "TCPClient.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

//���� ĳ���� �� �Ѿ�
extern MainCharacter mainCharacter;
BULLETBITMAP bulletBitmap;
MainState oldState;
int oldAnimationNum;
int jumpTime = 0, coolTime = 0, invincibleTime = 0, responTime = 0;

//����
extern BossMonster Boss;
extern BOSSCIMAGE bossImage;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //������ ���ν��� �����伱�� 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;				//������ �ڵ鰪 ����
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
	//CreateWindow(������Ŭ�����̸�,������ Ÿ��Ʋ �̸�,������ ��Ÿ��,������ x��ǥ,������ y��ǥ,������ ����ũ��,������ ����ũ��,�θ� ������ �ڵ�, �޴� �ڵ�, �������α׷� �ν��Ͻ�, ���� ������ ����)

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// �̺�Ʈ ����
	// tbd

	// ���� ��� ������ ����
	hNetworkThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK �Լ��� WinMain���� ȣ���ϴ� ���� �ƴ� ������ OS�� ȣ���Ѵ�.
{
	HDC hDC;
	PAINTSTRUCT ps;
	static HDC backMemDC, ObjectDC;
	static HBITMAP backHBITMAP;
	static BITMAP backGroundData;

	static RECT rect;
	static BOOL potalOn = FALSE;

	//Stage5
	static CImage BossGround[3];
	//static BossMonster Boss;
	//static STAGE4SPHERE* BossMeteorHead = new STAGE4SPHERE;

	static CImage victory[27];
	static int victoryNum = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rect);

		CreateMainChar(g_hInst, &mainCharacter);

		hDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(hDC);
		ObjectDC = CreateCompatibleDC(backMemDC);
		backHBITMAP = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(backMemDC, backHBITMAP);

		CREATESTAGE5(&Boss, &bossImage, BossGround, rect);

		TCHAR temp[30];
		for (int i = 0; i < 27; ++i) {
			wsprintf(temp, L"victory/victory%d.png", i + 1);
			victory[i].Load(temp);
		}

		LoadBullet(&bulletBitmap, g_hInst);

		ReleaseDC(hWnd, hDC);
		SetTimer(hWnd, 1, 15, NULL);
		SetTimer(hWnd, 5, 90, NULL);
		SetTimer(hWnd, 10, 100, NULL);

		PlaySoundA("sound/stage5.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		//PlaySoundA("sound/stage1.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);

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

			//��Ʈ�ڽ� ����
			HitBoxMainChar(&mainCharacter);

			//��Ÿ�� 
			if (coolTime > 0)coolTime--;
			else coolTime = 0;

			BossAttackMeteor(rect, &Boss, &mainCharacter, &oldState, &oldAnimationNum, &invincibleTime); //oldState �����ʿ�

			// �ǰ� Ȯ�� �� ���� 6���� ��ȯ

			if (invincibleTime != 0)invincibleTime--;

			if (mainCharacter.info.heart <= 0) 
			{
				if (responTime == 0)responTime = 500;

				if (responTime > 280) {
					mainCharacter.info.animationNum++;
					if (mainCharacter.info.animationNum > 23)mainCharacter.info.animationNum = 0;
					responTime--;
				}
				else {
					if (responTime == 280)
						mainCharacter.info.animationNum = 0;
					else if (responTime % 10 == 0)
						mainCharacter.info.animationNum++;
					responTime -= 5;
				}


				if (responTime == 0) {
					mainCharacter.info.heart = 6;

					if (oldState != MainState::JUMP) mainCharacter.info.state = MainState::IDLE;
					else mainCharacter.info.state = MainState::JUMP;

					mainCharacter.info.animationNum = 0;
					mainCharacter.info.right = FALSE;
					mainCharacter.info.left = FALSE;
					mainCharacter.info.direction = TRUE;
					mainCharacter.info.energy = -1;
					invincibleTime = 100;
				}
			}
			else if (mainCharacter.info.state == MainState::HIT) 
			{
				if (invincibleTime % 10 == 0)
					mainCharacter.info.animationNum++;

				if (mainCharacter.info.animationNum == 5)
				{
					if (oldState == MainState::EXSHOOT)
						mainCharacter.info.state = MainState::IDLE;
					else
						mainCharacter.info.state = oldState;
					mainCharacter.info.animationNum = oldAnimationNum;
				}
			}
			else if (mainCharacter.info.state == MainState::JUMP)
			{	//����
				mainCharacter.info.animationNum++;
				//MoveMainChar(&mainCharacter, rect);
				JumpMainChar(&mainCharacter, jumpTime, rect);
				if (GetKeyState(VK_CONTROL) < 0 && coolTime == 0) 
				{
					CreateBullet(mainCharacter, bulletBitmap); 
					coolTime = 18;
				}

				if (mainCharacter.info.animationNum > 7)
				{
					mainCharacter.info.animationNum = 0;
				}
				jumpTime++;
				
				//���� �� ���Ͻ� ������ ������ ���ǿ� ã��
				if (jumpTime >= 40) 
				{ 
					mainCharacter.info.state = MainState::IDLE; 
					jumpTime = 0;
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
					if (coolTime == 0) 
					{
						CreateBullet(mainCharacter, bulletBitmap);
						coolTime = 18;
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

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:		//���� ���� �غ� �� �� Ÿ��
			//�׳� 1�� ���� �뵵�ε�?
			SetTimer(hWnd, 3, 10, NULL);
			//Boss.AttackTailReady = TRUE;
			KillTimer(hWnd, 2);
			break;
		case 3:		//���� ���� �غ� Ÿ��
			Boss.AttackTailReady = TRUE;
			BossAttackTail(hWnd, rect, &mainCharacter, &Boss, &bossImage, &oldState, &oldAnimationNum, &invincibleTime); //���� ���� ���� ���� + oldState�ٲٱ�
			break;
		case 5:
			//IDLE �� IDLE SHOOT ����
			/*if (mainCharacter.info.state == MainState::IDLE) 
			{
				mainCharacter.info.animationNum++;
				if (mainCharacter.info.animationNum > 4)
				{
					mainCharacter.info.animationNum = 0;
				}
			}
			else*/ if (mainCharacter.info.state == MainState::SHOOT)
			{
				mainCharacter.info.animationNum++;

				if (mainCharacter.info.animationNum == 1)
				{
					CreateBullet(mainCharacter, bulletBitmap);
					coolTime = 18;
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
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetStretchBltMode(backMemDC, HALFTONE);

		PaintBoss(backMemDC, rect, &Boss, &bossImage, BossGround);

		if (Boss.HP <= 0) {
			if (victoryNum == 0)PlaySoundA("sound/announcer_knockout_0004.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			if (victoryNum >= 27)victoryNum = 0;
			victory[victoryNum++].TransparentBlt(backMemDC, 0, 0, rect.right, rect.bottom, RGB(255, 0, 255));
		}

		//ĳ���� �׸�
		if (mainCharacter.info.heart <= 0)PaintGhost(backMemDC, ObjectDC, mainCharacter, responTime);
		else if (mainCharacter.info.state == MainState::IDLE || mainCharacter.info.state == MainState::RUN) PaintMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.info.state == MainState::JUMP) PaintJump(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.info.state == MainState::SHOOT || mainCharacter.info.state == MainState::RUNSHOOT)PaintShootMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.info.state == MainState::EXSHOOT) PaintEXShoot(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.info.state == MainState::HIT)PaintHIT(backMemDC, ObjectDC, mainCharacter);

		PaintHeart(backMemDC, ObjectDC, mainCharacter);
		PaintBullet(backMemDC, ObjectDC, mainCharacter, bulletBitmap);
		PaintDeathBullet(backMemDC, ObjectDC, mainCharacter, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
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

		DeleteBitBullet(&bulletBitmap);
		DeleteMainChar(&mainCharacter);

		DeleteObject(backHBITMAP);

		DeleteDC(ObjectDC);
		DeleteDC(backMemDC);
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}