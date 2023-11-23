#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <Windows.h>
#include <time.h>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <atlimage.h>


#define PI 3.1415926535897

#define IDLEANI 5	//IDLE�ִϸ��̼� ��
#define RUNANI 16	//RUN �ִϸ��̼� ��
#define JUMPANI 8	//JUMP�ִϸ��̼� ��
#define SHOOTANI 3
#define RUNSHOOTANI 16
#define EXSHOOTANI 13
#define HITANI 6	//�ǰ� �ִѸ��̼� ��
#define GHOSTANI 24
#define REVIVEANI 28

#define IDLEWIDTH 50	//IDLE ���� �ʺ�
#define IDLEHEIGHT 75	//IDLE ���� ����
#define RUNWIDTH 68		//RUN ���� �ʺ�

#define BULWIDTH 32
#define BULHEIGHT 16
#define EXBULWIDTH  288/2
#define EXBULHEIGHT 157/2

#define STAGEFLOOR 500

#define BULLETNUM 10

enum class MainState
{
	IDLE = 0,
	RUN,
	JUMP,
	SHOOT,
	RUNSHOOT,
	EXSHOOT,
	HIT
};

struct BitmapData // bmWidth, bmHeight
{
	LONG bmWidth;
	LONG bmHeight;
};

struct Bullet
{
	BOOL exist;
	BOOL EX;
	
	// ���� TRUE�� ������ FALSE�� ����
	BOOL direction;
	
	int animationNum;
	int deathTime;
	
	RECT Pos;
	RECT hitBox;
};

struct BULLETBITMAP
{
	HBITMAP LOOPBITMAP, DEATHLOOPBITMAP[6];
	HBITMAP EXBITMAP, DEATHEXBITMAP[9];

	BitmapData LOOPBitData, DEATHLOOPBitData[6];
	BitmapData EXBitData, DEATHEXBitData[9];
};

struct MainCharacterInfo
{
	// p1? p2
	DWORD type;

	// 0�̸� IDLE / 1�̸� RUN / 2�̸� JUMP / 3�̸� �ѽ�� ���� / 4�̸� �޸��鼭 �ѽ�� / 5�̸� EX���� / 6�̸� �ǰݻ���
	MainState state;
	DWORD animationNum;
	RECT Pos;

	// ������/����Ű ����
	BOOL right, left;
	// �����ִ¹��� TRUE�� ������ FALSE�� ����
	BOOL direction;

	WORD heart;
	WORD energy;

	Bullet bullet[BULLETNUM];
};

struct MainCharacterBitmap
{
	HBITMAP IDLEBITMAP[IDLEANI], RUNBITMAP[RUNANI], JUMPBITMAP[JUMPANI],
		SHOOTBITMAP[SHOOTANI], RUNSHOOTBITMAP[RUNSHOOTANI], EXSHOOTBITMAP[EXSHOOTANI],
		HITBITMAP[HITANI], GHOSTBITMAP[GHOSTANI], REVIVEBITMAP[REVIVEANI];

	BitmapData IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];

	HBITMAP HEARTBITMAP[3];
};

struct MainCharacter
{
	MainCharacterInfo info;
	MainCharacterBitmap bitmap;
};

//////////////////////////////////////////////////

#define BOSSIDLEANI 16
#define BOSSDEADANI 40
#define BOSSTAILPREPAREANI 8	// ���� �������� �غ� �ִϸ��̼� ��, ���� ���������� �ε����̱⵵ �ϴ�
#define BOSSTAILANI 20			// ���� �������� �ִϸ��̼� ��(�غ� 8 + ����12)

#define METEORANI 8
#define METEORANISTART 11
#define METEOREXTINCTIONANI 35
#define FIREANI 8

#define FIRENUM 5
#define METEORNUM 5

struct ATTACKMETEOR
{
	BOOL exist = FALSE;
	BOOL extinction = FALSE;
	RECT rect;
	DWORD animationNum = 0;
	DWORD yStart = (rand() % 4) + 3;
	int angle = 0;
};

