#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#include "ClientCharacter.h"
#include "TCPClient.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

extern HWND hWnd;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

extern HANDLE hInitEvent;

//메인 캐릭터 및 총알
extern MainCharacter mainCharacter;
extern BulletBitmap bulletBitmap;
MainState oldState;
int oldAnimationNum;

//보스
extern BossMonster Boss;
extern BossCImage bossImage;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //윈도우 프로시저 프로토선언 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// HWND hWnd;				//윈도우 핸들값 선언
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

	// 소켓 통신 스레드 생성
	hNetworkThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if (hNetworkThread != NULL)
	{
		CloseHandle(hNetworkThread);
	}
	WSACleanup();

	// CloseHandle(hNetworkThread);
	// CloseHandle(hInitEvent);

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

	//static int Stage = 5;
	//static BOOL potalOn = FALSE;

	////메인 캐릭터 및 총알
	//static MainCharacter mainCharacter;
	//static BULLETBITMAP bulletBitmap;
	//static MainState oldState;
	//static int oldAnimationNum;
	//static int jumpTime = 0, coolTime = 0, invincibleTime = 0, responTime = 0;

	////STAGE 5
	//static CImage BossGround[3];
	//static BossMonster Boss;
	//static STAGE4SPHERE* BossMeteorHead = new STAGE4SPHERE;

	//static CImage victory[27];
	//static int victoryNum = 0;

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

		TCHAR temp[30];
		for (int i = 0; i < 27; ++i) {
			wsprintf(temp, L"victory/victory%d.png", i + 1);
			victory[i].Load(temp);
		}

		CreateMainChar(g_hInst, &mainCharacter);
		LoadBullet(&bulletBitmap, g_hInst);
		CreateBossAndStage(&Boss, &bossImage, BossGround, rect);

		// 이미지 불러왔음을 알리는 이벤트
		/*hInitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		SetEvent(hInitEvent);*/

		ReleaseDC(hWnd, hDC);

		PlaySoundA("sound/stage5.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);

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

		//캐릭터 그림
		if (mainCharacter.info.heart <= 0)PaintGhost(backMemDC, ObjectDC, mainCharacter);
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

	case WM_DESTROY:

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