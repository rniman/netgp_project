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