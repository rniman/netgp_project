#include "ClientCharacter.h"

void CreateMainChar(HINSTANCE g_hInst, MainCharacter* mainCharacter)
{
	mainCharacter->info.state = MainState::IDLE;
	mainCharacter->info.animationNum = 0;
	mainCharacter->info.right = FALSE;
	mainCharacter->info.left = FALSE;
	mainCharacter->info.direction = TRUE;

	mainCharacter->info.heart = 6;

	mainCharacter->bitmap.HEARTBITMAP[0] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mainCharacter->bitmap.HEARTBITMAP[1] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mainCharacter->bitmap.HEARTBITMAP[2] = (HBITMAP)LoadImage(g_hInst, L"Heart/heart3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	TCHAR temp[50];
	BITMAP bmData;
	for (int i = 0; i < IDLEANI; ++i) 
	{
		wsprintf(temp, L"CUPHEAD/idle/cuphead_idle_00%02d.bmp", i + 1);
		mainCharacter->bitmap.IDLEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.IDLEBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.IDLEBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.IDLEBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < RUNANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/run/cuphead_run_00%02d.bmp", i + 1);
		mainCharacter->bitmap.RUNBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.RUNBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.RUNBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.RUNBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < JUMPANI; ++i) 
	{
		wsprintf(temp, L"CUPHEAD/jump/cuphead_jump_00%02d.bmp", i + 1);
		mainCharacter->bitmap.JUMPBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.JUMPBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.JUMPBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.JUMPBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < SHOOTANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/shoot/cuphead_shoot_straight_00%02d.bmp", i + 1);
		mainCharacter->bitmap.SHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.SHOOTBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.SHOOTBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.SHOOTBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < RUNSHOOTANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/runshoot/cuphead_run_shoot_00%02d.bmp", i + 1);
		mainCharacter->bitmap.RUNSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.RUNSHOOTBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.RUNSHOOTBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.RUNSHOOTBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < EXSHOOTANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/exshoot/cuphead_ex_straight_00%02d.bmp", i + 1);
		mainCharacter->bitmap.EXSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.EXSHOOTBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.EXSHOOTBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.EXSHOOTBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < HITANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/hit/cuphead_hit_00%02d.bmp", i + 1);
		mainCharacter->bitmap.HITBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.HITBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.HITBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.HITBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < GHOSTANI; ++i)
	{
		wsprintf(temp, L"CUPHEAD/ghost/cuphead_ghost_00%02d.bmp", i + 1);
		mainCharacter->bitmap.GHOSTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.GHOSTBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.GHOSTBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.GHOSTBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < REVIVEANI; ++i) 
	{
		wsprintf(temp, L"CUPHEAD/revive/cuphead_revive_00%02d.bmp", i + 1);
		mainCharacter->bitmap.REVIVEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(mainCharacter->bitmap.REVIVEBITMAP[i], sizeof(BITMAP), &bmData);
		mainCharacter->bitmap.REVIVEBitData[i].bmWidth = bmData.bmWidth;
		mainCharacter->bitmap.REVIVEBitData[i].bmHeight = bmData.bmHeight;
	}

	mainCharacter->info.Pos.left = 0;
	mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmWidth / 2;
	mainCharacter->info.Pos.bottom = STAGEFLOOR;
	mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmHeight / 2;

	mainCharacter->info.energy = -1;

	for (int i = 0; i < BULLETNUM; ++i)
	{
		mainCharacter->info.bullet[i].exist = false;
		mainCharacter->info.bullet[i].animationNum = 0;
		mainCharacter->info.bullet[i].EX = false;
		mainCharacter->info.bullet[i].deathTime = 0;
		mainCharacter->info.bullet[i].direction = true;
		//mainCharacter->info.bullet[i].hitBox = false;
		//mainCharacter->info.bullet[i].Pos = false;
	}
}

void InitMainChar(MainCharacter* mainCharacter)
{
	mainCharacter->info.state = MainState::IDLE;
	mainCharacter->info.animationNum = 0;
	mainCharacter->info.right = FALSE;
	mainCharacter->info.left = FALSE;
	mainCharacter->info.direction = TRUE;

	mainCharacter->info.Pos.left = 0;
	mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmWidth / 2;
	mainCharacter->info.Pos.bottom = STAGEFLOOR;
	mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmHeight / 2;

	mainCharacter->info.energy = -1;
}

void DeleteMainChar(MainCharacter* mainCharacter)
{
	for (int i = 0; i < IDLEANI; ++i)
		DeleteObject(mainCharacter->bitmap.IDLEBITMAP[i]);
	for (int i = 0; i < RUNANI; ++i)
		DeleteObject(mainCharacter->bitmap.RUNBITMAP[i]);
	for (int i = 0; i < JUMPANI; ++i)
		DeleteObject(mainCharacter->bitmap.JUMPBITMAP[i]);
	for (int i = 0; i < SHOOTANI; ++i)
		DeleteObject(mainCharacter->bitmap.SHOOTBITMAP[i]);
	for (int i = 0; i < RUNSHOOTANI; ++i)
		DeleteObject(mainCharacter->bitmap.RUNSHOOTBITMAP[i]);
	for (int i = 0; i < EXSHOOTANI; ++i)
		DeleteObject(mainCharacter->bitmap.EXSHOOTBITMAP[i]);
	for (int i = 0; i < HITANI; ++i)
		DeleteObject(mainCharacter->bitmap.HITBITMAP[i]);
	for (int i = 0; i < GHOSTANI; ++i)
		DeleteObject(mainCharacter->bitmap.GHOSTBITMAP[i]);
	for (int i = 0; i < REVIVEANI; ++i)
		DeleteObject(mainCharacter->bitmap.REVIVEBITMAP[i]);
	for (int i = 0; i < 3; ++i)
		DeleteObject(mainCharacter->bitmap.HEARTBITMAP[i]);

}

void PaintMainCharacter(HDC backMemDC, HDC ObjectDC, DWORD playerNum, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap)
{
	if (mainCharacter.info.type == 0)
	{
		return;
	}

	if (mainCharacter.info.heart <= 0)PaintGhost(backMemDC, ObjectDC, mainCharacter);
	else if (mainCharacter.info.state == MainState::IDLE || mainCharacter.info.state == MainState::RUN) PaintMainChar(backMemDC, ObjectDC, mainCharacter);
	else if (mainCharacter.info.state == MainState::JUMP) PaintJump(backMemDC, ObjectDC, mainCharacter);
	else if (mainCharacter.info.state == MainState::SHOOT || mainCharacter.info.state == MainState::RUNSHOOT)PaintShootMainChar(backMemDC, ObjectDC, mainCharacter);
	else if (mainCharacter.info.state == MainState::EXSHOOT) PaintEXShoot(backMemDC, ObjectDC, mainCharacter);
	else if (mainCharacter.info.state == MainState::HIT)PaintHIT(backMemDC, ObjectDC, mainCharacter);

	if(mainCharacter.info.type == playerNum)
	{
		PaintHeart(backMemDC, ObjectDC, mainCharacter);
	}
	
	PaintBullet(backMemDC, ObjectDC, mainCharacter, bulletBitmap);
	PaintDeathBullet(backMemDC, ObjectDC, mainCharacter, bulletBitmap);
}

void PaintHeart(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	for (int i = 0; i < mainCharacter.info.heart / 2; ++i) 
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[0]);
		TransparentBlt(backMemDC, 50 * i, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}

	if (mainCharacter.info.heart == 5)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 50 * 2, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.info.heart <= 4)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 50 * 2, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.info.heart == 3) 
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 50 * 1, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.info.heart <= 2) 
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 50 * 1, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.info.heart == 1) 
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[1]);
		TransparentBlt(backMemDC, 0, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}
	if (mainCharacter.info.heart <= 0)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.HEARTBITMAP[2]);
		TransparentBlt(backMemDC, 0, 520, 362 / 10, 352 / 10, ObjectDC, 0, 0, 362, 352, RGB(255, 0, 255));
	}

}

