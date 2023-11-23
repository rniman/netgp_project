#include "ClientCharacter.h"

//스테이지 5
void PaintBoss(HDC BackMemDC, RECT rect, BossMonster* Boss, BOSSCIMAGE* bossImage, CImage BossGround[])
{
	HDC mdc;
	BossBackground(BackMemDC, rect, BossGround); //스테이지 보스배경
	if (Boss->HP > 0) 
	{
		if (!Boss->ATTACKREADY && !Boss->ATTACK)
		{ //보스 기본 상태
			BossMob(BackMemDC, Boss, &bossImage->BossIDLE[Boss->Idlecount]);
		}
		else if (Boss->ATTACKREADY && !Boss->ATTACK)
		{ //보스 공격 준비
			BossMob(BackMemDC, Boss, &bossImage->BossAttackReady[Boss->Idlecount]);
		}
		else if (Boss->ATTACKREADY && Boss->ATTACK)
		{ //보스 공격 상태
			BossMob(BackMemDC, Boss, &bossImage->BossAttack[Boss->Idlecount]);
			if (Boss->AttackTailReady)
			{
				BossAttackAnimation(BackMemDC, &Boss->AttackTailrect, &bossImage->AttackTail[Boss->AttackTailAnimeCount]);
			}
			if (Boss->AttackMeteorReady)
			{
				if (Boss->AttackMeteorPreparation < 11)
				{
					BossAttackAnimation(BackMemDC, &Boss->AttackMeteorect[0], &bossImage->AttackMeteor[Boss->AttackMeteorAnimeCount]);
				}
			}
			//삭제
		}
		else if (!Boss->ATTACKREADY && Boss->ATTACK)
		{ //보스 공격 해제
			//if (Boss->Idlecount >= 8)Boss->Idlecount = 0;
			BossMob(BackMemDC, Boss, &bossImage->BossAttackEnd[Boss->Idlecount]);
		}
	}
	else
	{
		BossMob(BackMemDC, Boss, &bossImage->Death[Boss->Idlecount]); //본체
		mdc = CreateCompatibleDC(BackMemDC);
		SelectObject(mdc, bossImage->Death[40]);
		TransparentBlt(BackMemDC, Boss->rect.left - 150, Boss->rect.top + 160, bossImage->Death[40].GetWidth() - 100, bossImage->Death[40].GetHeight() + 100
			, mdc, 0, 0, bossImage->Death[40].GetWidth(), bossImage->Death[40].GetHeight(), RGB(255, 0, 255));
		DeleteDC(mdc);
	}

	//추가
	for (int i = 0; i < FIRENUM; ++i)
	{
		if (!Boss->attackFire[i].exist)
		{
			break;
		}

		BossAttackAnimation(BackMemDC, &Boss->attackFire[i].rect, &bossImage->AttackFire[Boss->attackFire[i].animationNum]);
	}

	for (int i = 0; i < METEORNUM; ++i)
	{
		if (!Boss->attackMeteor[i].exist)
		{
			continue;
		}

		if (!Boss->attackMeteor[i].extinction)
		{
			BossAttackAnimation(BackMemDC, &Boss->attackMeteor[i].rect, &bossImage->AttackMeteor[11 + Boss->attackMeteor[i].animationNum]);
		}
		else
		{
			BossAttackAnimation(BackMemDC, &Boss->attackMeteor[i].rect, &bossImage->MeteorExtinction[Boss->attackMeteor[i].animationNum]);
		}
	}
}

