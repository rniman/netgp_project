#include"character.h"



void STAGE2(HDC BackMemDC, RECT rect, CImage STAGE3BackGround[], MonsterStage2 Mob[], ImageOfMonster* IMob)
{
	STAGE3BackGround[0].StretchBlt(BackMemDC, 0, 0, rect.right, rect.bottom, 0, 0,
		STAGE3BackGround[0].GetWidth(), STAGE3BackGround[0].GetHeight(), SRCCOPY);

	for (int i = 0; i < 4; ++i) {
		if (!Mob[i].ALIVE && !Mob[i].Explode) { continue; }
		if (Mob[i].Explode) {
			MonsterExplodeAnimation(BackMemDC, &Mob[i], &IMob->Stage2MonsterExplode[Mob[i].Idlecount]);
			continue;
		}
		if (!Mob[i].MOVE) {
			MonsterIdleAnimation(BackMemDC, &Mob[i], &IMob->Stage2MonsterIDLE[Mob[i].Idlecount]);
		}
		else {
			if (Mob[i].LEFT) {
				MonsterIdleAnimation(BackMemDC, &Mob[i], &IMob->Stage2MonsterLeftMove[Mob[i].Idlecount]);
			}
			else {
				MonsterIdleAnimation(BackMemDC, &Mob[i], &IMob->Stage2MonsterRightMove[Mob[i].Idlecount]);
			}
		}
	}
}

void CREATESTAGE2(CImage STAGE3BackGround[], MONSTERIMAGE* IMob, MonsterStage2 Mob[], RECT rect)
{
	STAGE3BackGround[0].Load(TEXT("리소스파일/몬스터/back/Stage2BackGround.png"));
	STAGE3BackGround[1].Load(TEXT("리소스파일/몬스터/back/레이어 1.png"));
	TCHAR str[100];
	for (int i = 0; i < 10; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/FireFly/%d.png"), i + 1);
		IMob->Stage2MonsterIDLE[i].Load(str);
	}

	for (int i = 0; i < 9; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/FireFly/3 (%d).png"), i + 1);
		IMob->Stage2MonsterExplode[i].Load(str);
	}

	for (int i = 0; i < 4; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/FireFly/2 (%d).png"), i + 1);
		IMob->Stage2MonsterLeftMove[i].Load(str);
		wsprintf(str, TEXT("리소스파일/몬스터/FireFly/2 (%d)R.png"), i + 1);
		IMob->Stage2MonsterRightMove[i].Load(str);
	}

	for (int i = 0; i < 4; ++i) {
		Mob[i].rect = { rect.right - 200 - ((i * IMob->Stage2MonsterIDLE->GetWidth()) - 20),
			rect.bottom - 250,rect.right - 200,rect.bottom - 250 };
	}
}

