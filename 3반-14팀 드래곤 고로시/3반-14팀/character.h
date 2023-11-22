#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <time.h>
<<<<<<< Updated upstream:3반-14팀 드래곤 고로시/3반-14팀/character.h
#include <string.h>
#include <tchar.h>
#include <atlimage.h>

#define PI 3.1415926535897 //�߰� 
=======
#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <conio.h>  // _getch �Լ��� ����ϱ� ���� ���

#include <atlimage.h>

#define PI 3.1415926535897
>>>>>>> Stashed changes:3반-14팀 드래곤 고로시/3반-14팀/ClientCharacter.h

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

typedef struct _Character {
	HBITMAP IDLEBITMAP[IDLEANI], RUNBITMAP[RUNANI], JUMPBITMAP[JUMPANI],
		SHOOTBITMAP[SHOOTANI], RUNSHOOTBITMAP[RUNSHOOTANI], EXSHOOTBITMAP[EXSHOOTANI],
		HITBITMAP[HITANI], GHOSTBITMAP[GHOSTANI], REVIVEBITMAP[REVIVEANI];

	BITMAP  IDLEBitData[IDLEANI], RUNBitData[RUNANI], JUMPBitData[JUMPANI],
		SHOOTBitData[SHOOTANI], RUNSHOOTBitData[RUNSHOOTANI], EXSHOOTBitData[EXSHOOTANI],
		HITBitData[HITANI], GHOSTBitData[GHOSTANI], REVIVEBitData[REVIVEANI];
	// 0�̸� IDLE / 1�̸� RUN / 2�̸� JUMP / 3�̸� �ѽ�� ���� / 4�̸� �޸��鼭 �ѽ�� / 5�̸� EX���� / 6�̸� �ǰݻ���
	int state;
	int animationNum;
	RECT Pos;
	// ������/����Ű ����
	BOOL right, left;
	// �����ִ¹��� TRUE�� ������ FALSE�� ����
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
	// ���� TRUE�� ������ FALSE�� ����
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
	int Idlecount = 0, Idle = 16; //�⺻ ���� �ִϸ��̼�
	BOOL ATTACKREADY = FALSE, ATTACK = FALSE;

	CImage BossIDLE[16];
	CImage BossAttackReady[8];
	CImage BossAttack[16];
	CImage BossAttackEnd[8];

	BOOL AttackTailReady = FALSE;
	int AttackTailAnimeCount = 0, Tail = 8, AttackTailPreparation = 0; //���� ���� �غ��ϴ� �ð� 300�� �Ǹ� ���� ����
	CImage AttackTail[20];

	BOOL AttackMeteorReady = FALSE;
	int AttackMeteorAnimeCount = 0, Meteor = 11, AttackMeteorPreparation = 0;
	CImage AttackMeteor[19];
	CImage MeteorExtinction[35];
	CImage AttackFire[8];

	int HP = 100;
	BOOL POFCRASH = FALSE; //�÷��̾�� �浹 �� TRUE;
	CImage Death[41];
}BossMonster;

typedef struct STAGE4SPHERE {
	RECT rect;
	int Idle = 8, Idlecount = 0, dis = 0, ySum = (rand() % 4) + 3;
	BOOL extinction = FALSE;
	STAGE4SPHERE* next = NULL;
}STAGE4SPHERE;

///////////////////////////////////////////////////

//���� �ɸ���
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
void HitBoxMainChar(MainCharacter* mainCharacter); // ��Ʈ�ڽ� ����

//�Ѿ�
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

