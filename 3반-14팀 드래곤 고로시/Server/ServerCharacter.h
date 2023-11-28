#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ
#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <Windows.h>
#include <time.h>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <atlimage.h>

#define PI 3.1415926535897 //�߰� 

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

struct BulletBitmap
{
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

	WORD jumpTime = 0, coolTime = 0, invincibleTime = 0, responeTime = 0;
};

struct MainCharacterBitmap
{
	BitmapData IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];
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

struct AttackMeteor
{
	BOOL exist = FALSE;
	BOOL extinction = FALSE;
	RECT rect;
	DWORD animationNum = 0;
	DWORD yStart = (rand() % 4) + 3;
	int angle = 0;
};

struct AttackFire
{
	BOOL exist = FALSE;
	RECT rect;
	DWORD animationNum = 0;
};

struct BossMonster
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

	AttackMeteor attackMeteor[METEORNUM];
	AttackFire attackFire[FIRENUM];
};

struct BossBitData
{
	BitmapData ATTACKTAILBitData[20], ATTACKMETEORBitData[8], METEOREXTINCTIONBitData[35], ATTACKFIREBitData[8];
};

#define BOSSTAILWIDTH 303
#define BOSSTAILHEIGHT 843

///////////////////////////////////////////////////

//���� �ɸ���
void CreateMainChar(/*HINSTANCE g_hInst,*/ MainCharacter* mainCharacter);

void MoveMainChar(MainCharacter* mainCharacter, RECT rect);
void JumpMainChar(MainCharacter* mainCharacter, RECT rect);
void HitBoxMainChar(MainCharacter* mainCharacter); // ��Ʈ�ڽ� ����

//�Ѿ�
void LoadBullet(BulletBitmap* bulletBitmap, HINSTANCE g_hInst);

void CreateBullet(MainCharacter& mainCharacter, const BulletBitmap& bulletBit);
void MoveBullet(MainCharacter& mainCharacter, const RECT& rect);
void DeathBullet(MainCharacter& mainCharacter);

//////////////// ////////////////////////

void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, MainState* oldState, int* oldAnimationNum);
void CreateBossAndStage(BossMonster* Boss, RECT rect);

void BossAttackStateChange(BossMonster* Boss, RECT rect);
void BossAttackMeteor(RECT rect, BossMonster* Boss, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum);
void CreateBossMeteor(BossMonster& boss, RECT rect);
void CreateBossFire(BossMonster& boss, RECT rect);

void SetBossAndBossAttackRect(BossMonster& boss, const BossBitData& bossBitData);
void BossStateChange(BossMonster* Boss, HWND hwnd, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////
