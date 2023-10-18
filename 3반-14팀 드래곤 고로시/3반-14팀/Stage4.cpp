#include "character.h"


void MeteorStackInsert(STAGE4SPHERE* head, RECT rect) 
{
	STAGE4SPHERE* newNode = new STAGE4SPHERE;
	newNode->dis = rand() % 10;
	if (newNode->dis != 0) {
		newNode->rect = { (rand() % 1000),-50,300,300 };
	}
	else {
		newNode->Idle = 4;
		newNode->rect.left = rect.right - 100;
		newNode->rect.right = newNode->rect.left + 135 / 2;
		newNode->rect.top = 400 /*- rand() % 2 * (187 / 2)*/;
		newNode->rect.bottom = newNode->rect.top + (187 / 2);
	}

	newNode->next = head->next;
	head->next = newNode;
}
void MeteorStackDelete(STAGE4SPHERE* head, STAGE4SPHERE* target)
{
	for (STAGE4SPHERE* i = head; i != NULL; i = i->next) {
		if (i->next == target) {
			STAGE4SPHERE* temp = target;
			i->next = i->next->next;
			//free(temp);
			delete temp;
		}
	}
}
void STAGE4MeteorAnimation(HDC BackMemDC, RECT* MeteorRect, CImage* Meteor)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *Meteor);

	*MeteorRect = { MeteorRect->left,MeteorRect->top,
			MeteorRect->left + (Meteor->GetWidth() / 2),MeteorRect->top + (Meteor->GetHeight() / 2) };
	TransparentBlt(BackMemDC, MeteorRect->left, MeteorRect->top, Meteor->GetWidth() / 2, Meteor->GetHeight() / 2
		, ObjectDC, 0, 0, Meteor->GetWidth(), Meteor->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}
void STAGE4MonsterExplodeAnimation(HDC BackMemDC, RECT* mob, CImage* hMonster)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hMonster);
	TransparentBlt(BackMemDC, (mob->left + ((mob->right - mob->left) / 2)) - (hMonster->GetWidth() / 4),
		(mob->top + ((mob->bottom - mob->top) / 2)) - (hMonster->GetHeight() / 4),
		hMonster->GetWidth() / 2, hMonster->GetHeight() / 2
		, ObjectDC, 0, 0, hMonster->GetWidth(), hMonster->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}