void MonsterIdleAnimation(HDC BackMemDC, MonsterStage2* Mob, CImage* hMonster)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hMonster);
	Mob->rect = { Mob->rect.left,Mob->rect.top,Mob->rect.left + hMonster->GetWidth() - 20,
		Mob->rect.top + hMonster->GetHeight() - 20 };
	TransparentBlt(BackMemDC, Mob->rect.left, Mob->rect.top, hMonster->GetWidth() - 20, hMonster->GetHeight() - 20
		, ObjectDC, 0, 0, hMonster->GetWidth(), hMonster->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void MonsterExplodeAnimation(HDC BackMemDC, MonsterStage2* Mob, CImage* hMonster)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hMonster);
	TransparentBlt(BackMemDC, (Mob->rect.left + ((Mob->rect.right - Mob->rect.left) / 2)) - (hMonster->GetWidth() / 4),
		(Mob->rect.top + ((Mob->rect.bottom - Mob->rect.top) / 2)) - (hMonster->GetHeight() / 4),
		hMonster->GetWidth() / 2, hMonster->GetHeight() / 2
		, ObjectDC, 0, 0, hMonster->GetWidth(), hMonster->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void Stage2_MonsterStateChange(MonsterStage2 Mob[], const RECT rect, const RECT player)
{
	for (int i = 0; i < 4; ++i) {
		if (!Mob[i].ALIVE && !Mob[i].Explode) { continue; }

		Mob[i].Idlecount++;
		if (Mob[i].Explode) {
			if (Mob[i].Idlecount >= Mob[i].explosion) {
				Mob[i].Explode = FALSE; Mob[i].Idlecount = 0;
			}
			continue;
		}

		if (!Mob[i].MOVE) {
			if (Mob[i].Idlecount >= Mob[i].Idle) {
				Mob[i].Idlecount = 0;
				int rd = rand() % 3;
				if (rd == 0) { //가만히
					continue;
				}
				else if (rd == 1) { //왼쪽으로 이동
					Mob[i].MOVE = TRUE; Mob[i].LEFT = TRUE;
				}
				else if (rd == 2) {//오른쪽으로 이동
					Mob[i].MOVE = TRUE; Mob[i].LEFT = FALSE;
				}
			}
		}
		else if (Mob[i].MOVE) {
			if (Mob[i].LEFT) {
				Mob[i].rect.left -= 5;
				Mob[i].Movecount++;		//움직인 횟수가 제한 횟수를 넘어서면 움직임을 멈춰라
				if (rect.left > Mob[i].rect.left) {
					Mob[i].rect.left += 5;  Mob[i].LEFT = FALSE;
				}
				if (Mob[i].Movecount >= 20) {
					Mob[i].Movecount = 0; Mob[i].MOVE = FALSE;
					int dis = ((player.left + ((player.right - player.left) / 2)) //플레이어와 적에 거리
						- (Mob[i].rect.left + ((Mob[i].rect.right - Mob[i].rect.left) / 2)));
					/*if (dis < 0) {
						dis *= -1;
					}*/

					if (dis > 0 &&dis < 300 /*&& player.rect.left + ((player.rect.right - player.rect.left) / 2) < Mob[i].rect.left*/) {
						Mob[i].MOVE = TRUE;
						Mob[i].LEFT = FALSE;
					}
					else if(dis < 0 && dis > -300 ) {
						Mob[i].MOVE = TRUE;
					}
				}
			}
			else {
				Mob[i].rect.left += 5;
				Mob[i].Movecount++;
				if (rect.right < Mob[i].rect.right) {
					Mob[i].rect.left -= 5;  Mob[i].LEFT = TRUE;
				}
				if (Mob[i].Movecount >= 20) {
					Mob[i].Movecount = 0; Mob[i].MOVE = FALSE;
					int dis = ((player.left + ((player.right - player.left) / 2)) //플레이어와 적에 거리
						- (Mob[i].rect.left + ((Mob[i].rect.right - Mob[i].rect.left) / 2)));
					/*if (dis < 0) {
						dis *= -1;
					}*/

					if (dis > 0 && dis < 300) {
						Mob[i].MOVE = TRUE;
					}
					else if (dis < 0 && dis > -300) {
						Mob[i].MOVE = TRUE;
						Mob[i].LEFT = TRUE;
					}
				}
			}
			if (Mob[i].Idlecount >= Mob[i].Move) {
				Mob[i].Idlecount = 0;
			}
		}
	}
}

/// <summary>
/// ///////
/// </summary>
/// <param name="BackMemDC"></param>
/// <param name="Mob"></param>
/// <param name="hMonster"></param>
/// 
void MonsterIdleAnimation2(HDC BackMemDC, MonsterStage3* Mob, CImage* hMonster)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hMonster);
	Mob->rect = { Mob->rect.left,Mob->rect.top,Mob->rect.left + hMonster->GetWidth() - 20,
		Mob->rect.top + hMonster->GetHeight() - 20 };
	TransparentBlt(BackMemDC, Mob->rect.left, Mob->rect.top, hMonster->GetWidth() - 20, hMonster->GetHeight() - 20
		, ObjectDC, 0, 0, hMonster->GetWidth(), hMonster->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void StackInsert(FireAttack* head, RECT rect, RECT Prect) //Prect : 플레이어 rect
{
	FireAttack* newNode = new FireAttack;
	newNode->rect = { rect.left + ((rect.right - rect.left) / 2) - 30,rect.top + ((rect.bottom - rect.top) / 2),
	rect.left + ((rect.right - rect.left) / 2),rect.top + ((rect.bottom - rect.top) / 2) };
	newNode->target = { Prect.left + ((Prect.right - Prect.left) / 2), Prect.top + ((Prect.bottom - Prect.top) / 2) };
	//여기에 생성될 당시에 몬스터의 중심 자리를 creatept에 넣고 그 값을 x1 값으로 설정하여 거리 공식
	newNode->createPt = { rect.left + ((rect.right - rect.left) / 2) ,rect.top + ((rect.bottom - rect.top) / 2) };
	newNode->next = head->next;
	head->next = newNode;
}

void StackDelete(FireAttack* head, FireAttack* target)
{
	for (FireAttack* i = head; i != NULL; i = i->next) {
		if (i->next == target) {
			FireAttack* temp = target;
			i->next = i->next->next;
			free(temp);
		}
	}
}

void MonsterFireAttackAnimation(HDC BackMemDC, FireAttack* FireAttack, CImage* hMonster)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hMonster);
	FireAttack->rect = { FireAttack->rect.left,FireAttack->rect.top,FireAttack->rect.left + hMonster->GetWidth(),
		FireAttack->rect.top + hMonster->GetHeight() };
	TransparentBlt(BackMemDC, FireAttack->rect.left, FireAttack->rect.top, hMonster->GetWidth(), hMonster->GetHeight()
		, ObjectDC, 0, 0, hMonster->GetWidth(), hMonster->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void PlayerStage3Mobcrash(MonsterStage3 MobStage3[],BLinkedList* bullet)
{
	BULLETNODE* cus = bullet->head;
	RECT temp;

	while (cus != NULL) {
		if (cus->data.exist == FALSE) {
			cus = cus->link;
			continue;
		}

		if (MobStage3[0].ALIVE && IntersectRect(&temp, &cus->data.hitBox, &MobStage3[0].rect)) {  //몬스터와 플레이어 충돌시 몬스터 사망.
			MobStage3[0].ALIVE = FALSE;
			MobStage3[0].Idle = 17; //죽음 애니메이션
			MobStage3[0].Idlecount = 0;
			cus->data.exist = FALSE;
			break;
		}

		cus = cus->link;
	}
}

void Stage3_TimeCrashEvent(MonsterStage3 MobStage3[], FireAttack* FireAttackHead, MainCharacter* mainCharacter,
	int* oldState, int* oldAnimationNum, int* invincibleTime)
{
	RECT temp;
	if (!MobStage3[0].ALIVE) {
		if (MobStage3[0].Idlecount >= 13) {
			MobStage3[0].rect.left += 2;
			MobStage3[0].rect.right += 2;
			MobStage3[0].rect.top -= 2;
			MobStage3[0].rect.bottom -= 2;
		}
	}

	for (FireAttack* i = FireAttackHead; i != NULL;) {

		i->rect.left = (float)i->createPt.x +
			((((float)i->target.x - (float)i->createPt.x) / 40) * i->dis++);
		i->rect.top = ((((float)i->target.y - (float)i->createPt.y)
			/ ((float)i->target.x - (float)i->createPt.x))
			* (i->rect.left - (float)i->createPt.x)) + (float)i->createPt.y;
		i->rect.top -= 25; //구체 조정

		if (i->dis >= 100) {
			i->dis = 0;
			FireAttack* t = i;
			i = i->next;
			StackDelete(FireAttackHead, t);
			continue;
		}

		if (IntersectRect(&temp, &mainCharacter->Pos, &i->rect) && *invincibleTime == 0 && mainCharacter->heart != 0) { //플레이어와 공격구체 충돌
			FireAttack* t = i;
			i = i->next;
			mainCharacter->heart -= 1;
			*oldState = mainCharacter->state;
			*oldAnimationNum = mainCharacter->animationNum;
			mainCharacter->state = 6;
			mainCharacter->energy = -1;
			mainCharacter->animationNum = 0;
			*invincibleTime = 100;
			StackDelete(FireAttackHead, t);
			continue;
		}

		i = i->next;
	}
}

void Stage3_MonsterStateChange(MonsterStage3 MobStage3[], FireAttack* FireAttackHead, RECT PlayerRect, RECT ProgramRect)
{
	for (int i = 0; i < 1; ++i) {
		MobStage3[i].Idlecount++;
		if (!MobStage3[i].ALIVE) {
			if (MobStage3[i].Idlecount >= MobStage3[i].Idle) {
				MobStage3[i].Idlecount = 13;
			}
			continue;
		}

		if (MobStage3[i].Idlecount >= MobStage3[i].Idle) {
			MobStage3[i].Idlecount = 0;
			if (!MobStage3[i].ATTACK) {
				MobStage3[i].ATTACK = TRUE; MobStage3[i].Idle = 25;
			}
			else {
				for (int j = 0; j < 1; ++j) {
					MobStage3[i].MOVE = rand() % 4;
					if (MobStage3[i].oldMove == MobStage3[i].MOVE) {
						j--;
						continue;
					}
					MobStage3[i].oldMove = MobStage3[i].MOVE;
				}
				MobStage3[i].ATTACK = FALSE; MobStage3[i].Idle = 18;
			}
		}
		if (MobStage3[i].ATTACK) {
			if (MobStage3[i].Idlecount == 17) {
				StackInsert(FireAttackHead, MobStage3[i].rect, PlayerRect);
			}
			if (MobStage3[i].Idlecount > 17) {
				if (MobStage3[i].MOVE == 0) {
					MobStage3[i].rect.left += 25;
					MobStage3[i].rect.right += 25;

					if (MobStage3[i].rect.right >= ProgramRect.right) {
						MobStage3[i].rect.left -= 25;
						MobStage3[i].rect.right -= 25;
					}
				}
				else if (MobStage3[i].MOVE == 1) {
					MobStage3[i].rect.left -= 25;
					MobStage3[i].rect.right -= 25;
					if (MobStage3[i].rect.left <= ProgramRect.left) {
						MobStage3[i].rect.left += 25;
						MobStage3[i].rect.right += 25;
					}
				}
				else if (MobStage3[i].MOVE == 2) {
					MobStage3[i].rect.top += 25;
					MobStage3[i].rect.bottom += 25;
					if (MobStage3[i].rect.bottom >= ProgramRect.bottom - 150) {
						MobStage3[i].rect.top -= 25;
						MobStage3[i].rect.bottom -= 25;
					}
				}
				else if (MobStage3[i].MOVE == 3) {
					MobStage3[i].rect.top -= 25;
					MobStage3[i].rect.bottom -= 25;
					if (MobStage3[i].rect.top <= ProgramRect.top) {
						MobStage3[i].rect.top += 25;
						MobStage3[i].rect.bottom += 25;
					}
				}
			}
		}
	}

	for (FireAttack* i = FireAttackHead; i != NULL; i = i->next) {
		i->Idlecount++;
		if (i->Idlecount >= i->Idle) {
			i->Idlecount = 0;
		}
	}

}

void Stage3(HDC BackMemDC, RECT rect, CImage* STAGE3BackGround, MonsterStage3 MobStage3[], ImageOfMonster* IMob, FireAttack* FireAttackHead)
{
	STAGE3BackGround->StretchBlt(BackMemDC, 0, 0, rect.right, rect.bottom, 0, 0, STAGE3BackGround->GetWidth(), STAGE3BackGround->GetHeight(), SRCCOPY);

	if (MobStage3[0].ALIVE) {
		if (!MobStage3[0].ATTACK) {
			MonsterIdleAnimation2(BackMemDC, &MobStage3[0], &IMob->Stage3MonsterIDLE[MobStage3[0].Idlecount]);
		}
		else {
			MonsterIdleAnimation2(BackMemDC, &MobStage3[0], &IMob->Stage3MonsterAttack[MobStage3[0].Idlecount]);
		}
	}
	else {
		MonsterIdleAnimation2(BackMemDC, &MobStage3[0], &IMob->Stage3MonsterDead[MobStage3[0].Idlecount]);
	}

	for (FireAttack* i = FireAttackHead->next; i != NULL; i = i->next) {
		MonsterFireAttackAnimation(BackMemDC, i, &IMob->Stage3MonsterFireAttack[i->Idlecount]);
	}

}

void CREATESTAGE3(CImage* STAGE3BackGround, MONSTERIMAGE* IMob, MonsterStage3 MobStage3[], RECT rect)
{
	STAGE3BackGround->Load(TEXT("리소스파일/몬스터/back/Stage3BackGround.png"));

	TCHAR str[100];
	for (int i = 0; i < 18; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/baby/%d.png"), i + 1);
		IMob->Stage3MonsterIDLE[i].Load(str);
	}
	for (int i = 0; i < 25; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/baby/attack/%d.png"), i + 1);
		IMob->Stage3MonsterAttack[i].Load(str);
	}
	for (int i = 0; i < 20; ++i) {
		wsprintf(str, TEXT("리소스파일/몬스터/baby/attack/fire/%d.png"), i + 1);
		IMob->Stage3MonsterFireAttack[i].Load(str);

		wsprintf(str, TEXT("리소스파일/몬스터/baby/dead/%d.png"), i + 1);
		IMob->Stage3MonsterDead[i].Load(str);
	}

	MobStage3[0].rect = { rect.right - 600,rect.top + 20,0,0 };
}

