#pragma once
#include "ClientCharacter.h"

struct ThreadParams
{
	SOCKET socket;
	HANDLE hThread;
};

#pragma pack(1)
struct RecvUpdateData
{
	MainCharacterInfo player1, player2;
	//BossMonster bossMonster;
};
#pragma pack()

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);
// 소켓 함수 오류 출력
void err_display(const char* msg);
// 소켓 함수 오류 출력
void err_display(int errcode);

int SendInputData(SOCKET remote, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss);
int RecvDefaultData(SOCKET client, MainCharacter& p1Update/*, MainCharacter& p2Update*/, BossMonster& boss);

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg);
