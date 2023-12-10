#include "ServerCharacter.h"

void CreateBullet(MainCharacter& mainCharacter, const BulletBitmap& bulletBit)
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