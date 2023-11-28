#include "ServerCharacter.h"


void CreateMainChar(/*HINSTANCE g_hInst,*/ MainCharacter* mainCharacter)
{
	mainCharacter->info.state = MainState::IDLE;
	mainCharacter->info.animationNum = 0;
	mainCharacter->info.right = FALSE;
	mainCharacter->info.left = FALSE;
	mainCharacter->info.direction = TRUE;

	mainCharacter->info.heart = 6;

	mainCharacter->info.coolTime = 0;
	mainCharacter->info.invincibleTime = 0;
	mainCharacter->info.jumpTime = 0;
	mainCharacter->info.responeTime = 0;

	//mainCharacter->info.type


	mainCharacter->info.Pos.left = 0;
	mainCharacter->info.Pos.right = 49;
	mainCharacter->info.Pos.bottom = STAGEFLOOR;
	mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - 77;

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

//void MainLoop(RECT& rect,MainCharacter& mainCharacter, BossMonster& Boss)
//{
//	//방향키 확인
//	if (GetKeyState(VK_RIGHT) >= 0 && mainCharacter.info.right)
//	{
//		mainCharacter.info.right = FALSE;
//	}
//	if (GetKeyState(VK_LEFT) >= 0 && mainCharacter.info.left)
//	{
//		mainCharacter.info.left = FALSE;
//	}
//
//	HitBoxMainChar(&mainCharacter);
//
//	//쿨타임 
//	if (mainCharacter.info.coolTime > 0)mainCharacter.info.coolTime--;
//	else mainCharacter.info.coolTime = 0;
//
//	BossAttackMeteor(rect, &Boss, &mainCharacter, &oldState, &oldAnimationNum); //oldState 수정필요
//
//	if (mainCharacter.info.invincibleTime != 0)
//	{
//		mainCharacter.info.invincibleTime--;
//	}
//
//	if (mainCharacter.info.heart <= 0)
//	{
//		if (mainCharacter.info.responeTime == 0)
//		{
//			mainCharacter.info.responeTime = 500;
//		}
//
//		if (mainCharacter.info.responeTime > 280)
//		{
//			mainCharacter.info.animationNum++;
//			if (mainCharacter.info.animationNum > 23)
//			{
//				mainCharacter.info.animationNum = 0;
//			}
//			mainCharacter.info.responeTime--;
//		}
//		else
//		{
//			if (mainCharacter.info.responeTime == 280)
//			{
//				mainCharacter.info.animationNum = 0;
//			}
//			else if (mainCharacter.info.responeTime % 10 == 0)
//			{
//				mainCharacter.info.animationNum++;
//			}
//			mainCharacter.info.responeTime -= 5;
//		}
//
//
//		if (mainCharacter.info.responeTime == 0)
//		{
//			mainCharacter.info.heart = 6;
//
//			if (oldState != MainState::JUMP) mainCharacter.info.state = MainState::IDLE;
//			else mainCharacter.info.state = MainState::JUMP;
//
//			mainCharacter.info.animationNum = 0;
//			mainCharacter.info.right = FALSE;
//			mainCharacter.info.left = FALSE;
//			mainCharacter.info.direction = TRUE;
//			mainCharacter.info.energy = -1;
//			mainCharacter.info.invincibleTime = 100;
//		}
//	}
//	else if (mainCharacter.info.state == MainState::HIT)
//	{
//		if (mainCharacter.info.invincibleTime % 10 == 0)
//			mainCharacter.info.animationNum++;
//
//		if (mainCharacter.info.animationNum == 5)
//		{
//			if (oldState == MainState::EXSHOOT)
//				mainCharacter.info.state = MainState::IDLE;
//			else
//				mainCharacter.info.state = oldState;
//			mainCharacter.info.animationNum = oldAnimationNum;
//		}
//	}
//	else if (mainCharacter.info.state == MainState::JUMP)
//	{	//점프
//		mainCharacter.info.animationNum++;
//		JumpMainChar(&mainCharacter, rect);
//		if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.info.coolTime == 0)
//		{
//			CreateBullet(mainCharacter, bulletBitmap);
//			mainCharacter.info.coolTime = 18;
//		}
//
//		if (mainCharacter.info.animationNum > 7)
//		{
//			mainCharacter.info.animationNum = 0;
//		}
//		mainCharacter.info.jumpTime++;
//
//		//점프 후 낙하시 발판이 있으면 발판에 찾기
//		if (mainCharacter.info.jumpTime >= 40)
//		{
//			mainCharacter.info.state = MainState::IDLE;
//			mainCharacter.info.jumpTime = 0;
//		}
//	}
//	else if (mainCharacter.info.state == MainState::EXSHOOT)
//	{
//		mainCharacter.info.energy++;
//		if (mainCharacter.info.energy == 0)mainCharacter.info.animationNum = 0;
//		else if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < 4)mainCharacter.info.animationNum++;
//
//		if (GetKeyState(VK_SHIFT) >= 0)
//		{
//			if (mainCharacter.info.animationNum < 4)
//			{
//				mainCharacter.info.state = MainState::IDLE;
//				mainCharacter.info.energy = -1;
//			}
//			else if (mainCharacter.info.animationNum == 4)
//			{
//				CreateBullet(mainCharacter, bulletBitmap);
//				mainCharacter.info.animationNum++;
//			}
//		}
//
//		if (mainCharacter.info.energy % 5 == 0 && mainCharacter.info.animationNum < EXSHOOTANI - 1 && mainCharacter.info.animationNum > 4)
//		{
//			mainCharacter.info.animationNum++;
//		}
//		else if (mainCharacter.info.animationNum == EXSHOOTANI - 1)
//		{
//			mainCharacter.info.state = MainState::IDLE;
//			mainCharacter.info.energy = -1;
//		}
//	}
//	else if (mainCharacter.info.left && mainCharacter.info.right)
//	{
//		if (GetKeyState(VK_CONTROL) >= 0)
//		{
//			mainCharacter.info.state = MainState::IDLE;
//		}
//		else
//		{
//			mainCharacter.info.state = MainState::SHOOT;
//		}
//
//		mainCharacter.info.Pos.right = mainCharacter.info.Pos.left + IDLEWIDTH;
//	}
//	else if (mainCharacter.info.right || mainCharacter.info.left)
//	{
//		mainCharacter.info.animationNum++;
//		if (GetKeyState(VK_CONTROL) >= 0)
//		{
//			mainCharacter.info.state = MainState::RUN;
//		}
//		else
//		{
//			if (mainCharacter.info.coolTime == 0)
//			{
//				CreateBullet(mainCharacter, bulletBitmap);
//				mainCharacter.info.coolTime = 18;
//			}
//			mainCharacter.info.state = MainState::RUNSHOOT;
//		}
//
//		MoveMainChar(&mainCharacter, rect);
//		if (mainCharacter.info.animationNum > 15)
//		{
//			mainCharacter.info.animationNum = 0;
//		}
//	}
//
//	//상태 변환
//	if (mainCharacter.info.state == MainState::HIT || mainCharacter.info.heart <= 0) {}
//	else if (GetKeyState(VK_RIGHT) < 0)mainCharacter.info.direction = TRUE;
//	else if (GetKeyState(VK_LEFT) < 0)mainCharacter.info.direction = FALSE;
//	else if (GetKeyState(VK_CONTROL) < 0 && mainCharacter.info.state != MainState::JUMP) mainCharacter.info.state = MainState::SHOOT;
//	else if (mainCharacter.info.state != MainState::JUMP && mainCharacter.info.state != MainState::EXSHOOT) mainCharacter.info.state = MainState::IDLE;
//
//	MoveBullet(mainCharacter, rect);
//	DeathBullet(mainCharacter);
//}
