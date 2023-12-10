#include "ClientCharacter.h"
#include "TCPClient.h"
#include "resource.h"

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

// IP 주소
extern char g_IpAddress[16];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //윈도우 프로시저 프로토선언 

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

	// IP 입력 창 생성 - 11.30
	if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, IpDialogProc) == IDCANCEL)
	{
		// 취소 버튼 클릭
		printf("canceled\n");
		return 0;
	}

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

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
		// 클라이언트 스레드에 종료 메시지 전송
        PostThreadMessage(GetThreadId(hNetworkThread), WM_QUIT, 0, 0);
        WaitForSingleObject(hNetworkThread, INFINITE);
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

		PlaySoundA("sound/stage5.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
				
		break;
	
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetStretchBltMode(backMemDC, HALFTONE);

		PaintBoss(backMemDC, rect, &Boss, &bossImage, BossGround);

		if (Boss.HP <= 0)
		{
			if (victoryNum >= 27) victoryNum = 0;
			victory[victoryNum++].TransparentBlt(backMemDC, 0, 0, rect.right, rect.bottom, RGB(255, 0, 255));
		}

		//캐릭터 그림 
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer1, bulletBitmap);
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer2, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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