void PaintGhost(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	if (mainCharacter.info.responeTime > 280)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.GHOSTBITMAP[mainCharacter.info.animationNum]);
		TransparentBlt(
			backMemDC, 
			mainCharacter.info.Pos.left,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.GHOSTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.GHOSTBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.GHOSTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.GHOSTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.GHOSTBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
	else 
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.REVIVEBITMAP[mainCharacter.info.animationNum]);
		TransparentBlt(
			backMemDC,
			(mainCharacter.info.Pos.right + mainCharacter.info.Pos.left) / 2 - mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmWidth / 4,
			(mainCharacter.info.Pos.bottom + mainCharacter.info.Pos.top) / 2 - mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmHeight / 4,
			mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.REVIVEBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
}

void PaintMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.info.state == MainState::IDLE)
	{
		if (mainCharacter.info.direction)
		{
			SelectObject(ObjectDC, mainCharacter.bitmap.IDLEBITMAP[mainCharacter.info.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.info.Pos.left,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else //뒤집어서 그린다.
		{
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.bitmap.IDLEBITMAP[mainCharacter.info.animationNum]);
			temp = CreateBitmap(mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(
				ObjectDC,
				0, 
				0,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight,
				tempDC, mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth - 1, 0,
				-mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight, SRCCOPY
			);

			TransparentBlt(
				backMemDC, 
				mainCharacter.info.Pos.right - mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.IDLEBitData[mainCharacter.info.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
			
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
	else 
	{
		if (mainCharacter.info.direction)
		{
			SelectObject(ObjectDC, mainCharacter.bitmap.RUNBITMAP[mainCharacter.info.animationNum]);
			TransparentBlt(backMemDC, mainCharacter.info.Pos.left,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight, RGB(255, 0, 255));
		}
		else
		{
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.bitmap.RUNBITMAP[mainCharacter.info.animationNum]);
			temp = CreateBitmap(mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight,
				tempDC,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth - 1, \
				0,
				-mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight, SRCCOPY);

			TransparentBlt(backMemDC, mainCharacter.info.Pos.right - mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNBitData[mainCharacter.info.animationNum].bmHeight, RGB(255, 0, 255)
			);
			
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
}

void PaintJump(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.info.direction)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.JUMPBITMAP[mainCharacter.info.animationNum]);
		TransparentBlt(backMemDC,
			mainCharacter.info.Pos.left,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
	else 
	{
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.bitmap.JUMPBITMAP[mainCharacter.info.animationNum]);
		temp = CreateBitmap(mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight,
			tempDC,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth - 1, \
			0,
			-mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight, SRCCOPY);

		TransparentBlt(backMemDC,
			mainCharacter.info.Pos.right - mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, 
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.JUMPBitData[mainCharacter.info.animationNum].bmHeight, RGB(255, 0, 255));
		
		DeleteDC(tempDC);
		DeleteObject(temp);
	}

}

void PaintShootMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.info.state == MainState::SHOOT)
	{
		if (mainCharacter.info.direction)
		{
			SelectObject(ObjectDC, mainCharacter.bitmap.SHOOTBITMAP[mainCharacter.info.animationNum]);
			TransparentBlt(backMemDC,
				mainCharacter.info.Pos.left,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else 
		{
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.bitmap.SHOOTBITMAP[mainCharacter.info.animationNum]);
			temp = CreateBitmap(mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);

			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				tempDC, mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth - 1, 0,
				-mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight, SRCCOPY);

			TransparentBlt(backMemDC, 
				mainCharacter.info.Pos.right - mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.SHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				RGB(255, 0, 255)
			);

			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
	else 
	{
		if (mainCharacter.info.direction)
		{
			SelectObject(ObjectDC, mainCharacter.bitmap.RUNSHOOTBITMAP[mainCharacter.info.animationNum]);
			TransparentBlt(backMemDC, 
				mainCharacter.info.Pos.left,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else 
		{
			tempDC = CreateCompatibleDC(ObjectDC);
			SelectObject(tempDC, mainCharacter.bitmap.RUNSHOOTBITMAP[mainCharacter.info.animationNum]);
			temp = CreateBitmap(mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
			SelectObject(ObjectDC, temp);

			StretchBlt(ObjectDC, 0, 0,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				tempDC,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth - 1,
				0,
				-mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
				SRCCOPY);

			TransparentBlt(backMemDC, 
				mainCharacter.info.Pos.right - mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.info.Pos.bottom - mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
				ObjectDC, 0, 0, mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
				mainCharacter.bitmap.RUNSHOOTBitData[mainCharacter.info.animationNum].bmHeight, 
				RGB(255, 0, 255)
			);
			
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
}

void PaintEXShoot(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.info.direction)
	{
		SelectObject(ObjectDC, mainCharacter.bitmap.EXSHOOTBITMAP[mainCharacter.info.animationNum]);
		TransparentBlt(backMemDC,
			mainCharacter.info.Pos.left,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
	else 
	{
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.bitmap.EXSHOOTBITMAP[mainCharacter.info.animationNum]);
		temp = CreateBitmap(mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
			tempDC,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth - 1,
			0,
			-mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
			SRCCOPY);

		TransparentBlt(backMemDC,
			mainCharacter.info.Pos.right - mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.EXSHOOTBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
		
		DeleteDC(tempDC);
		DeleteObject(temp);
	}
}

void PaintHIT(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter)
{
	HBITMAP temp;
	HDC tempDC;
	if (mainCharacter.info.direction) {
		SelectObject(ObjectDC, mainCharacter.bitmap.HITBITMAP[mainCharacter.info.animationNum]);
		TransparentBlt(backMemDC, 
			mainCharacter.info.Pos.left,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);
	}
	else {
		tempDC = CreateCompatibleDC(ObjectDC);
		SelectObject(tempDC, mainCharacter.bitmap.HITBITMAP[mainCharacter.info.animationNum]);
		temp = CreateBitmap(mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight, 1, 32, NULL);
		SelectObject(ObjectDC, temp);

		StretchBlt(ObjectDC, 0, 0,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight,
			tempDC,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth - 1,
			0,
			-mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight,
			SRCCOPY);

		TransparentBlt(backMemDC,
			mainCharacter.info.Pos.right - mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.info.Pos.bottom - mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight / 2,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth / 2,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight / 2,
			ObjectDC, 0, 0, mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmWidth,
			mainCharacter.bitmap.HITBitData[mainCharacter.info.animationNum].bmHeight,
			RGB(255, 0, 255)
		);

		DeleteDC(tempDC);
		DeleteObject(temp);
	}
}

void MoveMainChar(MainCharacter* mainCharacter, RECT rect)
{
	if (mainCharacter->info.right) 
	{
		mainCharacter->info.Pos.left += 10;
		mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;

		if (mainCharacter->info.Pos.right >= rect.right) 
		{
			mainCharacter->info.Pos.right = rect.right;
			mainCharacter->info.Pos.left = rect.right - mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;
		}
	}
	else if (mainCharacter->info.left)
	{
		mainCharacter->info.Pos.right -= 10;
		mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;
		if (mainCharacter->info.Pos.left <= rect.left) 
		{
			mainCharacter->info.Pos.left = rect.left;
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;
		}
	}
}

void JumpMainChar(MainCharacter* mainCharacter, RECT rect)
{
	if (mainCharacter->info.jumpTime < 20)
	{
		mainCharacter->info.Pos.top -= 10;
		mainCharacter->info.Pos.bottom -= 10;
	}
	else
	{
		mainCharacter->info.Pos.top += 10;
		mainCharacter->info.Pos.bottom += 10;
	}

	if (mainCharacter->info.right) 
	{
		mainCharacter->info.Pos.left += 10;
		mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;

		if (mainCharacter->info.Pos.right >= rect.right)
		{
			mainCharacter->info.Pos.right = rect.right;
			mainCharacter->info.Pos.left = rect.right - mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;
		}
	}
	else if (mainCharacter->info.left)
	{
		mainCharacter->info.Pos.right -= 10;
		mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;
		
		if (mainCharacter->info.Pos.left <= rect.left)
		{
			mainCharacter->info.Pos.left = rect.left;
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;
		}
	}
}

void ImpactMainChar(MainCharacter* mainCharacter, int* stage)
{
	if (mainCharacter->info.heart > 0)
	{
		mainCharacter->info.heart--;
	}
	else //게임오버
	{
		*stage = 0;
	}

	mainCharacter->info.state = MainState::HIT;
}

void HitBoxMainChar(MainCharacter* mainCharacter)
{
	switch (mainCharacter->info.state)
	{
	case MainState::IDLE:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::RUN:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else 
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.RUNBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::JUMP:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.JUMPBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::SHOOT:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.SHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.SHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.SHOOTBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::RUNSHOOT:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.RUNSHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.RUNSHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.RUNSHOOTBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::EXSHOOT:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.EXSHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.EXSHOOTBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.EXSHOOTBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	case MainState::HIT:
		if (mainCharacter->info.direction)
			mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.HITBitData[mainCharacter->info.animationNum].bmWidth / 2;
		else
			mainCharacter->info.Pos.left = mainCharacter->info.Pos.right - mainCharacter->bitmap.HITBitData[mainCharacter->info.animationNum].bmWidth / 2;
		mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.HITBitData[mainCharacter->info.animationNum].bmHeight / 2;
		break;
	default:
		break;
	}
}