#pragma once
#include "ServerCharacter.h"

#define KEYBUFSIZE    8

struct ThreadParams
{
	SOCKET socket;
	HANDLE hThread;
};

#pragma pack(1)
struct SendBitData
{
	BitmapData IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];

	BitmapData LOOPBitData, DEATHLOOPBitData[6];
	BitmapData EXBitData, DEATHEXBitData[9];
	BitmapData ATTACKTAILBitData[20], ATTACKMETEORBitData[8], METEOREXTINCTIONBitData[35], ATTACKFIREBitData[8];
};
#pragma pack()

#pragma pack(1)
struct SendUpdateData
{
	MainCharacterInfo player1;
	MainCharacterInfo player2;
	BossMonster bossMonster;
};
#pragma pack()

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
// ���� �Լ� ���� ���
void err_display(const char* msg);
// ���� �Լ� ���� ���
void err_display(int errcode);

int SendInitData(SOCKET client, DWORD playerNum, const SendUpdateData& sendData);
void SetInitBitData(SendBitData& sendBitData, MainCharacterBitmap& maincharBitData, BulletBitmap& bulletBitData, BossBitData& bossBitData);
void SetSecPlayerInitBitData(const MainCharacterBitmap& player1BitData, MainCharacterBitmap& player2BitData);
int RecvInitBitmapData(SOCKET client, MainCharacterBitmap& maincharBitData, BulletBitmap& bulletBitData, BossBitData& bossBitData);
int SendDefaultData(SOCKET client, const SendUpdateData& sendData);

DWORD WINAPI NetworkThread(LPVOID arg);
DWORD WINAPI UpdateThread(LPVOID arg);