void CREATESTAGE5(BossMonster* Boss, BOSSCIMAGE* bossImage, CImage BossGround[], RECT rect)
{
	TCHAR str[150];
	for (int i = 0; i < 8; ++i) {
		wsprintf(str, L"리소스파일/보스/peashot attack transform/dragon_peashot_trans_00%02d.png", i + 1);
		//Boss->BossAttackReady[i].Load(str);
		bossImage->BossAttackReady[i].Load(str);
	}

	for (int i = 0; i < 16; ++i) {
		wsprintf(str, L"리소스파일/보스/peashot attack shoot/dragon_peashot_idle_00%02d.png", i + 1);
		//Boss->BossAttack[i].Load(str);
		bossImage->BossAttack[i].Load(str);
	}

	for (int i = 0; i < 8; ++i) {
		wsprintf(str, L"리소스파일/보스/peashot attack turn back to normal/dragon_peashot_trans_back_00%02d.png", i + 1);
		//Boss->BossAttackEnd[i].Load(str);
		bossImage->BossAttackEnd[i].Load(str);
	}

	for (int i = 0; i < 20; ++i)
	{
		if (i < 8) {
			wsprintf(str, L"리소스파일/보스/sfx/green ring/attack (%d).png", i + 1);
		}
		else if (i < 20) {
			wsprintf(str, L"리소스파일/보스/tail/tail (%d).png", i + 1 - 8);
		}
		//Boss->AttackTail[i].Load(str);
		bossImage->AttackTail[i].Load(str);
	}

	for (int i = 0; i < 19; ++i) 
	{
		if (i < 11) {
			wsprintf(str, L"리소스파일/보스/sfx/peashot attack eyes/attack (%d).png", i + 1);
		}
		else if (i < 19) {
			wsprintf(str, L"리소스파일/보스/sfx/meteor/meteor/meteor (%d).png", i + 1 - 11);
		}
		//Boss->AttackMeteor[i].Load(str);
		bossImage->AttackMeteor[i].Load(str);
	}

	for (int i = 0; i < 35; ++i)
	{
		wsprintf(str, L"리소스파일/보스/sfx/meteor/meteor smoke/meteor_smoke_00%02d.png", i + 1);
		//Boss->MeteorExtinction[i].Load(str);
		bossImage->MeteorExtinction[i].Load(str);
	}
	for (int i = 0; i < 40; ++i) 
	{
		wsprintf(str, L"리소스파일/보스/dead/dragon_intro_00%02d.png", i + 1);
		//Boss->Death[i].Load(str);
		bossImage->Death[i].Load(str);
	}
	//Boss->Death[40].Load(L"리소스파일/보스/dead/1.png");
	bossImage->Death[40].Load(L"리소스파일/보스/dead/1.png");

	//보스배경
	BossGround[0].Load(TEXT("리소스파일/보스 배경/Final Ground/sky/Boss Skyjpg.jpg"));
	//보스 타워
	BossGround[1].Load(TEXT("리소스파일/보스 배경/Final Ground/Tower/BosspngBossTower.png"));

	//보스 땅 레이아웃
	BossGround[2].Load(TEXT("리소스파일/보스 배경/Final Ground/cloud/pngBossFloor.png"));

	for (int i = 0; i < 16; ++i)
	{
		wsprintf(str, L"리소스파일/보스/Idle/dragon_idle_00%02d.png", i + 1);
		//Boss->BossIDLE[i].Load(str);
		bossImage->BossIDLE[i].Load(str);
	}

	for (int i = 0; i < 8; ++i)
	{
		wsprintf(str, L"리소스파일/보스/sfx/meteor/meteor/firewall (%d).png", i + 1);
		//Boss->AttackFire[i].Load(str);
		bossImage->AttackFire[i].Load(str);
	}

	Boss->rect = { rect.right - 350,rect.top + 50,rect.right,rect.bottom }; //보스 크기

}

void BossBackground(HDC BackMemDC, RECT rect, CImage BossGround[])
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	BossGround[0].StretchBlt(BackMemDC, 0, 0, rect.right, rect.bottom, 0, 0, BossGround[0].GetWidth(), BossGround[0].GetHeight());

	(HBITMAP)SelectObject(ObjectDC, BossGround[1]);
	TransparentBlt(BackMemDC, rect.right - 400, 0, 300, rect.bottom, ObjectDC, 0, 0, BossGround[1].GetWidth()
		, BossGround[1].GetHeight(), RGB(255, 255, 255));

	(HBITMAP)SelectObject(ObjectDC, BossGround[2]);
	TransparentBlt(BackMemDC, -100, (rect.bottom / 5) * 3, rect.right + 200, (rect.bottom / 5) * 2
		, ObjectDC, 0, 0, BossGround[2].GetWidth(), BossGround[2].GetHeight(), RGB(255, 255, 255));

	DeleteDC(ObjectDC);
}

