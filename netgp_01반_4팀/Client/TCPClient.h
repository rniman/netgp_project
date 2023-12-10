#pragma once
#include "ClientCharacter.h"

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
struct RecvUpdateData
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

// IP �Է� dialog
INT_PTR CALLBACK IpDialogProc(HWND, UINT, WPARAM, LPARAM);

int RecvInitData(SOCKET remote, MainCharacter& p1Update, MainCharacter& p2Update, BossMonster& boss);
void SetSendBitmapData(SendBitData& sendBitData, const MainCharacterBitmap& maincharBitData, const BulletBitmap& bulletBitData, const BossCImage& bossBitData);
int SendInitBitmapData(SOCKET remote, const MainCharacterBitmap& maincharBitData, const BulletBitmap& bulletBitData, const BossCImage& bossBitData);
int SendInputData(SOCKET remote, const char* keyBuffer);
int RecvDefaultData(SOCKET remote, MainCharacter& p1Update, MainCharacter& p2Update, BossMonster& boss);

void ConvertLPWSTRToChar(LPWSTR lpwstr, char* dest, int destSize);

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg);
