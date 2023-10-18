#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#include "character.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

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
	static int Stage = 1;
	static BOOL potalOn = FALSE;

	//���� ĳ���� �� �Ѿ�
	static MainCharacter mainCharacter;
	static BULLETBITMAP bulletBitmap;
	static BLinkedList* bullet;
	static int oldState, oldAnimationNum;
	static int jumpTime = 0, coolTime = 0, invincibleTime = 0, responTime = 0;

	//STAGE 1
	static NPC npc;
	static int  dlgTime = 0;

	static VILLAGE village;
	static CLOUD cloud;

	static CImage potal[POTALANI];
	static int potalAni = 0;

	//STAGE 2

	static CImage STAGE2BackGround[2];
	static MonsterStage2 Mob[4];

	//STAGE 3

	static CImage STAGE3BackGround;
	static MonsterStage3 MobStage3[1];
	static FireAttack* FireAttackHead = new FireAttack;

	//STAGE 4
	
	static CImage STAGE4BackGround[1];
	static STAGE4SPHERE* MeteorHead = new STAGE4SPHERE;
	static STAGE4EVENT Stage4Event;
	
	//STAGE 5
	static CImage BossGround[3];
	static BossMonster Boss;
	static STAGE4SPHERE* BossMeteorHead = new STAGE4SPHERE;

	static CImage victory[27];
	static int victoryNum = 0;

	static ImageOfMonster IMob;

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &rect);

		CreateMainChar(g_hInst, &mainCharacter);
		CreateNPC(g_hInst, &npc);

		hDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(hDC);
		ObjectDC = CreateCompatibleDC(backMemDC);
		backHBITMAP = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(backMemDC, backHBITMAP);

		LoadStage1(g_hInst, &village, &cloud);
		LoadPotal(potal);

		//�������� ���� �����Լ�  //�ּ� Ǯ�� �� 
		CREATESTAGE2(STAGE2BackGround, &IMob, Mob, rect);
		CREATESTAGE3(&STAGE3BackGround, &IMob, MobStage3, rect);
		CREATESTAGE5(&Boss, BossGround, rect); 
		CREATESTAGE4(rect, &IMob, STAGE4BackGround, &Stage4Event);

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

		PlaySoundA("sound/stage1.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);

		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			//����Ű Ȯ��
			if (GetKeyState(VK_RIGHT) >= 0 && mainCharacter.right) {
				mainCharacter.right = FALSE;
			}
			if (GetKeyState(VK_LEFT) >= 0 && mainCharacter.left) {
				mainCharacter.left = FALSE;
			}

			//��Ʈ�ڽ� ����
			HitBoxMainChar(&mainCharacter);

			//��Ÿ�� 
			if (coolTime > 0)coolTime--;
			else coolTime = 0;


			if (Stage == 2) { //���� �浹
				CheckStage2(bullet, Mob);
				RECT temp;
				int monsterNum = 4;
				for (int i = 0; i < 4; ++i) {
					if (!Mob[i].ALIVE) { monsterNum--; continue; }
					if (IntersectRect(&temp, &mainCharacter.Pos, &Mob[i].rect) && invincibleTime == 0 && mainCharacter.heart != 0) {
						mainCharacter.heart--;
						oldState = mainCharacter.state;
						oldAnimationNum = mainCharacter.animationNum;
						mainCharacter.state = 6;
						mainCharacter.energy = -1;
						mainCharacter.animationNum = 0;
						invincibleTime = 100;
					}
				}
				if (monsterNum == 0)potalOn = TRUE;
			}
			else if (Stage == 3) {
				PlayerStage3Mobcrash(MobStage3, bullet);//�� �浹
				RECT temp;
				if (MobStage3[0].ALIVE && IntersectRect(&temp, &mainCharacter.Pos, &MobStage3[0].rect) 
					&& invincibleTime == 0 && mainCharacter.heart !=0){
					mainCharacter.heart--;
					oldState = mainCharacter.state;
					oldAnimationNum = mainCharacter.animationNum;
					mainCharacter.state = 6;
					mainCharacter.energy = -1;
					mainCharacter.animationNum = 0;
					invincibleTime = 100;
				}
				else if (MobStage3[0].ALIVE == FALSE)
					potalOn = TRUE;
				Stage3_TimeCrashEvent(MobStage3, FireAttackHead, &mainCharacter, &oldState, &oldAnimationNum, &invincibleTime);
			}
			else if (Stage == 4) {
				Stage4_OBJECTMOVE(MeteorHead, &mainCharacter, rect, &Stage4Event, &oldState, &oldAnimationNum, &invincibleTime);
				CheckStage4(bullet, MeteorHead, &Stage4Event, &potalOn);
			}
			else if (Stage == 5) {
				BossAttackMeteor(rect, &Boss, MeteorHead, bullet, &mainCharacter, &oldState, &oldAnimationNum, &invincibleTime);
			}


			// �ǰ� Ȯ�� �� ���� 6���� ��ȯ

			if (invincibleTime != 0)invincibleTime--;

			if (mainCharacter.heart <= 0) {
				if (responTime == 0)responTime = 500;
				
				if (responTime > 280) {
					mainCharacter.animationNum++;
					if (mainCharacter.animationNum > 23)mainCharacter.animationNum = 0;
					responTime--;
				}
				else{
					if (responTime == 280)
						mainCharacter.animationNum = 0;
					else if(responTime % 10 == 0)
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
			else if (mainCharacter.state == 2) {	//����
				mainCharacter.animationNum++;
				//MoveMainChar(&mainCharacter, rect);
				JumpMainChar(&mainCharacter, jumpTime, rect);
				if (GetKeyState(VK_CONTROL) < 0 && coolTime == 0) {
					CreateBullet(bullet, mainCharacter, bulletBitmap); coolTime = 18;
				}
				if (mainCharacter.animationNum > 7)mainCharacter.animationNum = 0;
				jumpTime++;
				//���� �� ���Ͻ� ������ ������ ���ǿ� ã��
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

			//���� ��ȯ
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
		case 2:		//���� ���� �غ� �� �� Ÿ��
			SetTimer(hWnd, 3, 10, NULL);
			Boss.AttackTailReady = TRUE;
			KillTimer(hWnd, 2);
			break;
		case 3:		//���� ���� �غ� Ÿ��
			BossAttackTail(hWnd,rect, &mainCharacter, &Boss, &oldState, &oldAnimationNum, &invincibleTime); //���� ���� ���� ����
			break;
		case 5:

			if (potalOn) {
				potalAni++;
				if (potalAni > 3)potalAni = 0;
			}
			if (Stage == 1) {
				//npc
				npc.animationNum++;
				if (npc.animationNum == 11)npc.animationNum = 5;
				else if (npc.animationNum == 5) {
					dlgTime++;
					npc.animationNum = 0;
					if (dlgTime > 15) {
						npc.animationNum = 5;
						dlgTime = 0;
					}
				}
			}
			//IDLE �� IDLE SHOOT ����
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
			if (Stage == 2) {
				Stage2_MonsterStateChange(Mob, rect, mainCharacter.Pos);
			}
			else if (Stage == 3) {
				Stage3_MonsterStateChange(MobStage3, FireAttackHead, mainCharacter.Pos, rect);
			}
			else if (Stage == 4) {
				Stage4_StateChange(MeteorHead);
			}
			else if (Stage == 5) {
				BossStateChange(&Boss, hWnd, MeteorHead, rect);
				BossAttackStateChange(&Boss, rect, MeteorHead);
			}
			break;
		case 11: // 1�ʿ� �� �� ����
			MeteorStackInsert(MeteorHead, rect);
			Stage4Event.Stage4Sec++;
			if (Stage4Event.Stage4Sec >= 130) { // Ŭ���� �ð� ����
				Stage4Event.STAGE4CLEAR = TRUE;
				KillTimer(hWnd, 11);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetStretchBltMode(backMemDC, HALFTONE);

		if (Stage == 1) {
			PaintStage1(g_hInst, backMemDC, ObjectDC, village, cloud, rect);
			PaintNPC(backMemDC, ObjectDC, &npc);
		}
		else if (Stage == 2) {
			STAGE2(backMemDC, rect, STAGE2BackGround, Mob, &IMob);
		}
		else if (Stage == 3) {
			Stage3(backMemDC, rect, &STAGE3BackGround, MobStage3, &IMob, FireAttackHead);
		}
		else if (Stage == 4) {
			STAGE4(backMemDC, rect, STAGE4BackGround, MeteorHead, &IMob, &Stage4Event);
		}
		else if (Stage == 5) {
			STAGE5(backMemDC, rect, &Boss, BossGround, MeteorHead);

		}

		if (Boss.HP <= 0 ) {
			if(victoryNum == 0)PlaySoundA("sound/announcer_knockout_0004.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			if (victoryNum >= 27)victoryNum = 0;
			victory[victoryNum++].TransparentBlt(backMemDC, 0, 0, rect.right, rect.bottom, RGB(255, 0, 255));
		}

		if(potalOn)
			PaintPotal(backMemDC, &potal[potalAni]);


		//�ɸ��� �׸�
		if (mainCharacter.heart <= 0)PaintGhost(backMemDC, ObjectDC, mainCharacter, responTime);
		else if (mainCharacter.state == 0 || mainCharacter.state == 1) PaintMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 2) PaintJump(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 3 || mainCharacter.state == 4)PaintShootMainChar(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 5) PaintEXShoot(backMemDC, ObjectDC, mainCharacter);
		else if (mainCharacter.state == 6)PaintHIT(backMemDC, ObjectDC, mainCharacter);
		
		
		if (Stage == 2) { //2�������� �� ��� ȿ��
			SelectObject(ObjectDC, STAGE2BackGround[1]);
			TransparentBlt(backMemDC, 0, rect.bottom - 150, rect.right, 150, ObjectDC, 0, 0, STAGE2BackGround[1].GetWidth(),
				STAGE2BackGround[1].GetHeight(), RGB(255, 0, 255));
		}
		PaintHeart(backMemDC, ObjectDC, mainCharacter);
		PaintBullet(backMemDC, ObjectDC, bullet, bulletBitmap);
		PaintDeathBullet(backMemDC, ObjectDC, bullet, bulletBitmap);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, backMemDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_RIGHT:
			mainCharacter.right = TRUE;
			mainCharacter.direction = TRUE;
			break;
		case VK_LEFT:
			mainCharacter.left = TRUE;
			mainCharacter.direction = FALSE;
			break;
		case VK_DOWN:
			break;
		case VK_UP:
			if (CheckPotal(mainCharacter) && potalOn) {
				if (Stage == 1) {
					DeleteStage1(&village, &cloud, potal);
					DeleteNPC(&npc);
					PlaySoundA("sound/stage2~3.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
				}
				else if (Stage == 2) {

				}
				else if (Stage == 3) {
					SetTimer(hWnd, 11, 250, NULL);
					PlaySoundA("sound/stage4.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
				}
				else if (Stage == 4) {
					PlaySoundA("sound/stage5.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
				}
				//��Ʈ�� ���� �� ��ġ �ʱ�ȭ
				InitMainChar(&mainCharacter);
				jumpTime = 0;
				Stage++;
				potalOn = FALSE;
			}
			break;
		case VK_SPACE:
			if (mainCharacter.state != 5 && mainCharacter.state != 6) {
				mainCharacter.state = 2;
			}
			break;
		case VK_CONTROL:
			if (mainCharacter.state != 2 && mainCharacter.state != 6) {
				if (mainCharacter.state == 1)mainCharacter.state = 4;
				else if(mainCharacter.state == 0) mainCharacter.state = 3;
			}
			break;
		case VK_SHIFT:
			if (mainCharacter.state != 2 && mainCharacter.state != 6) {
				mainCharacter.state = 5;
			}
			break;
		case VK_RETURN:
			if (Stage == 1) {
				if (potalOn)
					CheckNPCDlg(mainCharacter, &npc, &dlgTime);
				else
					potalOn = CheckNPCDlg(mainCharacter, &npc, &dlgTime);
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

		DeleteStage1(&village, &cloud, potal);
		DeleteNPC(&npc);
		DeleteObject(backHBITMAP);

		DeleteDC(ObjectDC);
		DeleteDC(backMemDC);
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

