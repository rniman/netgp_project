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

#define POTALANI 4

#define NPCANI 11	//NPC 애니메이션 수
#define NPCWIDTH 83
#define NPCHEIGT 135

#define IDLEWIDTH 50	//IDLE 상태 너비
#define IDLEHEIGHT 75	//IDLE 상태 높이
#define RUNWIDTH 68		//RUN 상태 너비

#define BULWIDTH 32
#define BULHEIGHT 16
#define EXBULWIDTH  288/2
#define EXBULHEIGHT 157/2


#define STAGE1FLOOR 500	//STAGE1 바닥위치

typedef struct NPC {
	// 0 ~ 4 대화 애니메이션
	// 5 ~ 10 평상시 애니메이션
	HBITMAP NPCBitmap[NPCANI];
	BITMAP NPCBitData[NPCANI];
	int animationNum;
	RECT Pos;
	
	HBITMAP textBox;//600 181
}NPC;

typedef struct village {
	HBITMAP sky;
	HBITMAP land;
	HBITMAP house;
	BITMAP skyData, landData, houseData;
}VILLAGE;

typedef struct cloud {
	HBITMAP cloudBITMAP[2];
	BITMAP cloudData[2];
}CLOUD;

typedef struct _Character {
	HBITMAP IDLEBITMAP[IDLEANI], RUNBITMAP[RUNANI], JUMPBITMAP[JUMPANI],
		SHOOTBITMAP[SHOOTANI], RUNSHOOTBITMAP[RUNSHOOTANI], EXSHOOTBITMAP[EXSHOOTANI],
		HITBITMAP[HITANI], GHOSTBITMAP[GHOSTANI], REVIVEBITMAP[REVIVEANI];

	BITMAP  IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];
	// 0이면 IDLE / 1이면 RUN / 2이면 JUMP / 3이면 총쏘는 상태 / 4이면 달리면서 총쏘기 / 5이면 EX어택 / 6이면 피격상태
	int state;
	int animationNum;
	RECT Pos;
	// 오른쪽/왼쪽키 누름
	BOOL right, left;
	// 보고있는방향 TRUE면 오른쪽 FALSE면 왼쪽
	BOOL direction;


	HBITMAP HEARTBITMAP[3];
	int heart;
	int energy;
}MainCharacter;

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

typedef struct MONSTEROBJECT {
	int Idlecount = 0, Idle = 10;

	BOOL LEFT = TRUE, MOVE = FALSE, ALIVE = TRUE, Explode = FALSE;
	int Move = 4, Movecount = 0, explosion = 9, Hp = 5;
	RECT rect;
}MonsterStage2;

typedef struct MONSTEROBJECT2 {
	int Idlecount = 0, Idle = 18;

	BOOL ALIVE = TRUE, ATTACK = FALSE;
	int MOVE = 0, oldMove = 0;
	RECT rect;
}MonsterStage3;

typedef struct STAGE3MONSTERATTACK {
	RECT rect;
	int Idle = 20, Idlecount = 0, dis = 0;
	POINT target, createPt;
	STAGE3MONSTERATTACK* next = NULL;
}FireAttack;

typedef struct STAGE4SPHERE {
	RECT rect;
	int Idle = 8, Idlecount = 0, dis = 0, ySum = (rand() % 4) + 3;
	BOOL extinction = FALSE;
	STAGE4SPHERE* next = NULL;
}STAGE4SPHERE;

typedef struct STAGE4EVENT {
	CImage STAGE4ClearTarget[1];
	RECT rect;
	int Stage4Sec = 0;//스테이지4 시간 초
	BOOL STAGE4CLEAR = FALSE, HIT = FALSE;
}STAGE4EVENT;

typedef struct MONSTERIMAGE { //몬스터 이미지 저장 구조체
	CImage Stage2MonsterIDLE[10];
	CImage Stage2MonsterLeftMove[4], Stage2MonsterRightMove[4];
	CImage Stage2MonsterExplode[9];

	CImage Stage3MonsterIDLE[18];
	CImage Stage3MonsterAttack[25];
	CImage Stage3MonsterFireAttack[20];
	CImage Stage3MonsterDead[20];

	CImage Stage4Meteor[8];
	CImage Stage4MeteorExtinction[35];
	CImage Stage4MonsterLeftMove[4];
	CImage Stage4MonsterExplode[9];
}ImageOfMonster;

///////////////////////////////////////////////////

//메인 케릭터
void CreateMainChar(HINSTANCE g_hInst, MainCharacter* mainCharacter);
void DeleteMainChar(MainCharacter* mainCharacter);

void PaintHeart(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);
void PaintGhost(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter, int responeTime);
void PaintMainChar(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);
void PaintJump(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);
void PaintShootMainChar(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);
void PaintEXShoot(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);
void PaintHIT(HDC backMemDC, HDC ObjectDC, MainCharacter mainCharacter);

void MoveMainChar(MainCharacter* mainCharacter, RECT rect);
void JumpMainChar(MainCharacter* mainCharacter, int jumpTime, RECT rect);
void HitBoxMainChar(MainCharacter* mainCharacter); // 히트박스 수정

//총알
void LoadBullet(BULLETBITMAP* bulletBitmap, HINSTANCE g_hInst);
void DeleteBitBullet(BULLETBITMAP* bulletBitmap);
void PaintBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);
void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);