void BossMob(HDC BackMemDC, BossMonster* Boss, CImage* hBoss)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *hBoss);
	
	Boss->rect = { Boss->rect.left,Boss->rect.top,Boss->rect.left + (hBoss->GetWidth() / 4) * 3, Boss->rect.top + (hBoss->GetHeight() / 5) * 4 }; //보스 크기
	
	TransparentBlt(BackMemDC, Boss->rect.left, Boss->rect.top, (hBoss->GetWidth() / 4) * 3, (hBoss->GetHeight() / 5) * 4, ObjectDC, 0, 0, hBoss->GetWidth(), hBoss->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void BossAttackAnimation(HDC BackMemDC, RECT* AttackRect, CImage* Attack)
{
	HDC ObjectDC;
	ObjectDC = CreateCompatibleDC(BackMemDC);

	SelectObject(ObjectDC, *Attack);
	*AttackRect = { AttackRect->left, AttackRect->top, AttackRect->left + Attack->GetWidth() / 2,
		AttackRect->top + Attack->GetHeight() / 2 };
	TransparentBlt(BackMemDC, AttackRect->left, AttackRect->top, Attack->GetWidth() / 2, Attack->GetHeight() / 2
		, ObjectDC, 0, 0, Attack->GetWidth(), Attack->GetHeight(), RGB(255, 0, 255));

	DeleteDC(ObjectDC);
}

void BossAttackTail(HWND hwnd, RECT rect, MainCharacter* mainCharacter, BossMonster* Boss, BOSSCIMAGE* bossImage, MainState* oldState, int* oldAnimationNum, int* invincibleTime)
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
			Boss->AttackTailrect.right = Boss->AttackTailrect.left + bossImage->AttackTail[Boss->AttackTailAnimeCount].GetWidth() / 3;
			Boss->AttackTailrect.bottom = Boss->AttackTailrect.top + bossImage->AttackTail[Boss->AttackTailAnimeCount].GetHeight() / 2;
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

void BossAttackMeteor(RECT rect, BossMonster* Boss, MainCharacter* mainCharacter, MainState* oldState, int* oldAnimationNum, int* invincibleTime)
{

	if (Boss->HP <= 0) 
	{
		Boss->rect.left += 3;
	}

	RECT temp;
	RECT hitBox = { (Boss->rect.left + Boss->rect.right) / 2 - 100, Boss->rect.top, (Boss->rect.left + Boss->rect.right) / 2 + 100,Boss->rect.bottom };

	// 총알과 보스 충돌
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
					Boss->Idle = BOSSDEADANI;
					Boss->Idlecount = 0;
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

	// 보스 몸체와 플레이어 충돌
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
		if (IntersectRect(&temp, &mainCharacter->info.Pos, &Boss->attackFire[i].rect) && *invincibleTime == 0 && mainCharacter->info.heart > 0) 
		{
			mainCharacter->info.heart--;

			*oldState = mainCharacter->info.state;
			*oldAnimationNum = mainCharacter->info.animationNum;
			mainCharacter->info.state = MainState::HIT;
			mainCharacter->info.energy = -1;
			mainCharacter->info.animationNum = 0;
			*invincibleTime = 100;
		}		
	}

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

		if (IntersectRect(&temp, &mainCharacter->info.Pos, &Boss->attackMeteor[i].rect) && *invincibleTime == 0 && mainCharacter->info.heart > 0) //플레이어와 충돌
		{
			//구체만 접촉했을때 폭발 애니메이션 시작. 불기둥은 접촉하면 데미지만 받음.
			
			Boss->attackMeteor[i].extinction = TRUE; 
			Boss->attackMeteor[i].animationNum = 0;

			if (mainCharacter->info.heart == 1)
			{
				mainCharacter->info.heart = 0;
			}
			else
			{
				mainCharacter->info.heart -= 2;
			}

			*oldState = mainCharacter->info.state;
			*oldAnimationNum = mainCharacter->info.animationNum;
			mainCharacter->info.state = MainState::HIT;
			mainCharacter->info.energy = -1;
			mainCharacter->info.animationNum = 0;
			*invincibleTime = 100;
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
				else if(Boss->tailAnimationLimit == BOSSTAILANI)	// 꼬리가 보이는 상태
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
		boss.attackFire[i].rect = { rect.left + ((((-i - 1) * -1) - 1) * 200), rect.top - 100, rect.left, rect.top};
	}
}

//추가
double getradian(int num)
{
	return num * (PI / 180);
}