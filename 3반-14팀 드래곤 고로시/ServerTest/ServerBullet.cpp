#include "ServerCharacter.h"

void PaintBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BULLETBITMAP& bulletBitmap)
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

void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BULLETBITMAP& bulletBitmap)
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

void CreateBullet(MainCharacter& mainCharacter, const BULLETBITMAP& bulletBit)
{
	int curNum = 0;
	for (curNum; curNum < BULLETNUM; ++curNum)
	{
		if (!mainCharacter.info.bullet[curNum].exist && mainCharacter.info.bullet[curNum].deathTime <= 0)
		{
			break;
		}
	}

	mainCharacter.info.bullet[curNum].direction = mainCharacter.info.direction;
	mainCharacter.info.bullet[curNum].exist = TRUE;
	mainCharacter.info.bullet[curNum].animationNum = 0;

	if (mainCharacter.info.state != MainState::EXSHOOT)
	{
		mainCharacter.info.bullet[curNum].EX = FALSE;
	}
	else
	{
		mainCharacter.info.bullet[curNum].EX = TRUE;
	}

	mainCharacter.info.bullet[curNum].deathTime = 45;

	if (mainCharacter.info.bullet[curNum].EX)
	{
		if (mainCharacter.info.bullet[curNum].direction)
		{
			mainCharacter.info.bullet[curNum].Pos.left = mainCharacter.info.Pos.right - 40;
			mainCharacter.info.bullet[curNum].Pos.right = mainCharacter.info.bullet[curNum].Pos.left + bulletBit.EXBitData.bmWidth / 2;

			mainCharacter.info.bullet[curNum].hitBox.left = mainCharacter.info.bullet[curNum].Pos.left + 158 / 2;
			mainCharacter.info.bullet[curNum].hitBox.right = mainCharacter.info.bullet[curNum].hitBox.left + EXBULWIDTH;
		}
		else
		{
			mainCharacter.info.bullet[curNum].Pos.right = mainCharacter.info.Pos.left + 40;
			mainCharacter.info.bullet[curNum].Pos.left = mainCharacter.info.bullet[curNum].Pos.right - bulletBit.EXBitData.bmWidth / 2;

			mainCharacter.info.bullet[curNum].hitBox.right = mainCharacter.info.bullet[curNum].Pos.right - 158 / 2;
			mainCharacter.info.bullet[curNum].hitBox.left = mainCharacter.info.bullet[curNum].hitBox.right - EXBULWIDTH;
		}

		mainCharacter.info.bullet[curNum].Pos.top = (mainCharacter.info.Pos.top + mainCharacter.info.Pos.bottom) / 2 - bulletBit.EXBitData.bmHeight / 4;
		mainCharacter.info.bullet[curNum].Pos.bottom = (mainCharacter.info.Pos.top + mainCharacter.info.Pos.bottom) / 2 + bulletBit.EXBitData.bmHeight / 4;

		mainCharacter.info.bullet[curNum].hitBox.top = mainCharacter.info.bullet[curNum].Pos.top + 9;
		mainCharacter.info.bullet[curNum].hitBox.bottom = mainCharacter.info.bullet[curNum].hitBox.top + EXBULHEIGHT;
	}
	else
	{
		if (mainCharacter.info.bullet[curNum].direction)
		{
			mainCharacter.info.bullet[curNum].Pos.left = mainCharacter.info.Pos.right - 20;
			mainCharacter.info.bullet[curNum].Pos.right = mainCharacter.info.bullet[curNum].Pos.left + bulletBit.LOOPBitData.bmWidth;

			mainCharacter.info.bullet[curNum].hitBox.left = mainCharacter.info.bullet[curNum].Pos.left + 11;
			mainCharacter.info.bullet[curNum].hitBox.right = mainCharacter.info.bullet[curNum].hitBox.left + BULWIDTH;
		}
		else
		{
			mainCharacter.info.bullet[curNum].Pos.right = mainCharacter.info.Pos.left + 20;
			mainCharacter.info.bullet[curNum].Pos.left = mainCharacter.info.bullet[curNum].Pos.right - bulletBit.LOOPBitData.bmWidth;

			mainCharacter.info.bullet[curNum].hitBox.right = mainCharacter.info.bullet[curNum].Pos.right - 11;
			mainCharacter.info.bullet[curNum].hitBox.left = mainCharacter.info.bullet[curNum].hitBox.right - BULWIDTH;
		}

		mainCharacter.info.bullet[curNum].Pos.top = (mainCharacter.info.Pos.top + mainCharacter.info.Pos.bottom) / 2 - bulletBit.LOOPBitData.bmHeight / 2;
		mainCharacter.info.bullet[curNum].Pos.bottom = (mainCharacter.info.Pos.top + mainCharacter.info.Pos.bottom) / 2 + bulletBit.LOOPBitData.bmHeight / 2;

		mainCharacter.info.bullet[curNum].hitBox.top = mainCharacter.info.bullet[curNum].Pos.top + 11;
		mainCharacter.info.bullet[curNum].hitBox.bottom = mainCharacter.info.bullet[curNum].hitBox.top + BULHEIGHT;
	}
}

void MoveBullet(MainCharacter& mainCharacter, const RECT& rect)
{
	for (int i = 0; i < BULLETNUM; ++i)
	{
		if (!mainCharacter.info.bullet[i].exist)
		{
			continue;
		}

		if (mainCharacter.info.bullet[i].direction)
		{
			mainCharacter.info.bullet[i].Pos.left += 20;
			mainCharacter.info.bullet[i].Pos.right += 20;

			mainCharacter.info.bullet[i].hitBox.left += 20;
			mainCharacter.info.bullet[i].hitBox.right += 20;

			if (mainCharacter.info.bullet[i].hitBox.left >= rect.right)
			{
				mainCharacter.info.bullet[i].exist = FALSE;
			}
		}
		else
		{
			mainCharacter.info.bullet[i].Pos.left -= 20;
			mainCharacter.info.bullet[i].Pos.right -= 20;

			mainCharacter.info.bullet[i].hitBox.left -= 20;
			mainCharacter.info.bullet[i].hitBox.right -= 20;

			if (mainCharacter.info.bullet[i].hitBox.right <= rect.left)
			{
				mainCharacter.info.bullet[i].exist = FALSE;
			}
		}
	}

}

void DeathBullet(MainCharacter& mainCharacter)
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

		mainCharacter.info.bullet[i].deathTime--;
		if (mainCharacter.info.bullet[i].deathTime % 5 == 0)
		{
			mainCharacter.info.bullet[i].animationNum++;
			if (mainCharacter.info.bullet[i].EX && mainCharacter.info.bullet[i].animationNum > 8)
			{
				mainCharacter.info.bullet[i].deathTime = 0;
			}
			else if (!mainCharacter.info.bullet[i].EX && mainCharacter.info.bullet[i].animationNum > 5)
			{
				mainCharacter.info.bullet[i].deathTime = 0;
			}
		}
	}
}