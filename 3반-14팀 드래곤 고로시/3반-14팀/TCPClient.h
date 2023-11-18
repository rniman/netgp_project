#pragma once
#include "character.h"

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

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
// ���� �Լ� ���� ���
void err_display(const char* msg);
// ���� �Լ� ���� ���
void err_display(int errcode);

int RecvDefaultData(SOCKET client, MainCharacter& p1Update, MainCharacter& p2Update, BossMonster& boss);
// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg);
