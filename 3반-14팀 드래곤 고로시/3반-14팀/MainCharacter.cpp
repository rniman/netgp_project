
#include "character.h"

void CreateMainChar(HINSTANCE g_hInst, MainCharacter* mainCharacter)
{

	mainCharacter->state = 0;
	mainCharacter->animationNum = 0;
	mainCharacter->right = FALSE;
	mainCharacter->left = FALSE;
	mainCharacter->direction = TRUE;

	mainCharacter->heart = 6;

	mainCharacter->HEARTBITMAP[0] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mainCharacter->HEARTBITMAP[1] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mainCharacter->HEARTBITMAP[2] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	TCHAR temp[50];
	for (int i = 0; i < IDLEANI; ++i) {
		wsprintf(temp, L"CUPHEAD/idle/cuphead_idle_00%02d.bmp", i + 1);
		mainCharacter->IDLEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->IDLEBITMAP[i], sizeof(BITMAP), &mainCharacter->IDLEBitData[i]);
	}
	for (int i = 0; i < RUNANI; ++i) {
		wsprintf(temp, L"CUPHEAD/run/cuphead_run_00%02d.bmp", i + 1);
		mainCharacter->RUNBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->RUNBITMAP[i], sizeof(BITMAP), &mainCharacter->RUNBitData[i]);
	}
	for (int i = 0; i < JUMPANI; ++i) {
		wsprintf(temp, L"CUPHEAD/jump/cuphead_jump_00%02d.bmp", i + 1);
		mainCharacter->JUMPBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->JUMPBITMAP[i], sizeof(BITMAP), &mainCharacter->JUMPBitData[i]);
	}
	for (int i = 0; i < SHOOTANI; ++i) {
		wsprintf(temp, L"CUPHEAD/shoot/cuphead_shoot_straight_00%02d.bmp", i + 1);
		mainCharacter->SHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->SHOOTBITMAP[i], sizeof(BITMAP), &mainCharacter->SHOOTBitData[i]);
	}
	for (int i = 0; i < RUNSHOOTANI; ++i) {
		wsprintf(temp, L"CUPHEAD/runshoot/cuphead_run_shoot_00%02d.bmp", i + 1);
		mainCharacter->RUNSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->RUNSHOOTBITMAP[i], sizeof(BITMAP), &mainCharacter->RUNSHOOTBitData[i]);
	}
	for (int i = 0; i < EXSHOOTANI; ++i) {
		wsprintf(temp, L"CUPHEAD/exshoot/cuphead_ex_straight_00%02d.bmp", i + 1);
		mainCharacter->EXSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->EXSHOOTBITMAP[i], sizeof(BITMAP), &mainCharacter->EXSHOOTBitData[i]);
	}
	for (int i = 0; i < HITANI; ++i) {
		wsprintf(temp, L"CUPHEAD/hit/cuphead_hit_00%02d.bmp", i + 1);
		mainCharacter->HITBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->HITBITMAP[i], sizeof(BITMAP), &mainCharacter->HITBitData[i]);
	}
	for (int i = 0; i < GHOSTANI; ++i) {
		wsprintf(temp, L"CUPHEAD/ghost/cuphead_ghost_00%02d.bmp", i + 1);
		mainCharacter->GHOSTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->GHOSTBITMAP[i], sizeof(BITMAP), &mainCharacter->GHOSTBitData[i]);
	}
	for (int i = 0; i < REVIVEANI; ++i) {
		wsprintf(temp, L"CUPHEAD/revive/cuphead_revive_00%02d.bmp", i + 1);
		mainCharacter->REVIVEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->REVIVEBITMAP[i], sizeof(BITMAP), &mainCharacter->REVIVEBitData[i]);
	}

	mainCharacter->Pos.left = 0;
	mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->IDLEBitData[mainCharacter->animationNum].bmWidth / 2;
	mainCharacter->Pos.bottom = STAGEFLOOR;
	mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->IDLEBitData[mainCharacter->animationNum].bmHeight / 2;

	mainCharacter->energy = -1;
}

void InitMainChar(MainCharacter* mainCharacter)
{
	mainCharacter->state = 0;
	mainCharacter->animationNum = 0;
	mainCharacter->right = FALSE;
	mainCharacter->left = FALSE;
	mainCharacter->direction = TRUE;

	mainCharacter->Pos.left = 0;
	mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->IDLEBitData[mainCharacter->animationNum].bmWidth / 2;
	mainCharacter->Pos.bottom = STAGEFLOOR;
	mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->IDLEBitData[mainCharacter->animationNum].bmHeight / 2;

	mainCharacter->energy = -1;
}

