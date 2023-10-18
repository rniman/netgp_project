
#include "character.h"

void CreateNPC(HINSTANCE g_hInst, NPC* npc)
{
	TCHAR temp[30];
	for (int i = 0; i < NPCANI; ++i) {
		wsprintf(temp, L"Stage1/NPC/NPC%02d.bmp", i + 1);
		npc->NPCBitmap[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(npc->NPCBitmap[i], sizeof(BITMAP), &npc->NPCBitData[i]);
	}
	npc->animationNum = 5;

	npc->textBox = (HBITMAP)LoadImage(g_hInst,L"Stage1/NPC/textBox.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	npc->Pos.left = 550;
	npc->Pos.right = npc->Pos.left + NPCWIDTH;
	npc->Pos.bottom = 485;
	npc->Pos.top = npc->Pos.bottom - NPCHEIGT;

}

void DeleteNPC(NPC* npc) 
{
	for (int i = 0; i < NPCANI; ++i) {
		if (npc->NPCBitmap[i] != NULL) DeleteObject(npc->NPCBitmap[i]);
	}
}

void PaintNPC(HDC backMemDC, HDC ObjectDC, NPC* npc)
{
	SetStretchBltMode(backMemDC, HALFTONE);
	SelectObject(ObjectDC, npc->NPCBitmap[npc->animationNum]);
	TransparentBlt(backMemDC,
		npc->Pos.right - npc->NPCBitData[npc->animationNum].bmWidth / 2,
		npc->Pos.bottom - npc->NPCBitData[npc->animationNum].bmHeight / 2,
		npc->NPCBitData[npc->animationNum].bmWidth / 2,
		npc->NPCBitData[npc->animationNum].bmHeight / 2,
		ObjectDC, 0, 0, npc->NPCBitData[npc->animationNum].bmWidth,
		npc->NPCBitData[npc->animationNum].bmHeight,
		RGB(255, 0, 255));
	
	if (npc->animationNum >= 0 && npc->animationNum <= 4) {
		SelectObject(ObjectDC, npc->textBox);
		TransparentBlt(backMemDC, 200, 200, 350, 160,
			ObjectDC, 0, 0, 600, 181, RGB(255, 0, 255));

		SetBkMode(backMemDC, TRANSPARENT);
		//RECT temp = { 250,200,550,390 };
		TextOut(backMemDC, 230, 230, L"흉악한 드래곤이 우리들의", 13);
		TextOut(backMemDC, 230, 260, L"식량 저장고인 동쪽 성을 점령했어!", 19);
		TextOut(backMemDC, 230, 290, L"드래곤을 물리치고 마을 평화를 되찾아줘!", 22);
		TextOut(backMemDC, 230, 320, L"오른쪽 포탈을 타고 가면 되!", 16);
		//DrawText(backMemDC, L"적을 무찔러 줘! 오른쪽 포탈을 타고 가면 되", 27, &temp,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		SetBkMode(backMemDC, OPAQUE);
	}
}

BOOL CheckNPCDlg(MainCharacter mainCharacter,NPC* npc, int* dlgTime)
{
	RECT temp = { npc->Pos.left - 30,npc->Pos.top - 30,npc->Pos.right + 30,npc->Pos.bottom + 50 };
	POINT mainPos = { (mainCharacter.Pos.right + mainCharacter.Pos.left) / 2,(mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 };
	if (PtInRect(&temp, mainPos) && *dlgTime == 0) {
		npc->animationNum = 0;
		*dlgTime = 1;
		return TRUE;
	}
	return FALSE;
}