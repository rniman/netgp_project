#pragma once
#include "ServerCharacter.h"

struct ThreadParams
{
	SOCKET socket;
	HANDLE hThread;
};

#pragma pack(1)
struct SendUpdateData
{
	MainCharacterInfo player1;
	//MainCharacterInfo player2;
	//BossMonster bossMonster;
};
#pragma pack()

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
// ���� �Լ� ���� ���
void err_display(const char* msg);
// ���� �Լ� ���� ���
void err_display(int errcode);

int SendDefaultData(SOCKET client, const SendUpdateData& updateData);