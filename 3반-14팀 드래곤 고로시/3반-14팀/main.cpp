#include "ClientCharacter.h"
#include "TCPClient.h"
#include "resource.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

extern HWND hWnd;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
HANDLE hNetworkThread;

extern HANDLE hInitEvent;

//���� ĳ���� �� �Ѿ�
extern DWORD playerNum;
extern MainCharacter mainPlayer1;
extern MainCharacter mainPlayer2;
extern BulletBitmap bulletBitmap;

//����
extern BossMonster Boss;
extern BossCImage bossImage;

// IP �ּ�
extern char g_IpAddress[16];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //������ ���ν��� �����伱�� 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//HWND hWnd;				//������ �ڵ鰪 ����
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

	// IP �Է� â ���� - 11.30
	if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, IpDialogProc) == IDCANCEL)
	{
		// ��� ��ư Ŭ��
		printf("canceled\n");
		return 0;
	}

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}


	// �̺�Ʈ ����
	// tbd

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hNetworkThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	/*while (GetMessage(&Message, 0, 0, 0)) 
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}*/

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if(hNetworkThread !=NULL)
	{
		// Ŭ���̾�Ʈ �����忡 ���� �޽��� ����
        PostThreadMessage(GetThreadId(hNetworkThread), WM_QUIT, 0, 0);
        WaitForSingleObject(hNetworkThread, INFINITE);
		CloseHandle(hNetworkThread);
	}
	/*CloseHandle(hInitEvent);
	
	WSACleanup();	*/	
	
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
		
		// �̹��� �ҷ������� �˸��� �̺�Ʈ
		hInitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hInitEvent)SetEvent(hInitEvent);

		ReleaseDC(hWnd, hDC);
		//SetTimer(hWnd, 1, 15, NULL);
		//SetTimer(hWnd, 5, 90, NULL);
		//SetTimer(hWnd, 10, 100, NULL);

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

		//ĳ���� �׸� 
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer1, bulletBitmap);
		PaintMainCharacter(backMemDC, ObjectDC, playerNum, mainPlayer2, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
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