void DeleteMainChar(MainCharacter* mainCharacter)
{
	for(int i=0;i<IDLEANI;++i)
		DeleteObject(mainCharacter->IDLEBITMAP[i]);
	for (int i = 0; i <RUNANI; ++i) 
		DeleteObject(mainCharacter->RUNBITMAP[i]);
	for(int i=0;i<JUMPANI;++i)
		DeleteObject(mainCharacter->JUMPBITMAP[i]);
	for (int i = 0; i < SHOOTANI; ++i)
		DeleteObject(mainCharacter->SHOOTBITMAP[i]);
	for (int i = 0; i < RUNSHOOTANI; ++i)
		DeleteObject(mainCharacter->RUNSHOOTBITMAP[i]);
	for (int i = 0; i < EXSHOOTANI; ++i)
		DeleteObject(mainCharacter->EXSHOOTBITMAP[i]);
	for (int i = 0; i < HITANI; ++i)
		DeleteObject(mainCharacter->HITBITMAP[i]);
	for (int i = 0; i < GHOSTANI; ++i)
		DeleteObject(mainCharacter->GHOSTBITMAP[i]);
	for (int i = 0; i < REVIVEANI; ++i)
		DeleteObject(mainCharacter->REVIVEBITMAP[i]);
	for (int i = 0; i < 3; ++i)
		DeleteObject(mainCharacter->HEARTBITMAP[i]);
	
}

void PaintHeart(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)
{
	for (int i = 0; i < mainCharacter.heart / 2; ++i) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[0]);
		TransparentBlt(backMemDC, 50 * i, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	
	if (mainCharacter.heart == 5) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 50 * 2, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.heart <= 4) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 50 * 2, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.heart == 3) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 50 * 1, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.heart <= 2) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 50 * 1, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.heart == 1) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 0, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.heart <= 0) {
		SelectObject(ObjectDC, mainCharacter.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 0, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}

}

