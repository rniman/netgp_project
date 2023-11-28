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

//���� ĳ���� �� �Ѿ�
extern MainCharacter mainCharacter;
extern BulletBitmap bulletBitmap;
MainState oldState;
int oldAnimationNum;

//����
extern BossMonster Boss;
extern BossCImage bossImage;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //������ ���ν��� �����伱�� 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// HWND hWnd;				//������ �ڵ鰪 ����
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
	{
		return 1;
	}
	// �̺�Ʈ ����
	// tbd

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���� ��� ������ ����
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

	//static int Stage = 5;
	//static BOOL potalOn = FALSE;

	////���� ĳ���� �� �Ѿ�
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

		// �̹��� �ҷ������� �˸��� �̺�Ʈ
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

		//ĳ���� �׸�
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