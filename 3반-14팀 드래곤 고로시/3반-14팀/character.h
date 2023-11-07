#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <tchar.h>
#include <atlimage.h>

#define PI 3.1415926535897 //추가 

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

struct MainCharacterInfo
{
	// p1? p2
	DWORD type;

	// 0이면 IDLE / 1이면 RUN / 2이면 JUMP / 3이면 총쏘는 상태 / 4이면 달리면서 총쏘기 / 5이면 EX어택 / 6이면 피격상태
	MainState state;
	DWORD animationNum;
	RECT Pos;

	// 오른쪽/왼쪽키 누름
	BOOL right, left;
	// 보고있는방향 TRUE면 오른쪽 FALSE면 왼쪽
	BOOL direction;

	DWORD heart;
	DWORD energy;
};

struct MainCharacterBitmap
{
	HBITMAP IDLEBITMAP[IDLEANI], RUNBITMAP[RUNANI], JUMPBITMAP[JUMPANI],
		SHOOTBITMAP[SHOOTANI], RUNSHOOTBITMAP[RUNSHOOTANI], EXSHOOTBITMAP[EXSHOOTANI],
		HITBITMAP[HITANI], GHOSTBITMAP[GHOSTANI], REVIVEBITMAP[REVIVEANI];

	BITMAP  IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];

	HBITMAP HEARTBITMAP[3];
};

struct MainCharacter
{
	MainCharacterInfo info;
	MainCharacterBitmap bitmap;
};


typedef struct BULLETBITMAP {
	HBITMAP LOOPBITMAP, DEATHLOOPBITMAP[6];
	BITMAP LOOPBitData, DEATHLOOPBitData[6];
	HBITMAP EXBITMAP, DEATHEXBITMAP[9];
	BITMAP EXBitData, DEATHEXBitData[9];
}BULLETBITMAP;

typedef struct BULLET {
	BOOL EX;
	int animationNum;
	RECT Pos;
	RECT hitBox;
	// 방향 TRUE면 오른쪽 FALSE면 왼쪽
	BOOL direction;
	BOOL exist;
	int deathTime;
}BULLET;

typedef struct BULLETNODE {
	BULLET data;
	BULLETNODE* link;
}BULLETNODE;

typedef struct BLinkedList {
	BULLETNODE* head;
}BLinkedList;

//////////////////////////////////////////////////

typedef struct BOSSOBJECT {
	RECT rect, AttackTailrect, AttackMeteorect[3];
	int Idlecount = 0, Idle = 16; //기본 상태 애니메이션
	BOOL ATTACKREADY = FALSE, ATTACK = FALSE;

	CImage BossIDLE[16];
	CImage BossAttackReady[8];
	CImage BossAttack[16];
	CImage BossAttackEnd[8];

	BOOL AttackTailReady = FALSE;
	int AttackTailAnimeCount = 0, Tail = 8, AttackTailPreparation = 0; //꼬리 공격 준비하는 시간 300이 되면 꼬리 공격
	CImage AttackTail[20];

	BOOL AttackMeteorReady = FALSE;
	int AttackMeteorAnimeCount = 0, Meteor = 11, AttackMeteorPreparation = 0;
	CImage AttackMeteor[19];
	CImage MeteorExtinction[35];
	CImage AttackFire[8];

	int HP = 100;
	BOOL POFCRASH = FALSE; //플레이어와 충돌 시 TRUE;
	CImage Death[41];
}BossMonster;

typedef struct STAGE4SPHERE {
	RECT rect;
	int Idle = 8, Idlecount = 0, dis = 0, ySum = (rand() % 4) + 3;
	BOOL extinction = FALSE;
	STAGE4SPHERE* next = NULL;
}STAGE4SPHERE;

///////////////////////////////////////////////////

//메인 케릭터
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
void HitBoxMainChar(MainCharacter* mainCharacter); // 히트박스 수정

//총알
void LoadBullet(BULLETBITMAP* bulletBitmap, HINSTANCE g_hInst);
void DeleteBitBullet(BULLETBITMAP* bulletBitmap);
void PaintBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);
void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);

BLinkedList* CreateBList(void);
void CreateBullet(BLinkedList* bullet, const MainCharacter& mainCharacter, BULLETBITMAP bulletBit);
void DeleteBullet(BLinkedList* bullet);
void MoveBullet(BLinkedList* bullet, RECT rect);
void DeathBullet(BLinkedList* bullet);

//////////////// ////////////////////////

void MeteorStackDelete(STAGE4SPHERE* head, STAGE4SPHERE* target);

void BossBackground(HDC BackMemDC, RECT rect, CImage BossGround[]);
void BossMob(HDC BackMemDC, BossMonster* Boss, CImage* hBoss);
void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, MainState* oldState, int* oldAnimationNum, int* invincibleTime);
void CREATESTAGE5(BossMonster* Boss, CImage BossGround[], RECT rect);
void BossAttackStateChange(BossMonster* Boss, RECT rect, STAGE4SPHERE* head);
void BossAttackAnimation(HDC BackMemDC, RECT* AttackRect, CImage* Attack);
//void BossAttackMeteor(RECT rect, BossMonster* Boss, STAGE4SPHERE* head, RECT PlayerRECT);
void BossAttackMeteor(RECT rect, BossMonster* Boss, STAGE4SPHERE* head, BLinkedList* bullet, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum, int* invincibleTime);
void STAGE5(HDC BackMemDC, RECT rect, BossMonster* Boss, CImage BossGround[], STAGE4SPHERE* head);
void BossMeteorStackInsert(STAGE4SPHERE* head, RECT rect, int num);
void BossStateChange(BossMonster* Boss, HWND hwnd, STAGE4SPHERE* BossMeteorHead, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////