void Stage4_StateChange(STAGE4SPHERE* MeteorHead)
{
	for (STAGE4SPHERE* i = MeteorHead->next; i != NULL; ) {
		i->Idlecount++;
		if (i->extinction) {
			if (i->Idlecount >= i->Idle) {
				STAGE4SPHERE* t = i;
				i = i->next;
				MeteorStackDelete(MeteorHead, t);
				continue;
			}
			i = i->next;
			continue;
		}
		if (i->Idlecount >= i->Idle) {
			i->Idlecount = 0;
		}
		i = i->next;
	}
}
void Stage4_OBJECTMOVE(STAGE4SPHERE* MeteorHead, MainCharacter* mainCharacter, RECT rect, STAGE4EVENT* Stage4Event,
	int* oldState,int* oldAnimationNum,int* invincibleTime)
{
	RECT temp;

	for (STAGE4SPHERE* i = MeteorHead; i != NULL; i = i->next) {
		if (i->extinction) {
			continue;
		}
		if (i->dis != 0) {
			i->rect.top += 7; i->rect.left -= 2;
		}
		else {
			i->rect.left -= 10;
		}
		if (IntersectRect(&temp, &mainCharacter->Pos, &i->rect) && *invincibleTime == 0 && mainCharacter->heart != 0) {
			i->extinction = TRUE;

			i->Idlecount = 0;
			if (i->dis != 0) {
				i->Idle = 35;
			}
			else {
				i->Idle = 9;
			}
			mainCharacter->heart--;
			*oldState = mainCharacter->state;
			*oldAnimationNum = mainCharacter->animationNum;
			mainCharacter->state = 6;
			mainCharacter->energy = -1;
			mainCharacter->animationNum = 0;
			*invincibleTime = 100;
		}
		if (rect.bottom <= i->rect.top || rect.left >= i->rect.right) {
			i->extinction = TRUE;
		}
	}
	if (Stage4Event->STAGE4CLEAR) {
		if (!Stage4Event->HIT && Stage4Event->rect.right > rect.right) {
			Stage4Event->rect.left -= 1;
			Stage4Event->rect.right -= 1;
		}
		else if (Stage4Event->HIT && Stage4Event->rect.left < rect.right) {
			Stage4Event->rect.left += 1;
			Stage4Event->rect.right += 1;
		}

	}
}
void STAGE4(HDC BackMemDC, RECT rect, CImage* STAGE4BackGround, STAGE4SPHERE* MeteorHead, ImageOfMonster* IMob, STAGE4EVENT* Stage4Event)
{
	STAGE4BackGround[0].StretchBlt(BackMemDC, 0, 0, rect.right, rect.bottom, 0, 0,
		STAGE4BackGround[0].GetWidth(), STAGE4BackGround[0].GetHeight(), SRCCOPY);
	for (STAGE4SPHERE* i = MeteorHead->next; i != NULL; i = i->next) {
		if (i->dis != 0) {
			if (!i->extinction) {
				STAGE4MeteorAnimation(BackMemDC, &i->rect, &IMob->Stage4Meteor[i->Idlecount]);
			}
			else {
				STAGE4MeteorAnimation(BackMemDC, &i->rect, &IMob->Stage4MeteorExtinction[i->Idlecount]);
			}
		}
		else {
			if (!i->extinction) {
				STAGE4MeteorAnimation(BackMemDC, &i->rect, &IMob->Stage4MonsterLeftMove[i->Idlecount]);
			}
			else {
				STAGE4MonsterExplodeAnimation(BackMemDC, &i->rect, &IMob->Stage4MonsterExplode[i->Idlecount]);
			}
		}
	}
	if (Stage4Event->STAGE4CLEAR) { // 苞翅
		STAGE4MeteorAnimation(BackMemDC, &Stage4Event->rect, &Stage4Event->STAGE4ClearTarget[0]);
	}
}
void CREATESTAGE4(RECT rect, ImageOfMonster* IMob, CImage* STAGE4BackGround, STAGE4EVENT* Stage4Event)
{
	STAGE4BackGround[0].Load(TEXT("府家胶颇老/阁胶磐/back/Stage4BackGround.png"));
	Stage4Event->STAGE4ClearTarget[0].Load(TEXT("府家胶颇老/STAGE4/11.png"));
	Stage4Event->rect = { rect.right,rect.top + 200,rect.right + 100,0 };

	TCHAR str[100];
	for (int i = 0; i < 8; ++i) {
		wsprintf(str, TEXT("府家胶颇老/STAGE4/Meteor/meteor (%d).png"), i + 1);
		IMob->Stage4Meteor[i].Load(str);
	}
	for (int i = 0; i < 35; ++i) {
		wsprintf(str, TEXT("府家胶颇老/STAGE4/Meteor/meteor smoke/meteor_smoke_00%02d.png"), i + 1);
		IMob->Stage4MeteorExtinction[i].Load(str);
	}
	for (int i = 0; i < 4; ++i) {
		wsprintf(str, TEXT("府家胶颇老/STAGE4/Meteor/mob/2 (%d).png"), i + 1);
		IMob->Stage4MonsterLeftMove[i].Load(str);
	}
	for (int i = 0; i < 9; ++i) {
		wsprintf(str, TEXT("府家胶颇老/STAGE4/Meteor/mob/3 (%d).png"), i + 1);
		IMob->Stage4MonsterExplode[i].Load(str);
	}

}
