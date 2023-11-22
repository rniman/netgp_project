<<<<<<< Updated upstream
#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#include "character.h"

=======
#include "TCPClient.h"
>>>>>>> Stashed changes

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //윈도우 프로시저 프로토선언 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;				//윈도우 핸들값 선언
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
	//CreateWindow(윈도우클래스이름,윈도우 타이틀 이름,윈도우 스타일,윈도우 x좌표,윈도우 y좌표,윈도우 가로크기,윈도우 세로크리,부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보)

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK 함수는 WinMain에서 호출하는 것이 아닌 윈도우 OS가 호출한다.
{
	HDC hDC;
	PAINTSTRUCT ps;
	static HDC backMemDC, ObjectDC;
	static HBITMAP backHBITMAP;
	static BITMAP backGroundData;

	static RECT rect;
	static int Stage = 5;
	static BOOL potalOn = FALSE;

	//메인 캐릭터 및 총알
	static MainCharacter mainCharacter;
	static BULLETBITMAP bulletBitmap;
	static BLinkedList* bullet;
	static int oldState, oldAnimationNum;
	static int jumpTime = 0, coolTime = 0, invincibleTime = 0, responTime = 0;

	//STAGE 5
	static CImage BossGround[3];
	static BossMonster Boss;
	static STAGE4SPHERE* BossMeteorHead = new STAGE4SPHERE;

	static CImage victory[27];
	static int victoryNum = 0;

	//static ImageOfMonster IMob;

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rect);

		CreateMainChar(g_hInst, &mainCharacter);

		hDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(hDC);
		ObjectDC = CreateCompatibleDC(backMemDC);
		backHBITMAP = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(backMemDC, backHBITMAP);

		CREATESTAGE5(&Boss, BossGround, rect);

		TCHAR temp[30];
		for (int i = 0; i < 27; ++i) {
			wsprintf(temp, L"victory/victory%d.png", i + 1);
			victory[i].Load(temp);
		}

		LoadBullet(&bulletBitmap, g_hInst);

		bullet = CreateBList();

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
			//방향키 확인
			if (GetKeyState(VK_RIGHT) >= 0 && mainCharacter.right) {
				mainCharacter.right = FALSE;
			}
			if (GetKeyState(VK_LEFT) >= 0 && mainCharacter.left) {
				mainCharacter.left = FALSE;
			}

			//히트박스 수정
			HitBoxMainChar(&mainCharacter);

			//쿨타임 
			if (coolTime > 0)coolTime--;
			else coolTime = 0;

			if (Stage == 5) {
				BossAttackMeteor(rect, &Boss, BossMeteorHead, bullet, &mainCharacter, &oldState, &oldAnimationNum, &invincibleTime);
			}

			// 피격 확인 후 상태 6으로 전환

			if (invincibleTime != 0)invincibleTime--;

			if (mainCharacter.heart <= 0) {
				if (responTime == 0)responTime = 500;

				if (responTime > 280) {
					mainCharacter.animationNum++;
					if (mainCharacter.animationNum > 23)mainCharacter.animationNum = 0;
					responTime--;
				}
				else {
					if (responTime == 280)
						mainCharacter.animationNum = 0;
					else if (responTime % 10 == 0)
						mainCharacter.animationNum++;
					responTime -= 5;
				}


				if (responTime == 0) {
					mainCharacter.heart = 6;

					if (oldState != 2) mainCharacter.state = 0;
					else mainCharacter.state = 2;

					mainCharacter.animationNum = 0;
					mainCharacter.right = FALSE;
					mainCharacter.left = FALSE;
					mainCharacter.direction = TRUE;
					mainCharacter.energy = -1;
					invincibleTime = 100;
				}
			}
			else if (mainCharacter.state == 6) {
				if (invincibleTime % 10 == 0)
					mainCharacter.animationNum++;

				if (mainCharacter.animationNum == 5) {
					if (oldState == 5)
						mainCharacter.state = 0;
					else
						mainCharacter.state = oldState;
					mainCharacter.animationNum = oldAnimationNum;
				}
			}
			else if (mainCharacter.state == 2) {	//점프
				mainCharacter.animationNum++;
				//MoveMainChar(&mainCharacter, rect);
				JumpMainChar(&mainCharacter, jumpTime, rect);
				if (GetKeyState(VK_CONTROL) < 0 && coolTime == 0) {
					CreateBullet(bullet, mainCharacter, bulletBitmap); coolTime = 18;
				}
				if (mainCharacter.animationNum > 7)mainCharacter.animationNum = 0;
				jumpTime++;
				//점프 후 낙하시 발판이 있으면 발판에 찾기
				if (jumpTime >= 40) { mainCharacter.state = 0; jumpTime = 0; }
			}
			else if (mainCharacter.state == 5) {
				mainCharacter.energy++;
				if (mainCharacter.energy == 0)mainCharacter.animationNum = 0;
				else if (mainCharacter.energy % 5 == 0 && mainCharacter.animationNum < 4)mainCharacter.animationNum++;

				if (GetKeyState(VK_SHIFT) >= 0) {
					if (mainCharacter.animationNum < 4) {
						mainCharacter.state = 0;
						mainCharacter.energy = -1;
					}
					else if (mainCharacter.animationNum == 4) {
						CreateBullet(bullet, mainCharacter, bulletBitmap);
						mainCharacter.animationNum++;
					}
				}

				if (mainCharacter.energy % 5 == 0 && mainCharacter.animationNum < EXSHOOTANI - 1 && mainCharacter.animationNum > 4) {
					mainCharacter.animationNum++;
				}
				else if (mainCharacter.animationNum == EXSHOOTANI - 1) {
					mainCharacter.state = 0;
					mainCharacter.energy = -1;
				}
			}
			else if (mainCharacter.left && mainCharacter.right) {
				if (GetKeyState(VK_CONTROL) >= 0) {
					mainCharacter.state = 0;
				}
				else {
					mainCharacter.state = 3;
				}
				mainCharacter.Pos.right = mainCharacter.Pos.left + IDLEWIDTH;
			}
			else if (mainCharacter.right || mainCharacter.left) {
				mainCharacter.animationNum++;
				if (GetKeyState(VK_CONTROL) >= 0) {
					mainCharacter.state = 1;
				}
				else {
					if (coolTime == 0) {
						CreateBullet(bullet, mainCharacter, bulletBitmap);
						coolTime = 18;
					}
					mainCharacter.state = 4;
				}
				MoveMainChar(&mainCharacter, rect);
				if (mainCharacter.animationNum > 15)
					mainCharacter.animationNum = 0;
			}

			//상태 변환
			if (mainCharacter.state == 6 || mainCharacter.heart <= 0) {}
			else if (GetKeyState(VK_RIGHT) < 0)mainCharacter.direction = TRUE;
			else if (GetKeyState(VK_LEFT) < 0)mainCharacter.direction = FALSE;
			else if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.state != 2) mainCharacter.state = 3;
			else if (mainCharacter.state != 2 && mainCharacter.state != 5) mainCharacter.state = 0;

			MoveBullet(bullet, rect);
			DeathBullet(bullet);
			DeleteBullet(bullet);

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 2:		//꼬리 공격 준비 할 텀 타임
			SetTimer(hWnd, 3, 10, NULL);
			Boss.AttackTailReady = TRUE;
			KillTimer(hWnd, 2);
			break;
		case 3:		//꼬리 공격 준비 타임
			BossAttackTail(hWnd, rect, &mainCharacter, &Boss, &oldState, &oldAnimationNum, &invincibleTime); //보스 꼬리 공격 구조
			break;
		case 5:

			//IDLE 및 IDLE SHOOT 상태
			if (mainCharacter.state == 0) {
				mainCharacter.animationNum++;
				if (mainCharacter.animationNum > 4) mainCharacter.animationNum = 0;
			}
			else if (mainCharacter.state == 3) {

				mainCharacter.animationNum++;

				if (mainCharacter.animationNum == 1) {
					CreateBullet(bullet, mainCharacter, bulletBitmap);
					coolTime = 18;
				}

				if (mainCharacter.animationNum > 2) mainCharacter.animationNum = 0;
			}
			break;
		case 10:
			if (Stage == 5)
			{
				BossStateChange(&Boss, hWnd, BossMeteorHead, rect);
				BossAttackStateChange(&Boss, rect, BossMeteorHead);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetStretchBltMode(backMemDC, HALFTONE);


		if (Stage == 5)
		{
			STAGE5(backMemDC, rect, &Boss, BossGround, BossMeteorHead);
		}

		if (Boss.HP <= 0) {
			if (victoryNum == 0)PlaySoundA("sound/announcer_knockout_0004.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			if (victoryNum >= 27)victoryNum = 0;
			victory[victoryNum++].TransparentBlt(backMemDC, 0, 0, rect.right, rect.bottom, RGB(255, 0, 255));
		}


		//캐릭터 그림
		if (mainCharacter.heart <= 0)PaintGhost(backMemDC, ObjectDC, mainCharacter, responTime);
		else if (mainCharacter.state == 0 || mainCharacter.state == 1) PaintMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 2) PaintJump(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 3 || mainCharacter.state == 4)PaintShootMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 5) PaintEXShoot(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 6)PaintHIT(backMemDC, ObjectDC, mainCharacter);

		PaintHeart(backMemDC, ObjectDC, mainCharacter);
		PaintBullet(backMemDC, ObjectDC, bullet, bulletBitmap);
		PaintDeathBullet(backMemDC, ObjectDC, bullet, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			mainCharacter.right = TRUE;
			mainCharacter.direction = TRUE;
			break;
		case VK_LEFT:
			mainCharacter.left = TRUE;
			mainCharacter.direction = FALSE;
			break;
		case VK_SPACE:
			if (mainCharacter.state != 5 && mainCharacter.state != 6)
			{
				mainCharacter.state = 2;
			}
			break;
		case VK_CONTROL:
			if (mainCharacter.state != 2 && mainCharacter.state != 6)
			{
				if (mainCharacter.state == 1)mainCharacter.state = 4;
				else if (mainCharacter.state == 0) mainCharacter.state = 3;
			}
			break;
		case VK_SHIFT:
			if (mainCharacter.state != 2 && mainCharacter.state != 6)
			{
				mainCharacter.state = 5;
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