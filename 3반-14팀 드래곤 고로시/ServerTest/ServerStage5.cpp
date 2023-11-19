#include "ServerCharacter.h"

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

void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, MainState* oldState, int* oldAnimationNum, int* invincibleTime)
{
	if (Boss->AttackTailPreparation < 300)
	{
		Boss->AttackTailrect.left = mainCharacter->info.Pos.left - 10; Boss->AttackTailrect.top = mainCharacter->info.Pos.bottom;
		Boss->AttackTailPreparation++;

		if (Boss->AttackTailPreparation >= 300)
		{
			Boss->Tail = 20; Boss->AttackTailAnimeCount = 8;
			Boss->AttackTailrect.left = mainCharacter->info.Pos.left - 10; Boss->AttackTailrect.top = rect.bottom;
			Boss->AttackTailrect.right = Boss->AttackTailrect.left + Boss->AttackTail[Boss->AttackTailAnimeCount].GetWidth() / 3;
			Boss->AttackTailrect.bottom = Boss->AttackTailrect.top + Boss->AttackTail[Boss->AttackTailAnimeCount].GetHeight() / 2;
		}
	}
	else if (Boss->AttackTailPreparation < 350)
	{
		Boss->AttackTailPreparation++;
	}
	else if (Boss->AttackTailPreparation < 420) //꼬리 공격 실행
	{
		Boss->AttackTailPreparation++;
		Boss->AttackTailrect.top -= 5;
		Boss->AttackTailrect.bottom -= 5;
	}
	else if (Boss->AttackTailPreparation < 500)
	{
		Boss->AttackTailPreparation++;
		Boss->AttackTailrect.top += 5;
		Boss->AttackTailrect.bottom += 5;
	}
	else
	{
		Boss->AttackTailReady = FALSE;
		Boss->AttackTailPreparation = 0;
		Boss->Tail = 8; Boss->AttackTailAnimeCount = 0;
		Boss->ATTACKREADY = FALSE;
		Boss->Idle = 8;
		Boss->Idlecount = 0;
		KillTimer(hwnd, 3);
	}

	RECT temp, hitBox = Boss->AttackTailrect;
	hitBox = { hitBox.left,hitBox.top,hitBox.right - 65,hitBox.bottom };

	if (IntersectRect(&temp, &mainCharacter->info.Pos, &hitBox) && mainCharacter->info.heart > 0 && *invincibleTime == 0)
	{
		mainCharacter->info.heart -= 1;
		*oldState = mainCharacter->info.state;
		*oldAnimationNum = mainCharacter->info.animationNum;
		mainCharacter->info.state = MainState::HIT;
		mainCharacter->info.energy = -1;
		mainCharacter->info.animationNum = 0;
		*invincibleTime = 100;
	}
}

void BossAttackMeteor(RECT rect, BossMonster* Boss, STAGE4SPHERE* head, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum, int* invincibleTime)
{
	RECT temp;

	if (Boss->HP <= 0) {
		Boss->rect.left += 3;
	}

	RECT hitBox = { (Boss->rect.left + Boss->rect.right) / 2 - 100, Boss->rect.top, (Boss->rect.left + Boss->rect.right) / 2 + 100,Boss->rect.bottom };

	for (int i = 0; i < BULLETNUM; ++i)
	{
		if (!mainCharacter->info.bullet[i].exist)
		{
			continue;
		}

		if (IntersectRect(&temp, &mainCharacter->info.bullet[i].hitBox, &hitBox))
		{
			if (!Boss->POFCRASH) //충돌 상태가 아닐시 명령시행
			{
				if (mainCharacter->info.bullet[i].EX)
				{
					Boss->HP -= 5;
				}
				else
				{
					Boss->HP -= 1;
				}

				if (Boss->HP <= 0)
				{
					Boss->Idle = 19;
				}

				Boss->POFCRASH = TRUE;

				mainCharacter->info.bullet[i].exist = FALSE;
				break;
			}
		}
		else
		{
			Boss->POFCRASH = FALSE;
		}
	}

	if (IntersectRect(&temp, &mainCharacter->info.Pos, &hitBox) && *invincibleTime == 0 && mainCharacter->info.heart > 0)
	{
		mainCharacter->info.heart--;
		*oldState = mainCharacter->info.state;
		*oldAnimationNum = mainCharacter->info.animationNum;
		mainCharacter->info.state = MainState::HIT;
		mainCharacter->info.energy = -1;
		mainCharacter->info.animationNum = 0;
		*invincibleTime = 100;
	}

	for (STAGE4SPHERE* i = head->next; i != NULL; )
	{
		if (i->extinction)
		{
			i = i->next;
			continue;
		}

		if (i->dis >= 0)
		{
			i->rect.left -= 15;
			i->rect.top = i->rect.top + (10) * (sin(getradian(i->dis += i->ySum)));
		}
		else if (i->dis <= -1)
		{
			i->rect.top += 3;
			if (rect.bottom < i->rect.top)
			{
				STAGE4SPHERE* t = i;
				i = i->next;
				Boss->AttackMeteorReady = FALSE;
				MeteorStackDelete(head, t);
				Boss->AttackMeteorPreparation = 0;
				continue;
			}
		}

		if (i->rect.right < rect.left)
		{
			STAGE4SPHERE* t = i;
			i = i->next;
			MeteorStackDelete(head, t);
			continue;
		}

		if (IntersectRect(&temp, &mainCharacter->info.Pos, &i->rect) && *invincibleTime == 0 && mainCharacter->info.heart > 0) //플레이어와 충돌
		{
			if (i->dis >= 0) //구체만 접촉했을때 폭발 애니메이션 시작. 불기둥은 접촉하면 데미지만 받음.
			{
				i->extinction = TRUE; i->Idlecount = 0;
				i->Idle = 35;
			}

			if (i->dis >= 0)
			{
				if (mainCharacter->info.heart == 1)
				{
					mainCharacter->info.heart = 0;
				}
				else
				{
					mainCharacter->info.heart -= 2;
				}
			}
			else
			{
				mainCharacter->info.heart--;
			}

			*oldState = mainCharacter->info.state;
			*oldAnimationNum = mainCharacter->info.animationNum;
			mainCharacter->info.state = MainState::HIT;
			mainCharacter->info.energy = -1;
			mainCharacter->info.animationNum = 0;
			*invincibleTime = 100;
		}

		i = i->next;
	}

}

