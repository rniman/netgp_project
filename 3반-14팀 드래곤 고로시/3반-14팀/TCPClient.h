#pragma once
#include "character.h"

#pragma pack(1)
struct SendUpdateData
{
	MainCharacterInfo player1, player2;
	//BossMonster bossMonster;
};
#pragma pack()