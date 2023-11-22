#include "ServerCharacter.h"


void CreateMainChar(/*HINSTANCE g_hInst,*/ MainCharacter* mainCharacter)
{

	mainCharacter->info.state = MainState::IDLE;
	mainCharacter->info.animationNum = 0;
	mainCharacter->info.right = FALSE;
	mainCharacter->info.left = FALSE;
	mainCharacter->info.direction = TRUE;

	mainCharacter->info.heart = 6;

	// BITDATA를 CLIENT에서 넘겨받는 부분을 추가해서 여기서 초기화 해줘야함
	// 
	//TCHAR temp[50];
	//BITMAP bmData;
	//for (int i = 0; i < IDLEANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/idle/cuphead_idle_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.IDLEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.IDLEBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.IDLEBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.IDLEBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < RUNANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/run/cuphead_run_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.RUNBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.RUNBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.RUNBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.RUNBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < JUMPANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/jump/cuphead_jump_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.JUMPBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.JUMPBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.JUMPBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.JUMPBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < SHOOTANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/shoot/cuphead_shoot_straight_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.SHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.SHOOTBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.SHOOTBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.SHOOTBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < RUNSHOOTANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/runshoot/cuphead_run_shoot_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.RUNSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.RUNSHOOTBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.RUNSHOOTBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.RUNSHOOTBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < EXSHOOTANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/exshoot/cuphead_ex_straight_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.EXSHOOTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.EXSHOOTBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.EXSHOOTBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.EXSHOOTBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < HITANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/hit/cuphead_hit_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.HITBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.HITBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.HITBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.HITBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < GHOSTANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/ghost/cuphead_ghost_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.GHOSTBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.GHOSTBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.GHOSTBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.GHOSTBitData[i].bmHeight = bmData.bmHeight;
	//}
	//for (int i = 0; i < REVIVEANI; ++i)
	//{
	//	wsprintf(temp, L"CUPHEAD/revive/cuphead_revive_00%02d.bmp", i + 1);
	//	mainCharacter->bitmap.REVIVEBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	GetObject(mainCharacter->bitmap.REVIVEBITMAP[i], sizeof(BITMAP), &bmData);
	//	mainCharacter->bitmap.REVIVEBitData[i].bmWidth = bmData.bmWidth;
	//	mainCharacter->bitmap.REVIVEBitData[i].bmHeight = bmData.bmHeight;
	//}

	mainCharacter->info.Pos.left = 0;
	mainCharacter->info.Pos.right = 49;
	//mainCharacter->info.Pos.right = mainCharacter->info.Pos.left + mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmWidth / 2;
	mainCharacter->info.Pos.bottom = STAGEFLOOR;
	mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - 77;
	//mainCharacter->info.Pos.top = mainCharacter->info.Pos.bottom - mainCharacter->bitmap.IDLEBitData[mainCharacter->info.animationNum].bmHeight / 2;

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