//수정
void BossStateChange(BossMonster* Boss, HWND hwnd, STAGE4SPHERE* BossMeteorHead, RECT rect)
{
	Boss->Idlecount++;
	if (Boss->HP <= 0) {
		if (Boss->Idlecount >= Boss->Idle)
			Boss->Idlecount = 12;
		return;
	}
	if (Boss->Idlecount >= Boss->Idle) {
		Boss->Idlecount = 0;
		if (!Boss->ATTACKREADY && !Boss->ATTACK) {
			Boss->ATTACKREADY = TRUE;
			Boss->Idle = 8;
		}
		else if (Boss->ATTACKREADY && !Boss->ATTACK) {
			Boss->ATTACK = TRUE;
			Boss->Idle = 16;
		}
		else if (Boss->ATTACKREADY && Boss->ATTACK) {
			if (!Boss->AttackTailReady) {
				SetTimer(hwnd, 2, 1000, NULL); //일정 시간이 되면 꼬리 공격 시행
			}
			int temp = rand() % 2;
			if (temp == 0) {
				if (!Boss->AttackMeteorReady) {
					Boss->AttackMeteorect[0] = { Boss->rect.left,Boss->rect.top + 100,Boss->rect.left,Boss->rect.bottom };
					Boss->AttackMeteorReady = TRUE;
				}
			}
			temp = rand() % 2;
			if (temp == 0 && Boss->HP <= 50) {  //HP 50 이하이면 2페이즈 공격 실시
				BossMeteorStackInsert(BossMeteorHead, rect, 0);
			}

		}
		else if (!Boss->ATTACKREADY && Boss->ATTACK) {
			Boss->ATTACK = FALSE;
			Boss->AttackMeteorReady = FALSE;
			Boss->Idle = 16;
		}
	}

	for (STAGE4SPHERE* i = BossMeteorHead->next; i != NULL; ) {
		i->Idlecount++;
		if (!i->extinction && i->Idlecount >= i->Idle) {
			i->Idlecount = 0;
		}
		else if (i->extinction && i->Idlecount >= i->Idle) {
			STAGE4SPHERE* t = i;
			i = i->next;
			MeteorStackDelete(BossMeteorHead, t);
			continue;
		}
		i = i->next;
	}

}

void BossAttackStateChange(BossMonster* Boss, RECT rect, STAGE4SPHERE* head)
{
	if (Boss->HP <= 0) {
		return;
	}
	if (Boss->ATTACKREADY && Boss->ATTACK) {
		if (Boss->AttackTailReady) {
			Boss->AttackTailAnimeCount++;
			if (Boss->AttackTailAnimeCount >= Boss->Tail) {
				if (Boss->Tail == 8) {
					Boss->AttackTailAnimeCount = 0;
				}
				else {
					Boss->AttackTailAnimeCount = 8;
				}
			}
		}
		if (Boss->AttackMeteorReady) {
			Boss->AttackMeteorPreparation++;
			if (Boss->AttackMeteorPreparation == 11) {
				for (int i = 0; i < 5; ++i) {
					BossMeteorStackInsert(head, rect, -(i + 1));
				}
			}
			Boss->AttackMeteorAnimeCount++;
			if (Boss->AttackMeteorAnimeCount >= Boss->Meteor) {
				if (Boss->Meteor == 11) {
					Boss->AttackMeteorAnimeCount = 0;
				}
				else {
					Boss->AttackMeteorAnimeCount = 11;
				}
			}
		}
	}
}

//추가
void BossMeteorStackInsert(STAGE4SPHERE* head, RECT rect, int num)
{
	STAGE4SPHERE* newNode = new STAGE4SPHERE;
	if (num <= -1) {
		newNode->rect = { rect.left + (((num * -1) - 1) * 200),rect.top - 100,rect.left,rect.top };
	}
	else {
		newNode->rect = { rect.right - 50,rect.top + 150,0,0 };
	}
	newNode->dis = num;
	newNode->next = head->next;
	head->next = newNode;
}
//추가
double getradian(int num)
{
	return num * (PI / 180);
}