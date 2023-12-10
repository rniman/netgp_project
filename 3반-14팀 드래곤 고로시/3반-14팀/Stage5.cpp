#include "ClientCharacter.h"

void PaintBoss(HDC BackMemDC, RECT rect, BossMonster* Boss, BossCImage* bossImage, CImage BossGround[])
{
	HDC mdc;
	BossBackground(BackMemDC, rect, BossGround); //�������� �������
	if (Boss->HP > 0) 
	{
		if (!Boss->ATTACKREADY && !Boss->ATTACK)
		{ //���� �⺻ ����
			BossMob(BackMemDC, Boss, &bossImage->BossIDLE[Boss->Idlecount]);
		}
		else if (Boss->ATTACKREADY && !Boss->ATTACK)
		{ //���� ���� �غ�
			BossMob(BackMemDC, Boss, &bossImage->BossAttackReady[Boss->Idlecount]);
		}
		else if (Boss->ATTACKREADY && Boss->ATTACK)
		{ //���� ���� ����
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
			//����
		}
		else if (!Boss->ATTACKREADY && Boss->ATTACK)
		{
			BossMob(BackMemDC, Boss, &bossImage->BossAttackEnd[Boss->Idlecount]);
		}
	}
	else
	{
		BossMob(BackMemDC, Boss, &bossImage->Death[Boss->Idlecount]); //��ü
		mdc = CreateCompatibleDC(BackMemDC);
		SelectObject(mdc, bossImage->Death[40]);
		TransparentBlt(BackMemDC, Boss->rect.left - 150, Boss->rect.top + 160, bossImage->Death[40].GetWidth() - 100, bossImage->Death[40].GetHeight() + 100
			, mdc, 0, 0, bossImage->Death[40].GetWidth(), bossImage->Death[40].GetHeight(), RGB(255, 0, 255));
		DeleteDC(mdc);
	}

	//�߰�
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

void CreateBossAndStage(BossMonster* Boss, BossCImage* bossImage, CImage BossGround[], RECT rect)
{
	TCHAR str[150];
	for (int i = 0; i < 8; ++i) {
		wsprintf(str, L"���ҽ�����/����/peashot attack transform/dragon_peashot_trans_00%02d.png", i + 1);
		bossImage->BossAttackReady[i].Load(str);
	}

	for (int i = 0; i < 16; ++i) {
		wsprintf(str, L"���ҽ�����/����/peashot attack shoot/dragon_peashot_idle_00%02d.png", i + 1);
		bossImage->BossAttack[i].Load(str);
	}

	for (int i = 0; i < 8; ++i) {
		wsprintf(str, L"���ҽ�����/����/peashot attack turn back to normal/dragon_peashot_trans_back_00%02d.png", i + 1);
		bossImage->BossAttackEnd[i].Load(str);
	}

	for (int i = 0; i < 20; ++i)
	{
		if (i < 8) {
			wsprintf(str, L"���ҽ�����/����/sfx/green ring/attack (%d).png", i + 1);
		}
		else if (i < 20) {
			wsprintf(str, L"���ҽ�����/����/tail/tail (%d).png", i + 1 - 8);
		}
		bossImage->AttackTail[i].Load(str);
	}

	for (int i = 0; i < 19; ++i) 
	{
		if (i < 11) {
			wsprintf(str, L"���ҽ�����/����/sfx/peashot attack eyes/attack (%d).png", i + 1);
		}
		else if (i < 19) {
			wsprintf(str, L"���ҽ�����/����/sfx/meteor/meteor/meteor (%d).png", i + 1 - 11);
		}
		bossImage->AttackMeteor[i].Load(str);
	}

	for (int i = 0; i < 35; ++i)
	{
		wsprintf(str, L"���ҽ�����/����/sfx/meteor/meteor smoke/meteor_smoke_00%02d.png", i + 1);
		bossImage->MeteorExtinction[i].Load(str);
	}
	for (int i = 0; i < 40; ++i) 
	{
		wsprintf(str, L"���ҽ�����/����/dead/dragon_intro_00%02d.png", i + 1);
		bossImage->Death[i].Load(str);
	}
	bossImage->Death[40].Load(L"���ҽ�����/����/dead/1.png");

	//�������
	BossGround[0].Load(TEXT("���ҽ�����/���� ���/Final Ground/sky/Boss Skyjpg.jpg"));
	//���� Ÿ��
	BossGround[1].Load(TEXT("���ҽ�����/���� ���/Final Ground/Tower/BosspngBossTower.png"));

	//���� �� ���̾ƿ�
	BossGround[2].Load(TEXT("���ҽ�����/���� ���/Final Ground/cloud/pngBossFloor.png"));

	for (int i = 0; i < 16; ++i)
	{
		wsprintf(str, L"���ҽ�����/����/Idle/dragon_idle_00%02d.png", i + 1);
		bossImage->BossIDLE[i].Load(str);
	}

	for (int i = 0; i < 8; ++i)
	{
		wsprintf(str, L"���ҽ�����/����/sfx/meteor/meteor/firewall (%d).png", i + 1);
		bossImage->AttackFire[i].Load(str);
	}

	Boss->rect = { rect.right - 350,rect.top + 50, rect.right, rect.bottom }; //���� ũ��
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
	
	Boss->rect = { Boss->rect.left,Boss->rect.top,Boss->rect.left + (hBoss->GetWidth() / 4) * 3, Boss->rect.top + (hBoss->GetHeight() / 5) * 4 }; //���� ũ��
	
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