void PaintGhost(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter, int responeTime)
{
	if (responeTime > 280) {
		SelectObject(ObjectDC, mainCharacter.GHOSTBITMAP[mainCharacter.animationNum]);
		TransparentBlt(backMemDC, mainCharacter.Pos.left,
			mainCharacter.Pos.bottom - mainCharacter.GHOSTBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.GHOSTBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.GHOSTBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.GHOSTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.GHOSTBitData[mainCharacter.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
	else {
		SelectObject(ObjectDC, mainCharacter.REVIVEBITMAP[mainCharacter.animationNum]);
		TransparentBlt(backMemDC, 
			(mainCharacter.Pos.right + mainCharacter.Pos.left)/2 - mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmWidth / 4,
			(mainCharacter.Pos.bottom + mainCharacter.Pos.top)/2 - mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmHeight / 4,
			mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.REVIVEBitData[mainCharacter.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
}

void PaintMainChar(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.state == 0) {
		if (mainCharacter.direction) {
			SelectObject(ObjectDC, mainCharacter.IDLEBITMAP[mainCharacter.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.Pos.left,
				mainCharacter.Pos.bottom - mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else{
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.IDLEBITMAP[mainCharacter.animationNum]);
			temp = CreateBitmap(mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight,
				tempDC, mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth - 1, 0,
				-mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight, SRCCOPY);
			
			TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.Pos.bottom - mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.IDLEBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.IDLEBitData[mainCharacter.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
	else {
		if (mainCharacter.direction) {
			SelectObject(ObjectDC, mainCharacter.RUNBITMAP[mainCharacter.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.Pos.left,
				mainCharacter.Pos.bottom - mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
		}
		else {
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.RUNBITMAP[mainCharacter.animationNum]);
			temp = CreateBitmap(mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight,
				tempDC,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth - 1,\
				0,
				-mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight, SRCCOPY);

			TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.Pos.bottom - mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.RUNBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
}

void PaintJump(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)//아직 반전 까지 안그림
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.direction) {
		SelectObject(ObjectDC, mainCharacter.JUMPBITMAP[mainCharacter.animationNum]);
		TransparentBlt(backMemDC, mainCharacter.Pos.left,
			mainCharacter.Pos.bottom - mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight,
			RGB(255, 0, 255));
	}
	else {
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.JUMPBITMAP[mainCharacter.animationNum]);
		temp = CreateBitmap(mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight,
			tempDC,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth - 1, \
			0,
			-mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight, SRCCOPY);

		TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.Pos.bottom - mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.JUMPBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.JUMPBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
		DeleteDC(tempDC);
		DeleteObject(temp);
	}
	
}

void PaintShootMainChar(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.state == 3) {
		if (mainCharacter.direction) {
			SelectObject(ObjectDC, mainCharacter.SHOOTBITMAP[mainCharacter.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.Pos.left,
				mainCharacter.Pos.bottom - mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else {
			tempDC = CreateCompatibleDC(ObjectDC);

			SelectObject(tempDC, mainCharacter.SHOOTBITMAP[mainCharacter.animationNum]);

			temp = CreateBitmap(mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);

			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight,
				tempDC, mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth - 1, 0,
				-mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight, SRCCOPY);

			TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.Pos.bottom - mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.SHOOTBitData[mainCharacter.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
	else {
		if (mainCharacter.direction) {
			SelectObject(ObjectDC, mainCharacter.RUNSHOOTBITMAP[mainCharacter.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.Pos.left,
				mainCharacter.Pos.bottom - mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
		}
		else {
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.RUNSHOOTBITMAP[mainCharacter.animationNum]);
			temp = CreateBitmap(mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight,
				tempDC,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth - 1,
				0,
				-mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight,
				SRCCOPY);

			TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.Pos.bottom - mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmWidth,
				mainCharacter.RUNSHOOTBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
}

void PaintEXShoot(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.direction) {
		SelectObject(ObjectDC, mainCharacter.EXSHOOTBITMAP[mainCharacter.animationNum]);
		TransparentBlt(backMemDC, mainCharacter.Pos.left,
			mainCharacter.Pos.bottom - mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
	}
	else {
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.EXSHOOTBITMAP[mainCharacter.animationNum]);
		temp = CreateBitmap(mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight,
			tempDC,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth - 1,
			0,
			-mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight,
			SRCCOPY);

		TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.Pos.bottom - mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.EXSHOOTBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
		DeleteDC(tempDC);
		DeleteObject(temp);
	}
}

void PaintHIT(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.direction) {
		SelectObject(ObjectDC, mainCharacter.HITBITMAP[mainCharacter.animationNum]);
		TransparentBlt(backMemDC, mainCharacter.Pos.left,
			mainCharacter.Pos.bottom - mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));
	}
	else {
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.HITBITMAP[mainCharacter.animationNum]);
		temp = CreateBitmap(mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight,
			tempDC,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth - 1,
			0,
			-mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight,
			SRCCOPY);

		TransparentBlt(backMemDC, mainCharacter.Pos.right - mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.Pos.bottom - mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight / 2,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth / 2,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.HITBitData[mainCharacter.animationNum].bmWidth,
			mainCharacter.HITBitData[mainCharacter.animationNum].bmHeight, RGB(255, 0, 255));

		DeleteDC(tempDC);
		DeleteObject(temp);
	}
}

void MoveMainChar(MainCharacter* mainCharacter, RECT rect)
{
	if (mainCharacter->right) {
		mainCharacter->Pos.left += 10;
		mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;

		if (mainCharacter->Pos.right >= rect.right){
			mainCharacter->Pos.right = rect.right;
			mainCharacter->Pos.left = rect.right - mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;
		}
	}
	else if(mainCharacter->left){
		mainCharacter->Pos.right -= 10;
		mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;
		if (mainCharacter->Pos.left <= rect.left) {
			mainCharacter->Pos.left = rect.left;
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;
		}
	}
}

void JumpMainChar(MainCharacter* mainCharacter, int jumpTime, RECT rect)
{
	if (jumpTime < 20) {
		mainCharacter->Pos.top -= 10;
		mainCharacter->Pos.bottom -= 10;
	}
	else{
		mainCharacter->Pos.top += 10;
		mainCharacter->Pos.bottom += 10;
	}
	if (mainCharacter->right) {
		mainCharacter->Pos.left += 10;
		mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;

		if (mainCharacter->Pos.right >= rect.right) {
			mainCharacter->Pos.right = rect.right;
			mainCharacter->Pos.left = rect.right - mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;
		}
	}
	else if(mainCharacter->left) {
		mainCharacter->Pos.right -= 10;
		mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;
		if (mainCharacter->Pos.left <= rect.left) {
			mainCharacter->Pos.left = rect.left;
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;
		}
	}
}

void ImpactMainChar(MainCharacter* mainCharacter, int* stage)
{
	if (mainCharacter->heart > 0)
		mainCharacter->heart--;
	else{//게임오버
		*stage = 0;
	}

	mainCharacter->state = 6;
}

void HitBoxMainChar(MainCharacter* mainCharacter)
{
	switch (mainCharacter->state)
	{
	case 0:
		if (mainCharacter->direction)
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->IDLEBitData[mainCharacter->animationNum].bmWidth / 2;
		else 
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->IDLEBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->IDLEBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 1:
		if (mainCharacter->direction) 
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;
		else
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->RUNBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->RUNBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 2:	
		if (mainCharacter->direction) 
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;
		else 
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->JUMPBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->JUMPBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 3:
		if (mainCharacter->direction) 
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->SHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		else 
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->SHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->SHOOTBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 4:
		if (mainCharacter->direction) 
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->RUNSHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		else
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->RUNSHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->RUNSHOOTBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 5:
		if (mainCharacter->direction)
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->EXSHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		else
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->EXSHOOTBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->EXSHOOTBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	case 6:
		if (mainCharacter->direction) 
			mainCharacter->Pos.right = mainCharacter->Pos.left + mainCharacter->HITBitData[mainCharacter->animationNum].bmWidth / 2;
		else
			mainCharacter->Pos.left = mainCharacter->Pos.right - mainCharacter->HITBitData[mainCharacter->animationNum].bmWidth / 2;
		mainCharacter->Pos.top = mainCharacter->Pos.bottom - mainCharacter->HITBitData[mainCharacter->animationNum].bmHeight / 2;
		break;
	default:
		break;
	}
}