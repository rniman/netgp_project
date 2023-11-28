#include "ClientCharacter.h"
#include "TCPClient.h"

extern HWND hWnd;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

extern HANDLE hInitEvent;

//메인 캐릭터 및 총알
extern DWORD playerNum;
extern MainCharacter mainPlayer1;
extern MainCharacter mainPlayer2;
extern BulletBitmap bulletBitmap;

//보스
extern BossMonster Boss;
extern BossCImage bossImage;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //윈도우 프로시저 프로토선언 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//HWND hWnd;				//윈도우 핸들값 선언
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

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	// 이벤트 생성
	// tbd

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hNetworkThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	while (GetMessage(&Message, 0, 0, 0)) 
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if(hNetworkThread !=NULL)
	{
		CloseHandle(hNetworkThread);
	}
	CloseHandle(hInitEvent);
	
	WSACleanup();		
	
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

	static CImage BossGround[3];
	static CImage victory[27];
	static int victoryNum = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rect);

		hDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(hDC);
		ObjectDC = CreateCompatibleDC(backMemDC);
		backHBITMAP = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(backMemDC, backHBITMAP);

		TCHAR temp[30];
		for (int i = 0; i < 27; ++i)
		{
			wsprintf(temp, L"victory/victory%d.png", i + 1);
			victory[i].Load(temp);
		}

		CreateMainChar(g_hInst, &mainPlayer1);
		CreateMainChar(g_hInst, &mainPlayer2);
		LoadBullet(&bulletBitmap, g_hInst);
		CreateBossAndStage(&Boss, &bossImage, BossGround, rect);
		
		// 이미지 불러왔음을 알리는 이벤트
		hInitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hInitEvent)SetEvent(hInitEvent);

		ReleaseDC(hWnd, hDC);
		//SetTimer(hWnd, 1, 15, NULL);
		//SetTimer(hWnd, 5, 90, NULL);
		//SetTimer(hWnd, 10, 100, NULL);

		PlaySoundA("sound/stage5.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
				
		break;
	//case WM_TIMER:
	//	switch (wParam)
	//	{
	//	case 1:
	//		//방향키 확인
	//		if (GetKeyState(VK_RIGHT) >= 0 && mainCharacter.info.right)
	//		{
	//			mainCharacter.info.right = FALSE;
	//		}
	//		if (GetKeyState(VK_LEFT) >= 0 && mainCharacter.info.left)
	//		{
	//			mainCharacter.info.left = FALSE;
	//		}

	//		//히트박스 수정
	//		HitBoxMainChar(&mainCharacter);

	//		//쿨타임 
	//		if (coolTime > 0)coolTime--;
	//		else coolTime = 0;

	//		BossAttackMeteor(rect, &Boss, &mainCharacter, &oldState, &oldAnimationNum, &invincibleTime); //oldState 수정필요

	//		// 피격 확인 후 상태 6으로 전환

	//		if (invincibleTime != 0)invincibleTime--;

	//		if (mainCharacter.info.heart <= 0) 
	//		{
	//			if (responTime == 0)responTime = 500;

	//			if (responTime > 280) {
	//				mainCharacter.info.animationNum++;
	//				if (mainCharacter.info.animationNum > 23)mainCharacter.info.animationNum = 0;
	//				responTime--;
	//			}
	//			else {
	//				if (responTime == 280)
	//					mainCharacter.info.animationNum = 0;
	//				else if (responTime % 10 == 0)
	//					mainCharacter.info.animationNum++;
	//				responTime -= 5;
	//			}


	//			if (responTime == 0) {
	//				mainCharacter.info.heart = 6;

	//				if (oldState != MainState::JUMP) mainCharacter.info.state = MainState::IDLE;
	//				else mainCharacter.info.state = MainState::JUMP;

	//				mainCharacter.info.animationNum = 0;
	//				mainCharacter.info.right = FALSE;
	//				mainCharacter.info.left = FALSE;
	//				mainCharacter.info.direction = TRUE;
	//				mainCharacter.info.energy = -1;
	//				invincibleTime = 100;
	//			}
	//		}
	//		else if (mainCharacter.info.state == MainState::HIT) 
	//		{
	//			if (invincibleTime % 10 == 0)
	//				mainCharacter.info.animationNum++;

	//			if (mainCharacter.info.animationNum == 5)
	//			{
	//				if (oldState == MainState::EXSHOOT)
	//					mainCharacter.info.state = MainState::IDLE;
	//				else
	//					mainCharacter.info.state = oldState;
	//				mainCharacter.info.animationNum = oldAnimationNum;
	//			}
	//		}
	//		else if (mainCharacter.info.state == MainState::JUMP)
	//		{	//점프
	//			mainCharacter.info.animationNum++;
	//			//MoveMainChar(&mainCharacter, rect);
	//			JumpMainChar(&mainCharacter, jumpTime, rect);
	//			if (GetKeyState(VK_CONTROL) < 0 && coolTime == 0) 
	//			{
	//				CreateBullet(mainCharacter, bulletBitmap); 
	//				coolTime = 18;
	//			}

	//			if (mainCharacter.info.animationNum > 7)
	//			{
	//				mainCharacter.info.animationNum = 0;
	//			}
	//			jumpTime++;
	//			
	//			//점프 후 낙하시 발판이 있으면 발판에 찾기
	//			if (jumpTime >= 40) 
	//			{ 
	//				mainCharacter.info.state = MainState::IDLE; 
	//				jumpTime = 0;
	//			}
	//		}
	//		else if (mainCharacter.info.state == MainState::EXSHOOT) 
	//		{
	//			mainCharacter.info.energy++;
	//			if (mainCharacter.info.energy == 0)mainCharacter.info.animationNum = 0;
	//			else if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < 4)mainCharacter.info.animationNum++;

	//			if (GetKeyState(VK_SHIFT) >= 0)
	//			{
	//				if (mainCharacter.info.animationNum < 4) 
	//				{
	//					mainCharacter.info.state = MainState::IDLE;
	//					mainCharacter.info.energy = -1;
	//				}
	//				else if (mainCharacter.info.animationNum == 4) 
	//				{
	//					CreateBullet(mainCharacter, bulletBitmap);
	//					mainCharacter.info.animationNum++;
	//				}
	//			}

	//			if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < EXSHOOTANI - 1 && mainCharacter.info.animationNum > 4) 
	//			{
	//				mainCharacter.info.animationNum++;
	//			}
	//			else if (mainCharacter.info.animationNum == EXSHOOTANI - 1) 
	//			{
	//				mainCharacter.info.state = MainState::IDLE;
	//				mainCharacter.info.energy = -1;
	//			}
	//		}
	//		else if (mainCharacter.info.left && mainCharacter.info.right)
	//		{
	//			if (GetKeyState(VK_CONTROL) >= 0) 
	//			{
	//				mainCharacter.info.state = MainState::IDLE;
	//			}
	//			else 
	//			{
	//				mainCharacter.info.state = MainState::SHOOT;
	//			}

	//			mainCharacter.info.Pos.right = mainCharacter.info.Pos.left + IDLEWIDTH;
	//		}
	//		else if (mainCharacter.info.right || mainCharacter.info.left) 
	//		{
	//			mainCharacter.info.animationNum++;
	//			if (GetKeyState(VK_CONTROL) >= 0) 
	//			{
	//				mainCharacter.info.state = MainState::RUN;
	//			}
	//			else 
	//			{
	//				if (coolTime == 0) 
	//				{
	//					CreateBullet(mainCharacter, bulletBitmap);
	//					coolTime = 18;
	//				}
	//				mainCharacter.info.state = MainState::RUNSHOOT;
	//			}

	//			MoveMainChar(&mainCharacter, rect);
	//			if (mainCharacter.info.animationNum > 15)
	//			{
	//				mainCharacter.info.animationNum = 0;
	//			}
	//		}

	//		//상태 변환
	//		if (mainCharacter.info.state == MainState::HIT || mainCharacter.info.heart <= 0) {}
	//		else if (GetKeyState(VK_RIGHT) < 0)mainCharacter.info.direction = TRUE;
	//		else if (GetKeyState(VK_LEFT) < 0)mainCharacter.info.direction = FALSE;
	//		else if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.info.state != MainState::JUMP) mainCharacter.info.state = MainState::SHOOT;
	//		else if (mainCharacter.info.state != MainState::JUMP && mainCharacter.info.state != MainState::EXSHOOT) mainCharacter.info.state = MainState::IDLE;

	//		MoveBullet(mainCharacter, rect);
	//		DeathBullet(mainCharacter);

	//		InvalidateRect(hWnd, NULL, FALSE);
	//		break;
	//	case 2:		//꼬리 공격 준비 할 텀 타임
	//		//그냥 1초 쉬는 용도인듯?
	//		SetTimer(hWnd, 3, 10, NULL);
	//		//Boss.AttackTailReady = TRUE;
	//		KillTimer(hWnd, 2);
	//		break;
	//	case 3:		//꼬리 공격 준비 타임
	//		Boss.AttackTailReady = TRUE;
	//		BossAttackTail(hWnd, rect, &mainCharacter, &Boss, &bossImage, &oldState, &oldAnimationNum, &invincibleTime); //보스 꼬리 공격 구조 + oldState바꾸기
	//		break;
	//	case 5:
	//		//IDLE 및 IDLE SHOOT 상태
	//		if (mainCharacter.info.state == MainState::IDLE) 
	//		{
	//			mainCharacter.info.animationNum++;
	//			if (mainCharacter.info.animationNum > 4)
	//			{
	//				mainCharacter.info.animationNum = 0;
	//			}
	//		}
	//		else if (mainCharacter.info.state == MainState::SHOOT)
	//		{
	//			mainCharacter.info.animationNum++;

	//			if (mainCharacter.info.animationNum == 1)
	//			{
	//				CreateBullet(mainCharacter, bulletBitmap);
	//				coolTime = 18;
	//			}

	//			if (mainCharacter.info.animationNum > 2) 
	//			{
	//				mainCharacter.info.animationNum = 0;
	//			}
	//		}
	//		break;
	//	case 10:
	//		BossStateChange(&Boss, hWnd, rect);
	//		BossAttackStateChange(&Boss, rect);
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetStretchBltMode(backMemDC, HALFTONE);

		PaintBoss(backMemDC, rect, &Boss, &bossImage, BossGround);

		if (Boss.HP <= 0)
		{
			if (victoryNum == 0) PlaySoundA("sound/announcer_knockout_0004.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			if (victoryNum >= 27) victoryNum = 0;
			victory[victoryNum++].TransparentBlt(backMemDC, 0, 0, rect.right, rect.bottom, RGB(255, 0, 255));
		}

		//캐릭터 그림 
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer1, bulletBitmap);
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer2, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		break;
	case WM_DESTROY:
		//KillTimer(hWnd, 1);
		//KillTimer(hWnd, 2);
		//KillTimer(hWnd, 3);
		//KillTimer(hWnd, 5);
		//KillTimer(hWnd, 10);

		DeleteBitBullet(&bulletBitmap);
		DeleteMainChar(&mainPlayer1);
		DeleteMainChar(&mainPlayer2);

		DeleteObject(backHBITMAP);

		DeleteDC(ObjectDC);
		DeleteDC(backMemDC);
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}