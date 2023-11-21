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

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);
// 소켓 함수 오류 출력
void err_display(const char* msg);
// 소켓 함수 오류 출력
void err_display(int errcode);

int SendDefaultData(SOCKET client, const SendUpdateData& updateData);