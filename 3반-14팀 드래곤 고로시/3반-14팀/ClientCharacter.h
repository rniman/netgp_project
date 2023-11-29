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
#include <conio.h>  // _getch 함수를 사용하기 위한 헤더

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
	HBITMAP LOOPBITMAP, DEATHLOOPBITMAP[6];
	HBITMAP EXBITMAP, DEATHEXBITMAP[9];

	BitmapData LOOPBitData, DEATHLOOPBitData[6];
	BitmapData EXBitData, DEATHEXBitData[9];
};

struct MainCharacterInfo
{
	// p1? p2
	DWORD type;

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

struct BossCImage
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

//메인 케릭터
void CreateMainChar(HINSTANCE g_hInst, MainCharacter* mainCharacter);
void DeleteMainChar(MainCharacter* mainCharacter);

void PaintMainCharacter(HDC backMemDC, HDC ObjectDC, DWORD playerNum, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap);
void PaintHeart(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintGhost(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintJump(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintShootMainChar(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintEXShoot(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);
void PaintHIT(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter);

void MoveMainChar(MainCharacter* mainCharacter, RECT rect);
void JumpMainChar(MainCharacter* mainCharacter, RECT rect);
void HitBoxMainChar(MainCharacter* mainCharacter); // 히트박스 수정

//총알
void LoadBullet(BulletBitmap* bulletBitmap, HINSTANCE g_hInst);
void DeleteBitBullet(BulletBitmap* bulletBitmap);
void PaintBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap);
void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, const MainCharacter& mainCharacter, const BulletBitmap& bulletBitmap);

void CreateBullet(MainCharacter& mainCharacter, const BulletBitmap& bulletBit);
void MoveBullet(MainCharacter& mainCharacter, const RECT& rect);
void DeathBullet(MainCharacter& mainCharacter);

//////////////// ////////////////////////

void BossBackground(HDC BackMemDC, RECT rect, CImage BossGround[]);
void BossMob(HDC BackMemDC, BossMonster* Boss, CImage* hBoss);
void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, BossCImage* bossImage, MainState* oldState, int* oldAnimationNum);
void CreateBossAndStage(BossMonster* Boss, BossCImage* bossImage, CImage BossGround[], RECT rect);

void BossAttackStateChange(BossMonster* Boss, RECT rect);
void BossAttackAnimation(HDC BackMemDC, RECT* AttackRect, CImage* Attack);
void BossAttackLoop(RECT rect, BossMonster* Boss, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum);
void PaintBoss(HDC BackMemDC, RECT rect, BossMonster* Boss, BossCImage* bossImage, CImage BossGround[]);
void CreateBossMeteor(BossMonster& boss, RECT rect);
void CreateBossFire(BossMonster& boss, RECT rect);

void BossStateChange(BossMonster* Boss, HWND hwnd, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////




