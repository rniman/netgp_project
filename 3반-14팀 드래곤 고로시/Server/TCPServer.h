#pragma once
#include "character.h"

struct ThreadParams
{
	SOCKET socket;
	HANDLE hThread;
};

#pragma pack(1)
struct SendUpdateData
{
	MainCharacterInfo player1, player2;
	//BossMonster bossMonster;
};
#pragma pack()

int SendDefaultData(SOCKET client, const SendUpdateData& updateData)
{
	int retval;
	int len = sizeof(SendUpdateData);

	retval = send(client, (char*)&len, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) 
	{
		err_display("recv()");
		return -1;
	}

	retval = send(client, (char*)&updateData, len, MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return -1;
	}
}