BLinkedList* CreateBList(void);
void CreateBullet(BLinkedList* bullet, MainCharacter mainCharacter, BULLETBITMAP bulletBit);
void DeleteBullet(BLinkedList* bullet);
void MoveBullet(BLinkedList* bullet, RECT rect);
void DeathBullet(BLinkedList* bullet);
void CheckStage2(BLinkedList* bullet, MonsterStage2 Mob[]);
void CheckStage4(BLinkedList* bullet, STAGE4SPHERE* MeteorHead, STAGE4EVENT* Stage4Event, BOOL* potalOn);

//스테이지 1 배경
void LoadStage1(HINSTANCE g_hInst, VILLAGE* village, CLOUD* cloud);
void LoadPotal(CImage potal[]);
void PaintStage1(HINSTANCE g_hInst, HDC backMemDC, HDC ObjectDC, VILLAGE village, CLOUD cloud, RECT rect);
void PaintPotal(HDC backMemDC, CImage* potal);
void DeleteStage1(VILLAGE* village, CLOUD* cloud, CImage potal[]);
BOOL CheckPotal(MainCharacter mainCharacter);

//NPC
void CreateNPC(HINSTANCE g_hInst, NPC* npc);
void InitMainChar(MainCharacter* mainCharacter);
void PaintNPC(HDC backMemDC, HDC ObjectDC, NPC* npc);
void DeleteNPC(NPC* npc);
BOOL CheckNPCDlg(MainCharacter mainCharacter, NPC* npc, int* dlgTime);


//////////////// ////////////////////////


void BossBackground(HDC BackMemDC, RECT rect, CImage BossGround[]);
void BossMob(HDC BackMemDC, BossMonster* Boss, CImage* hBoss);
void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, int* oldState, int* oldAnimationNum, int* invincibleTime);
void CREATESTAGE5(BossMonster* Boss, CImage BossGround[], RECT rect);
void BossAttackStateChange(BossMonster* Boss, RECT rect, STAGE4SPHERE* head);
void BossAttackAnimation(HDC BackMemDC, RECT* AttackRect, CImage* Attack);
//void BossAttackMeteor(RECT rect, BossMonster* Boss, STAGE4SPHERE* head, RECT PlayerRECT);
void BossAttackMeteor(RECT rect, BossMonster* Boss, STAGE4SPHERE* head, BLinkedList* bullet, MainCharacter* mainCharacter,
	int* oldState, int* oldAnimationNum, int* invincibleTime);
void STAGE5(HDC BackMemDC, RECT rect, BossMonster* Boss, CImage BossGround[], STAGE4SPHERE* head);
void BossMeteorStackInsert(STAGE4SPHERE* head, RECT rect, int num);
void BossStateChange(BossMonster* Boss, HWND hwnd, STAGE4SPHERE* BossMeteorHead, RECT rect);

double getradian(int num);
////////////////////////////////////////////////////////////

void MonsterIdleAnimation(HDC BackMemDC, MonsterStage2* Mob, CImage* hMonster);
void MonsterExplodeAnimation(HDC BackMemDC, MonsterStage2* Mob, CImage* hMonster);
void CREATESTAGE2(CImage STAGE3BackGround[], MONSTERIMAGE* IMob, MonsterStage2 Mob[], RECT rect);
void STAGE2(HDC BackMemDC, RECT rect, CImage STAGE3BackGround[], MonsterStage2 Mob[], ImageOfMonster* IMob);
void Stage2_MonsterStateChange(MonsterStage2 Mob[], const RECT rect, const RECT player);


void MonsterIdleAnimation2(HDC BackMemDC, MonsterStage3* Mob, CImage* hMonster);
void MonsterFireAttackAnimation(HDC BackMemDC, FireAttack* FireAttack, CImage* hMonster);
void StackInsert(FireAttack* head, RECT rect, RECT Prect);
void StackDelete(FireAttack* head, FireAttack* target);
void PlayerStage3Mobcrash(MonsterStage3 MobStage3[], BLinkedList* bullet);
void Stage3_TimeCrashEvent(MonsterStage3 MobStage3[], FireAttack* FireAttackHead, MainCharacter* mainCharacter,
	int* oldState, int* oldAnimationNum, int* invincibleTime);
void Stage3_MonsterStateChange(MonsterStage3 MobStage3[], FireAttack* FireAttackHead, RECT PlayerRect, RECT ProgramRect);
void Stage3(HDC BackMemDC, RECT rect, CImage* STAGE3BackGround, MonsterStage3 MobStage3[], ImageOfMonster* IMob, FireAttack* FireAttackHead);
void CREATESTAGE3(CImage* STAGE3BackGround, MONSTERIMAGE* IMob, MonsterStage3 MobStage3[], RECT rect);

void MeteorStackInsert(STAGE4SPHERE* head, RECT rect);
void MeteorStackDelete(STAGE4SPHERE* head, STAGE4SPHERE* target);
void STAGE4MeteorAnimation(HDC BackMemDC, RECT* MeteorRect, CImage* Meteor);
void STAGE4MonsterExplodeAnimation(HDC BackMemDC, RECT* mob, CImage* hMonster);
void Stage4_StateChange(STAGE4SPHERE* MeteorHead);
void Stage4_OBJECTMOVE(STAGE4SPHERE* MeteorHead, MainCharacter* mainCharacter, RECT rect, STAGE4EVENT* Stage4Event,
	int* oldState, int* oldAnimationNum, int* invincibleTime);
void STAGE4(HDC BackMemDC, RECT rect, CImage* STAGE4BackGround, STAGE4SPHERE* MeteorHead, ImageOfMonster* IMob, STAGE4EVENT* Stage4Event);
void CREATESTAGE4(RECT rect, ImageOfMonster* IMob, CImage* STAGE4BackGround, STAGE4EVENT* Stage4Event);
