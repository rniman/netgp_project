#include "ClientCharacter.h"

void LoadBullet(BulletBitmap* bulletBitmap, HINSTANCE g_hInst)
{
	TCHAR temp[40];
	BITMAP bmData;

	bulletBitmap->LOOPBITMAP = (HBITMAP)LoadImage(g_hInst, L"CUPHEAD/bullet/bullet1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(bulletBitmap->LOOPBITMAP, sizeof(BITMAP), &bmData);
	bulletBitmap->LOOPBitData.bmWidth = bmData.bmWidth;
	bulletBitmap->LOOPBitData.bmHeight = bmData.bmHeight;

	bulletBitmap->EXBITMAP = (HBITMAP)LoadImage(g_hInst, L"CUPHEAD/exbullet/EX_bullet1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(bulletBitmap->EXBITMAP, sizeof(BITMAP), &bmData);
	bulletBitmap->EXBitData.bmWidth = bmData.bmWidth;
	bulletBitmap->EXBitData.bmHeight = bmData.bmHeight;

	for (int i = 0; i < 6; ++i)
	{
		wsprintf(temp, L"CUPHEAD/bullet/death_bullet%d.bmp", i + 1);
		bulletBitmap->DEATHLOOPBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(bulletBitmap->DEATHLOOPBITMAP[i], sizeof(BITMAP), &bmData);
		bulletBitmap->DEATHLOOPBitData[i].bmWidth = bmData.bmWidth;
		bulletBitmap->DEATHLOOPBitData[i].bmHeight = bmData.bmHeight;
	}
	for (int i = 0; i < 9; ++i)
	{
		wsprintf(temp, L"CUPHEAD/exbullet/death_EX_bullet%d.bmp", i + 1);
		bulletBitmap->DEATHEXBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(bulletBitmap->DEATHEXBITMAP[i], sizeof(BITMAP), &bmData);
		bulletBitmap->DEATHEXBitData[i].bmWidth = bmData.bmWidth;
		bulletBitmap->DEATHEXBitData[i].bmHeight = bmData.bmHeight;
	}
}

void DeleteBitBullet(BulletBitmap* bulletBitmap)
{
	DeleteObject(bulletBitmap->LOOPBITMAP);
	DeleteObject(bulletBitmap->EXBITMAP);
	for (int i = 0; i < 6; ++i)
	{
		DeleteObject(bulletBitmap->DEATHLOOPBITMAP[i]);
	}
	for (int i = 0; i < 9; ++i)
	{
		DeleteObject(bulletBitmap->DEATHEXBITMAP[i]);
	}
}

void PaintBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap)
{
	for (int i = 0; i < BULLETNUM; ++i)
	{
		if (!mainCharacter.info.bullet[i].exist)
		{
			continue;
		}

		if (mainCharacter.info.bullet[i].direction)
		{
			if (mainCharacter.info.bullet[i].EX)
			{
				SelectObject(ObjectDC, bulletBitmap.EXBITMAP);
				TransparentBlt(backMemDC,
					mainCharacter.info.bullet[i].Pos.left,
					mainCharacter.info.bullet[i].Pos.top,
					bulletBitmap.EXBitData.bmWidth / 2,
					bulletBitmap.EXBitData.bmHeight / 2,
					ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
			else
			{
				SelectObject(ObjectDC, bulletBitmap.LOOPBITMAP);
				TransparentBlt(backMemDC,
					mainCharacter.info.bullet[i].Pos.left,
					mainCharacter.info.bullet[i].Pos.top,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
		}
		else
		{
			HBITMAP temp;
			HDC tempDC;
			if (mainCharacter.info.bullet[i].EX)
			{
				tempDC = CreateCompatibleDC(ObjectDC);
				SelectObject(tempDC, bulletBitmap.EXBITMAP);
				temp = CreateCompatibleBitmap(backMemDC, bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight);
				SelectObject(ObjectDC, temp);

				StretchBlt(ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					tempDC,
					bulletBitmap.EXBitData.bmWidth - 1, 0,
					-bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					SRCCOPY);

				TransparentBlt(backMemDC,
					mainCharacter.info.bullet[i].Pos.left,
					mainCharacter.info.bullet[i].Pos.top,
					bulletBitmap.EXBitData.bmWidth / 2,
					bulletBitmap.EXBitData.bmHeight / 2,
					ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
			else
			{
				tempDC = CreateCompatibleDC(ObjectDC);
				SelectObject(tempDC, bulletBitmap.LOOPBITMAP);
				temp = CreateBitmap(bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight, 1, 24, NULL);
				SelectObject(ObjectDC, temp);

				StretchBlt(ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					tempDC,
					bulletBitmap.LOOPBitData.bmWidth - 1, 0,
					-bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					SRCCOPY);

				TransparentBlt(backMemDC,
					mainCharacter.info.bullet[i].Pos.left,
					mainCharacter.info.bullet[i].Pos.top,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
			DeleteDC(tempDC);
			DeleteObject(temp);
		}
	}
}

void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap)
{
	for (int i = 0; i < BULLETNUM; ++i)
	{
		if (mainCharacter.info.bullet[i].exist)
		{
			continue;
		}

		if (mainCharacter.info.bullet[i].deathTime <= 0)
		{
			continue;
		}

		if (mainCharacter.info.bullet[i].EX)
		{
			SelectObject(ObjectDC, bulletBitmap.DEATHEXBITMAP[mainCharacter.info.bullet[i].animationNum]);
			TransparentBlt(backMemDC,
				(mainCharacter.info.bullet[i].Pos.right + mainCharacter.info.bullet[i].Pos.left) / 2 - bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmWidth / 6,
				mainCharacter.info.bullet[i].Pos.bottom - bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmHeight / 3,
				bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmWidth / 3,
				bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmHeight / 3,
				ObjectDC, 0, 0,
				bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmWidth,
				bulletBitmap.DEATHEXBitData[mainCharacter.info.bullet[i].animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else
		{
			SelectObject(ObjectDC, bulletBitmap.DEATHLOOPBITMAP[mainCharacter.info.bullet[i].animationNum]);
			TransparentBlt(backMemDC,
				(mainCharacter.info.bullet[i].Pos.right + mainCharacter.info.bullet[i].Pos.left) / 2 - bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmWidth / 4,
				mainCharacter.info.bullet[i].Pos.bottom - bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmHeight / 4,
				bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmWidth / 2,
				bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmHeight / 2,
				ObjectDC, 0, 0,
				bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmWidth,
				bulletBitmap.DEATHLOOPBitData[mainCharacter.info.bullet[i].animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
	}
}