struct ATTACKFIRE
{
	BOOL exist = FALSE;
	RECT rect;
	DWORD animationNum = 0;
};

typedef struct BOSSOBJECT 
{
	RECT rect, AttackTailrect, AttackMeteorect[3];
	int Idlecount = 0, Idle = 16; //�⺻ ���� �ִϸ��̼�
	BOOL ATTACKREADY = FALSE, ATTACK = FALSE;

	BOOL AttackTailReady = FALSE;
	int AttackTailAnimeCount = 0;
	int tailAnimationLimit = BOSSTAILPREPAREANI;
	int AttackTailPreparation = 0; //���� ���� �غ��ϴ� �ð� 300�� �Ǹ� ���� ����

	BOOL AttackMeteorReady = FALSE;
	int AttackMeteorAnimeCount = 0, Meteor = 11, AttackMeteorPreparation = 0;

	int HP = 100;
	BOOL POFCRASH = FALSE; //�÷��̾�� �浹 �� TRUE;

	ATTACKMETEOR attackMeteor[METEORNUM];
	ATTACKFIRE attackFire[FIRENUM];
}BossMonster;

struct BOSSCIMAGE
{
	CImage BossIDLE[16];
	CImage BossAttackReady[8];
	CImage BossAttack[16];
	CImage BossAttackEnd[8];

	CImage AttackTail[20];

	CImage AttackMeteor[19];
	CImage MeteorExtinction[35];
	CImage AttackFire[8];

	CImage Death[BOSSDEADANI + 1];
};
///////////////////////////////////////////////////

//���� �ɸ���
void CreateMainChar(HINSTANCE g_hInst, MainCharacter* mainCharacter);
void DeleteMainChar(MainCharacter* mainCharacter);

void PaintHeart(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintGhost(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, int responeTime);
void PaintMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintJump(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintShootMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintEXShoot(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintHIT(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);

void MoveMainChar(MainCharacter* mainCharacter, RECT rect);
void JumpMainChar(MainCharacter* mainCharacter, int jumpTime, RECT rect);
void HitBoxMainChar(MainCharacter* mainCharacter); // ��Ʈ�ڽ� ����

//�Ѿ�
void LoadBullet(BULLETBITMAP* bulletBitmap, HINSTANCE g_hInst);
void DeleteBitBullet(BULLETBITMAP* bulletBitmap);
void PaintBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BULLETBITMAP& bulletBitmap);
void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BULLETBITMAP& bulletBitmap);

void CreateBullet(MainCharacter& mainCharacter, const BULLETBITMAP& bulletBit);
void MoveBullet(MainCharacter& mainCharacter, const RECT& rect);
void DeathBullet(MainCharacter& mainCharacter);

//////////////// ////////////////////////

void BossBackground(HDC BackMemDC, RECT rect, CImage BossGround[]);
void BossMob(HDC BackMemDC, BossMonster* Boss, CImage* hBoss);
void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, BOSSCIMAGE* bossImage, MainState* oldState, int* oldAnimationNum, int* invincibleTime);
void CREATESTAGE5(BossMonster* Boss, BOSSCIMAGE* bossImage, CImage BossGround[], RECT rect);

void BossAttackStateChange(BossMonster* Boss, RECT rect);
void BossAttackAnimation(HDC BackMemDC, RECT* AttackRect, CImage* Attack);
void BossAttackMeteor(RECT rect, BossMonster* Boss, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum, int* invincibleTime);
void PaintBoss(HDC BackMemDC, RECT rect, BossMonster* Boss, BOSSCIMAGE* bossImage, CImage BossGround[]);
void CreateBossMeteor(BossMonster& boss, RECT rect);
void CreateBossFire(BossMonster& boss, RECT rect);

void BossStateChange(BossMonster* Boss, HWND hwnd, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////




