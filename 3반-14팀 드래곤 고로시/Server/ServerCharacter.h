#pragma once
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <Windows.h>
#include <time.h>
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#include <atlimage.h>

#define PI 3.1415926535897

#define IDLEANI 5	//IDLE애니메이션 수
#define RUNANI 16	//RUN 애니메이션 수
#define JUMPANI 8	//JUMP애니메이션 수
#define SHOOTANI 3
#define RUNSHOOTANI 16
#define EXSHOOTANI 13
#define HITANI 6	//피격 애닌메이션 수
#define GHOSTANI 24
#define REVIVEANI 28

#define IDLEWIDTH 50	//IDLE 상태 너비
#define IDLEHEIGHT 75	//IDLE 상태 높이
#define RUNWIDTH 68		//RUN 상태 너비

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

	// 방향 TRUE면 오른쪽 FALSE면 왼쪽
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
	//null = 0, p1 = 1, p2 = 0
	DWORD type = 0;

	// 0이면 IDLE / 1이면 RUN / 2이면 JUMP / 3이면 총쏘는 상태 / 4이면 달리면서 총쏘기 / 5이면 EX어택 / 6이면 피격상태
	MainState state;
	DWORD animationNum;
	MainState oldState;
	DWORD oldAnimationNum;

	RECT Pos;

	// 오른쪽/왼쪽키 누름
	BOOL right, left;
	// 보고있는방향 TRUE면 오른쪽 FALSE면 왼쪽
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
#define BOSSTAILWIDTH 303
#define BOSSTAILHEIGHT 843

#define BOSSIDLEANI 16
#define BOSSDEADANI 40
#define BOSSTAILPREPAREANI 8	// 보스 꼬리공격 준비 애니메이션 수, 실제 꼬리공격의 인덱스이기도 하다
#define BOSSTAILANI 20			// 보스 꼬리공격 애니메이션 수(준비 8 + 꼬리12)

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
	int Idlecount = 0, Idle = 16; //기본 상태 애니메이션
	BOOL ATTACKREADY = FALSE, ATTACK = FALSE;

	BOOL AttackTailReady = FALSE;
	int AttackTailAnimeCount = 0;
	int tailAnimationLimit = BOSSTAILPREPAREANI;
	int AttackTailPreparation = 0; //꼬리 공격 준비하는 시간 300이 되면 꼬리 공격

	BOOL AttackMeteorReady = FALSE;
	int AttackMeteorAnimeCount = 0, Meteor = 11, AttackMeteorPreparation = 0;

	int HP = 100;
	BOOL POFCRASH = FALSE; //플레이어와 충돌 시 TRUE;

	AttackMeteor attackMeteor[METEORNUM];
	AttackFire attackFire[FIRENUM];
};

struct BossBitData
{
	BitmapData ATTACKTAILBitData[20], ATTACKMETEORBitData[8], METEOREXTINCTIONBitData[35], ATTACKFIREBitData[8];
};


///////////////////////////////////////////////////

//메인 케릭터
void CreateMainChar(/*HINSTANCE g_hInst,*/ MainCharacter* mainCharacter);

void MoveMainChar(MainCharacter* mainCharacter, RECT rect);
void JumpMainChar(MainCharacter* mainCharacter, RECT rect);
void HitBoxMainChar(MainCharacter* mainCharacter); // 히트박스 수정

//void MainLoop(RECT& rect, MainCharacter& mainCharacter, BossMonster& Boss, BulletBitmap& bulletBitmap);
void MainLoop(RECT& rect, MainCharacter& mainCharacter, BossMonster& Boss, BulletBitmap& bulletBitmap, char key[8]);
void IdleAndShootStateMainChar(MainCharacter& mainCharacter, BulletBitmap& bulletBitmap);

//총알
void LoadBullet(BulletBitmap* bulletBitmap, HINSTANCE g_hInst);

void CreateBullet(MainCharacter& mainCharacter, const BulletBitmap& bulletBit);
void MoveBullet(MainCharacter& mainCharacter, const RECT& rect);
void DeathBullet(MainCharacter& mainCharacter);

//////////////// ////////////////////////

void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss);
void CreateBossAndStage(BossMonster* Boss, RECT rect);

void BossAttackStateChange(BossMonster* Boss, RECT rect);
void BossAttackLoop(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2);
void BossCollideBullet(RECT rect, RECT hitBox, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2);
void BossCollidePlayer(RECT rect, RECT hitBox, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2);
void FireCollidePlayer(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2);
void MeteorCollidePlayer(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2);
void TailCollidePlayer(RECT rect, RECT hitBox, MainCharacter* player1, MainCharacter* player2);

void CreateBossMeteor(BossMonster& boss, RECT rect);
void CreateBossFire(BossMonster& boss, RECT rect);

void SetBossAndBossAttackRect(BossMonster& boss, const BossBitData& bossBitData);
void BossStateChange(BossMonster* Boss, HWND hwnd, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////




