#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <time.h>
<<<<<<< Updated upstream:3ë°˜-14íŒ€ ë“œëž˜ê³¤ ê³ ë¡œì‹œ/3ë°˜-14íŒ€/character.h
#include <string.h>
#include <tchar.h>
#include <atlimage.h>

#define PI 3.1415926535897 //Ãß°¡ 
=======
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <conio.h>  // _getch ÇÔ¼ö¸¦ »ç¿ëÇÏ±â À§ÇÑ Çì´õ

#include <atlimage.h>

#define PI 3.1415926535897
>>>>>>> Stashed changes:3ë°˜-14íŒ€ ë“œëž˜ê³¤ ê³ ë¡œì‹œ/3ë°˜-14íŒ€/ClientCharacter.h

#define IDLEANI 5	//IDLE¾Ö´Ï¸ÞÀÌ¼Ç ¼ö
#define RUNANI 16	//RUN ¾Ö´Ï¸ÞÀÌ¼Ç ¼ö
#define JUMPANI 8	//JUMP¾Ö´Ï¸ÞÀÌ¼Ç ¼ö
#define SHOOTANI 3
#define RUNSHOOTANI 16
#define EXSHOOTANI 13
#define HITANI 6	//ÇÇ°Ý ¾Ö´Ñ¸ÞÀÌ¼Ç ¼ö
#define GHOSTANI 24
#define REVIVEANI 28

#define IDLEWIDTH 50	//IDLE »óÅÂ ³Êºñ
#define IDLEHEIGHT 75	//IDLE »óÅÂ ³ôÀÌ
#define RUNWIDTH 68		//RUN »óÅÂ ³Êºñ

#define BULWIDTH 32
#define BULHEIGHT 16
#define EXBULWIDTH  288/2
#define EXBULHEIGHT 157/2

#define STAGEFLOOR 500

typedef struct _Character {
	HBITMAP IDLEBITMAP[IDLEANI], RUNBITMAP[RUNANI], JUMPBITMAP[JUMPANI],
		SHOOTBITMAP[SHOOTANI], RUNSHOOTBITMAP[RUNSHOOTANI], EXSHOOTBITMAP[EXSHOOTANI],
		HITBITMAP[HITANI], GHOSTBITMAP[GHOSTANI], REVIVEBITMAP[REVIVEANI];

	BITMAP  IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];
	// 0ÀÌ¸é IDLE / 1ÀÌ¸é RUN / 2ÀÌ¸é JUMP / 3ÀÌ¸é ÃÑ½î´Â »óÅÂ / 4ÀÌ¸é ´Þ¸®¸é¼­ ÃÑ½î±â / 5ÀÌ¸é EX¾îÅÃ / 6ÀÌ¸é ÇÇ°Ý»óÅÂ
	int state;
	int animationNum;
	RECT Pos;
	// ¿À¸¥ÂÊ/¿ÞÂÊÅ° ´©¸§
	BOOL right, left;
	// º¸°íÀÖ´Â¹æÇâ TRUE¸é ¿À¸¥ÂÊ FALSE¸é ¿ÞÂÊ
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
	// ¹æÇâ TRUE¸é ¿À¸¥ÂÊ FALSE¸é ¿ÞÂÊ
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
	int Idlecount = 0, Idle = 16; //±âº» »óÅÂ ¾Ö´Ï¸ÞÀÌ¼Ç
	BOOL ATTACKREADY = FALSE, ATTACK = FALSE;

	CImage BossIDLE[16];
	CImage BossAttackReady[8];
	CImage BossAttack[16];
	CImage BossAttackEnd[8];

	BOOL AttackTailReady = FALSE;
	int AttackTailAnimeCount = 0, Tail = 8, AttackTailPreparation = 0; //²¿¸® °ø°Ý ÁØºñÇÏ´Â ½Ã°£ 300ÀÌ µÇ¸é ²¿¸® °ø°Ý
	CImage AttackTail[20];

	BOOL AttackMeteorReady = FALSE;
	int AttackMeteorAnimeCount = 0, Meteor = 11, AttackMeteorPreparation = 0;
	CImage AttackMeteor[19];
	CImage MeteorExtinction[35];
	CImage AttackFire[8];

	int HP = 100;
	BOOL POFCRASH = FALSE; //ÇÃ·¹ÀÌ¾î¿Í Ãæµ¹ ½Ã TRUE;
	CImage Death[41];
}BossMonster;

typedef struct STAGE4SPHERE {
	RECT rect;
	int Idle = 8, Idlecount = 0, dis = 0, ySum = (rand() % 4) + 3;
	BOOL extinction = FALSE;
	STAGE4SPHERE* next = NULL;
}STAGE4SPHERE;

///////////////////////////////////////////////////

//¸ÞÀÎ ÄÉ¸¯ÅÍ
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
void HitBoxMainChar(MainCharacter* mainCharacter); // È÷Æ®¹Ú½º ¼öÁ¤

//ÃÑ¾Ë
void LoadBullet(BULLETBITMAP* bulletBitmap, HINSTANCE g_hInst);
void DeleteBitBullet(BULLETBITMAP* bulletBitmap);
void PaintBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);
void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap);

BLinkedList* CreateBList(void);
void CreateBullet(BLinkedList* bullet, MainCharacter mainCharacter, BULLETBITMAP bulletBit);
void DeleteBullet(BLinkedList* bullet);
void MoveBullet(BLinkedList* bullet, RECT rect);
void DeathBullet(BLinkedList* bullet);

//////////////// ////////////////////////

void MeteorStackDelete(STAGE4SPHERE* head, STAGE4SPHERE* target);

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

