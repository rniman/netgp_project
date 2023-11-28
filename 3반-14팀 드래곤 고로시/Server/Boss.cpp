#include "ServerCharacter.h"

void CreateBossAndStage(BossMonster* Boss, RECT rect)
{
	Boss->rect = { rect.right - 350, rect.top + 50, rect.right, rect.bottom }; //보스 크기
}

void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss)
{
	if (Boss->AttackTailPreparation < 300)
	{
		Boss->AttackTailrect.left = mainCharacter->info.Pos.left - 10; Boss->AttackTailrect.top = mainCharacter->info.Pos.bottom;
		Boss->AttackTailPreparation++;

		if (Boss->AttackTailPreparation >= 300)
		{
			Boss->tailAnimationLimit = BOSSTAILANI;
			Boss->AttackTailAnimeCount = BOSSTAILPREPAREANI;	// AttackTail의 8부터 시작해서 19까지 사용
			Boss->AttackTailrect.left = mainCharacter->info.Pos.left - 10; Boss->AttackTailrect.top = rect.bottom;

			Boss->AttackTailrect.right = Boss->AttackTailrect.left + BOSSTAILWIDTH / 3;
			Boss->AttackTailrect.bottom = Boss->AttackTailrect.top + BOSSTAILHEIGHT / 2;
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
		Boss->tailAnimationLimit = BOSSTAILPREPAREANI;
		Boss->AttackTailAnimeCount = 0;
		Boss->ATTACKREADY = FALSE;
		Boss->Idle = 8;
		Boss->Idlecount = 0;
		KillTimer(hwnd, 3);
	}

	//RECT temp, hitBox = Boss->AttackTailrect;
	//hitBox = { hitBox.left,hitBox.top,hitBox.right - 65,hitBox.bottom };

	//if (IntersectRect(&temp, &mainCharacter->info.Pos, &hitBox) && mainCharacter->info.heart > 0 && mainCharacter->info.invincibleTime == 0)
	//{
	//	mainCharacter->info.heart -= 1;
	//	*oldState = mainCharacter->info.state;
	//	*oldAnimationNum = mainCharacter->info.animationNum;
	//	mainCharacter->info.state = MainState::HIT;
	//	mainCharacter->info.energy = -1;
	//	mainCharacter->info.animationNum = 0;
	//	mainCharacter->info.invincibleTime = 100;
	//}
}

void BossAttackLoop(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2)
{
	if (Boss->HP <= 0)
	{
		Boss->rect.left += 3;
	}

	RECT temp;
	RECT hitBox = { (Boss->rect.left + Boss->rect.right) / 2 - 100, Boss->rect.top, (Boss->rect.left + Boss->rect.right) / 2 + 100,Boss->rect.bottom };

	BossCollideBullet(rect, hitBox, Boss, player1, player2);
	BossCollidePlayer(rect, hitBox, Boss, player1, player2);
	FireCollidePlayer(rect, Boss, player1, player2);
	MeteorCollidePlayer(rect, Boss, player1, player2);

	//꼬리 충돌
	if (Boss->AttackTailPreparation < 350 || Boss->AttackTailPreparation > 500)
	{
		return;
	}

	hitBox = Boss->AttackTailrect;
	hitBox = { hitBox.left,hitBox.top,hitBox.right - 65,hitBox.bottom };

	TailCollidePlayer(rect, hitBox, player1, player2);
}


void BossCollideBullet(RECT rect, RECT hitBox, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2)
{
	RECT temp;
	for (int i = 0; i < BULLETNUM; ++i)
	{
		if (!player1->info.bullet[i].exist)
		{
			continue;
		}

		if (IntersectRect(&temp, &player1->info.bullet[i].hitBox, &hitBox))
		{
			if (!Boss->POFCRASH) //충돌 상태가 아닐시 명령시행
			{
				if (player1->info.bullet[i].EX)
				{
					Boss->HP -= 5;
				}
				else
				{
					Boss->HP -= 1;
				}

				if (Boss->HP <= 0)
				{
					Boss->Idle = BOSSDEADANI;
					Boss->Idlecount = 0;
				}

				Boss->POFCRASH = TRUE;

				player1->info.bullet[i].exist = FALSE;
				break;
			}
		}
		else
		{
			Boss->POFCRASH = FALSE;
		}
	}

	if (player2->info.type == 2)
	{
		for (int i = 0; i < BULLETNUM; ++i)
		{
			if (!player2->info.bullet[i].exist)
			{
				continue;
			}

			if (IntersectRect(&temp, &player2->info.bullet[i].hitBox, &hitBox))
			{
				if (!Boss->POFCRASH) //충돌 상태가 아닐시 명령시행
				{
					if (player2->info.bullet[i].EX)
					{
						Boss->HP -= 5;
					}
					else
					{
						Boss->HP -= 1;
					}

					if (Boss->HP <= 0)
					{
						Boss->Idle = BOSSDEADANI;
						Boss->Idlecount = 0;
					}

					Boss->POFCRASH = TRUE;

					player2->info.bullet[i].exist = FALSE;
					break;
				}
			}
			else
			{
				Boss->POFCRASH = FALSE;
			}
		}

	}
}

void BossCollidePlayer(RECT rect, RECT hitBox, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2)
{
	RECT temp;
	if (IntersectRect(&temp, &player1->info.Pos, &hitBox) && player1->info.invincibleTime == 0 && player1->info.heart > 0)
	{
		player1->info.heart--;
		player1->info.oldState = player1->info.state;
		player1->info.oldAnimationNum = player1->info.animationNum;
		player1->info.state = MainState::HIT;
		player1->info.energy = -1;
		player1->info.animationNum = 0;
		player1->info.invincibleTime = 100;
	}

	if (player2->info.type == 2)
	{
		if (IntersectRect(&temp, &player2->info.Pos, &hitBox) && player2->info.invincibleTime == 0 && player2->info.heart > 0)
		{
			player2->info.heart--;
			player2->info.oldState = player2->info.state;
			player2->info.oldAnimationNum = player2->info.animationNum;
			player2->info.state = MainState::HIT;
			player2->info.energy = -1;
			player2->info.animationNum = 0;
			player2->info.invincibleTime = 100;
		}
	}
}

void FireCollidePlayer(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2)
{
	RECT temp;
	for (int i = 0; i < FIRENUM; ++i)
	{
		if (!Boss->attackFire[i].exist)
		{
			continue;
		}

		Boss->attackFire[i].rect.top += 3;
		if (rect.bottom < Boss->attackFire[i].rect.top)
		{
			Boss->AttackMeteorReady = FALSE;
			Boss->attackFire[i].exist = FALSE;
			Boss->attackFire[i].animationNum = 0;
			Boss->AttackMeteorPreparation = 0;

			continue;
		}

		//플레이어와 충돌
		if (IntersectRect(&temp, &player1->info.Pos, &Boss->attackFire[i].rect) && player1->info.invincibleTime == 0 && player1->info.heart > 0)
		{
			player1->info.heart--;

			player1->info.oldState = player1->info.state;
			player1->info.oldAnimationNum = player1->info.animationNum;
			player1->info.state = MainState::HIT;
			player1->info.energy = -1;
			player1->info.animationNum = 0;
			player1->info.invincibleTime = 100;
		}
	}

	if (player2->info.type == 2)
	{

		for (int i = 0; i < FIRENUM; ++i)
		{
			if (!Boss->attackFire[i].exist)
			{
				continue;
			}

			//플레이어와 충돌
			if (IntersectRect(&temp, &player2->info.Pos, &Boss->attackFire[i].rect) && player2->info.invincibleTime == 0 && player2->info.heart > 0)
			{
				player2->info.heart--;

				player2->info.oldState = player2->info.state;
				player2->info.oldAnimationNum = player2->info.animationNum;
				player2->info.state = MainState::HIT;
				player2->info.energy = -1;
				player2->info.animationNum = 0;
				player2->info.invincibleTime = 100;
			}
		}
	}
}

void MeteorCollidePlayer(RECT rect, BossMonster* Boss, MainCharacter* player1, MainCharacter* player2)
{
	RECT temp;
	for (int i = 0; i < METEORNUM; ++i)
	{
		if (!Boss->attackMeteor[i].exist)
		{
			continue;
		}

		if (Boss->attackMeteor[i].extinction)
		{
			continue;
		}

		Boss->attackMeteor[i].rect.left -= 15;
		Boss->attackMeteor[i].angle += Boss->attackMeteor[i].yStart;
		Boss->attackMeteor[i].rect.top = Boss->attackMeteor[i].rect.top + (10) * (sin(getradian(Boss->attackMeteor[i].angle)));

		if (Boss->attackMeteor[i].rect.right < rect.left)
		{
			Boss->attackMeteor[i].exist = FALSE;
			Boss->attackMeteor[i].angle = 0;
			Boss->attackMeteor[i].animationNum = 0;
			Boss->attackMeteor[i].extinction = FALSE;
			continue;
		}

		if (IntersectRect(&temp, &player1->info.Pos, &Boss->attackMeteor[i].rect) && player1->info.invincibleTime == 0 && player1->info.heart > 0) //플레이어와 충돌
		{
			//구체만 접촉했을때 폭발 애니메이션 시작. 불기둥은 접촉하면 데미지만 받음.
			Boss->attackMeteor[i].extinction = TRUE;
			Boss->attackMeteor[i].animationNum = 0;

			if (player1->info.heart == 1)
			{
				player1->info.heart = 0;
			}
			else
			{
				player1->info.heart -= 2;
			}

			player1->info.oldState = player1->info.state;
			player1->info.oldAnimationNum = player1->info.animationNum;
			player1->info.state = MainState::HIT;
			player1->info.energy = -1;
			player1->info.animationNum = 0;
			player1->info.invincibleTime = 100;
		}
	}

	if (player2->info.type == 2)
	{
		for (int i = 0; i < METEORNUM; ++i)
		{
			if (!Boss->attackMeteor[i].exist)
			{
				continue;
			}

			if (Boss->attackMeteor[i].extinction)
			{
				continue;
			}

			if (IntersectRect(&temp, &player2->info.Pos, &Boss->attackMeteor[i].rect) && player2->info.invincibleTime == 0 && player2->info.heart > 0) //플레이어와 충돌
			{
				//구체만 접촉했을때 폭발 애니메이션 시작. 불기둥은 접촉하면 데미지만 받음.
				Boss->attackMeteor[i].extinction = TRUE;
				Boss->attackMeteor[i].animationNum = 0;

				if (player2->info.heart == 1)
				{
					player2->info.heart = 0;
				}
				else
				{
					player2->info.heart -= 2;
				}

				player2->info.oldState = player2->info.state;
				player2->info.oldAnimationNum = player2->info.animationNum;
				player2->info.state = MainState::HIT;
				player2->info.energy = -1;
				player2->info.animationNum = 0;
				player2->info.invincibleTime = 100;
			}
		}

	}
}

void TailCollidePlayer(RECT rect, RECT hitBox, MainCharacter* player1, MainCharacter* player2)
{
	RECT temp;
	if (IntersectRect(&temp, &player1->info.Pos, &hitBox) && player1->info.heart > 0 && player1->info.invincibleTime == 0)
	{
		player1->info.heart -= 1;
		player1->info.oldState = player1->info.state;
		player1->info.oldAnimationNum = player1->info.animationNum;
		player1->info.state = MainState::HIT;
		player1->info.energy = -1;
		player1->info.animationNum = 0;
		player1->info.invincibleTime = 100;
	}

	if (player2->info.type == 2)
	{
		if (IntersectRect(&temp, &player2->info.Pos, &hitBox) && player2->info.heart > 0 && player2->info.invincibleTime == 0)
		{
			player2->info.heart -= 1;
			player2->info.oldState = player2->info.state;
			player2->info.oldAnimationNum = player2->info.animationNum;
			player2->info.state = MainState::HIT;
			player2->info.energy = -1;
			player2->info.animationNum = 0;
			player2->info.invincibleTime = 100;
		}
	}
}

void SetBossAndBossAttackRect(BossMonster& boss, const BossBitData& bossBitData)
{
	if (boss.HP > 0)
	{
		if (boss.ATTACKREADY && boss.ATTACK)
		{ //보스 공격 상태
			if (boss.AttackTailReady)
			{
				boss.AttackTailrect = {
					boss.AttackTailrect.left ,
					boss.AttackTailrect.top,
					boss.AttackTailrect.left + bossBitData.ATTACKTAILBitData[boss.AttackTailAnimeCount].bmWidth / 2,
					boss.AttackTailrect.top + bossBitData.ATTACKTAILBitData[boss.AttackTailAnimeCount].bmHeight / 2,
				};
			}
			if (boss.AttackMeteorReady)
			{
				if (boss.AttackMeteorPreparation < 11)
				{
					boss.AttackMeteorect[0] = {
						boss.AttackMeteorect[0].left ,
						boss.AttackMeteorect[0].top,
						boss.AttackMeteorect[0].left + bossBitData.ATTACKMETEORBitData[boss.AttackMeteorAnimeCount].bmWidth / 2,
						boss.AttackMeteorect[0].top + bossBitData.ATTACKMETEORBitData[boss.AttackMeteorAnimeCount].bmHeight / 2,
					};
				}
			}
		}
	}

	//추가
	for (int i = 0; i < FIRENUM; ++i)
	{
		if (!boss.attackFire[i].exist)
		{
			break;
		}

		boss.attackFire[i].rect = {
			boss.attackFire[i].rect.left ,
			boss.attackFire[i].rect.top,
			boss.attackFire[i].rect.left + bossBitData.ATTACKFIREBitData[boss.attackFire[i].animationNum].bmWidth / 2,
			boss.attackFire[i].rect.top + bossBitData.ATTACKFIREBitData[boss.attackFire[i].animationNum].bmHeight / 2,
		};
	}

	for (int i = 0; i < METEORNUM; ++i)
	{
		if (!boss.attackMeteor[i].exist)
		{
			continue;
		}

		if (!boss.attackMeteor[i].extinction)
		{
			boss.attackMeteor[i].rect = {
				boss.attackMeteor[i].rect.left ,
				boss.attackMeteor[i].rect.top,
				boss.attackMeteor[i].rect.left + bossBitData.ATTACKMETEORBitData[11 + boss.attackMeteor[i].animationNum].bmWidth / 2,
				boss.attackMeteor[i].rect.top + bossBitData.ATTACKMETEORBitData[11 + boss.attackMeteor[i].animationNum].bmHeight / 2,
			};
		}
		else
		{
			boss.attackMeteor[i].rect = {
				boss.attackMeteor[i].rect.left ,
				boss.attackMeteor[i].rect.top,
				boss.attackMeteor[i].rect.left + bossBitData.METEOREXTINCTIONBitData[boss.attackMeteor[i].animationNum].bmWidth / 2,
				boss.attackMeteor[i].rect.top + bossBitData.METEOREXTINCTIONBitData[boss.attackMeteor[i].animationNum].bmHeight / 2,
			};
		}
	}
}


//수정
void BossStateChange(BossMonster* Boss, HWND hwnd, RECT rect)
{
	Boss->Idlecount++;
	if (Boss->HP <= 0)
	{
		if (Boss->Idlecount >= BOSSDEADANI)
			Boss->Idlecount = 0;
		return;
	}

	if (Boss->Idlecount >= Boss->Idle)
	{
		Boss->Idlecount = 0;
		if (!Boss->ATTACKREADY && !Boss->ATTACK)
		{
			Boss->ATTACKREADY = TRUE;
			Boss->Idle = 8;
		}
		else if (Boss->ATTACKREADY && !Boss->ATTACK)
		{
			Boss->ATTACK = TRUE;
			Boss->Idle = 16;
		}
		else if (Boss->ATTACKREADY && Boss->ATTACK)
		{
			if (!Boss->AttackTailReady)
			{
				SetTimer(hwnd, 2, 1000, NULL); //일정 시간이 되면 꼬리 공격 시행
			}

			int temp = rand() % 2;
			if (temp == 0)
			{
				if (!Boss->AttackMeteorReady)
				{
					Boss->AttackMeteorect[0] = { Boss->rect.left,Boss->rect.top + 100,Boss->rect.left,Boss->rect.bottom };
					Boss->AttackMeteorReady = TRUE;
				}
			}
			temp = rand() % 2;
			if (temp == 0 && Boss->HP <= 50)
			{  //HP 50 이하이면 2페이즈 공격 실시
				CreateBossMeteor(*Boss, rect);
			}

		}
		else if (!Boss->ATTACKREADY && Boss->ATTACK)
		{
			Boss->ATTACK = FALSE;
			Boss->AttackMeteorReady = FALSE;
			Boss->Idle = 16;
		}
	}

	for (int i = 0; i < FIRENUM; ++i)
	{
		if (!Boss->attackFire[i].exist)
		{
			continue;
		}

		Boss->attackFire[i].animationNum++;
		if (Boss->attackFire[i].animationNum >= FIREANI)
		{
			Boss->attackFire[i].animationNum = 0;
		}
	}

	for (int i = 0; i < METEORNUM; ++i)
	{
		if (!Boss->attackMeteor[i].exist)
		{
			continue;
		}

		Boss->attackMeteor[i].animationNum++;
		if (!Boss->attackMeteor[i].extinction && Boss->attackMeteor[i].animationNum >= METEORANI)
		{
			Boss->attackMeteor[i].animationNum = 0;
		}
		else if (Boss->attackMeteor[i].extinction && Boss->attackMeteor[i].animationNum >= METEOREXTINCTIONANI)
		{
			Boss->attackMeteor[i].exist = FALSE;
			Boss->attackMeteor[i].extinction = FALSE;
			Boss->attackMeteor[i].angle = 0;
			Boss->attackMeteor[i].animationNum = 0;
		}
	}
}

void BossAttackStateChange(BossMonster* Boss, RECT rect)
{
	if (Boss->HP <= 0)
	{
		return;
	}

	if (Boss->ATTACKREADY && Boss->ATTACK)
	{
		if (Boss->AttackTailReady)
		{
			Boss->AttackTailAnimeCount++;
			if (Boss->AttackTailAnimeCount >= Boss->tailAnimationLimit)	// Tain공격이 시작되면 Count가 8부터 시작해서 20까지 간다.
			{
				if (Boss->tailAnimationLimit == BOSSTAILPREPAREANI) // 꼬리가 올라올 위치에 링을 표시하는 상태
				{
					Boss->AttackTailAnimeCount = 0;
				}
				else if (Boss->tailAnimationLimit == BOSSTAILANI)	// 꼬리가 보이는 상태
				{
					Boss->AttackTailAnimeCount = BOSSTAILPREPAREANI;
				}
			}
		}

		if (Boss->AttackMeteorReady)
		{
			Boss->AttackMeteorPreparation++;
			if (Boss->AttackMeteorPreparation == 11)
			{
				for (int i = 0; i < FIRENUM; ++i) //불기둥 생성
				{
					CreateBossFire(*Boss, rect);
				}
			}
			Boss->AttackMeteorAnimeCount++;
			if (Boss->AttackMeteorAnimeCount >= Boss->Meteor)
			{
				if (Boss->Meteor == 11)
				{
					Boss->AttackMeteorAnimeCount = 0;
				}
				else
				{
					Boss->AttackMeteorAnimeCount = 11;
				}
			}
		}
	}
}

void CreateBossMeteor(BossMonster& boss, RECT rect)
{
	for (int i = 0; i < METEORNUM; ++i)
	{
		if (boss.attackMeteor[i].exist)
		{
			continue;
		}

		boss.attackMeteor[i].yStart = (rand() % 4) + 3;
		boss.attackMeteor[i].exist = TRUE;
		boss.attackMeteor[i].extinction = FALSE;
		boss.attackMeteor[i].angle = 0;
		boss.attackMeteor[i].rect = { rect.right - 50, rect.top + 150, 0, 0 };
		break;
	}
}

void CreateBossFire(BossMonster& boss, RECT rect)
{
	for (int i = 0; i < FIRENUM; ++i)
	{
		if (boss.attackFire[i].exist)
		{
			break;
		}

		boss.attackFire[i].exist = TRUE;
		boss.attackFire[i].animationNum = 0;
		boss.attackFire[i].rect = { rect.left + ((((-i - 1) * -1) - 1) * 200), rect.top - 100, rect.left, rect.top };
	}
}

//추가
double getradian(int num)
{
	return num * (PI / 180);
}