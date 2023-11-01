
#include "character.h"

void LoadBullet(BULLETBITMAP* bulletBitmap,HINSTANCE g_hInst)
{
	TCHAR temp[40];

	bulletBitmap->LOOPBITMAP = (HBITMAP)LoadImage(g_hInst, L"CUPHEAD/bullet/bullet1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(bulletBitmap->LOOPBITMAP, sizeof(BITMAP), &bulletBitmap->LOOPBitData);
	
	bulletBitmap->EXBITMAP = (HBITMAP)LoadImage(g_hInst, L"CUPHEAD/exbullet/EX_bullet1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(bulletBitmap->EXBITMAP, sizeof(BITMAP), &bulletBitmap->EXBitData);

	for (int i = 0; i < 6; ++i) {
		wsprintf(temp, L"CUPHEAD/bullet/death_bullet%d.bmp", i + 1);
		bulletBitmap->DEATHLOOPBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(bulletBitmap->DEATHLOOPBITMAP[i], sizeof(BITMAP), &bulletBitmap->DEATHLOOPBitData[i]);
	}
	for (int i = 0; i < 9; ++i) {
		wsprintf(temp, L"CUPHEAD/exbullet/death_EX_bullet%d.bmp", i + 1);
		bulletBitmap->DEATHEXBITMAP[i] = (HBITMAP)LoadImage(g_hInst, temp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(bulletBitmap->DEATHEXBITMAP[i], sizeof(BITMAP), &bulletBitmap->DEATHEXBitData[i]);
	}

}
void DeleteBitBullet(BULLETBITMAP* bulletBitmap)
{
	DeleteObject(bulletBitmap->LOOPBITMAP);
	DeleteObject(bulletBitmap->EXBITMAP);
	for (int i = 0; i < 6; ++i)
		DeleteObject(bulletBitmap->DEATHLOOPBITMAP[i]);
	for (int i = 0; i < 9; ++i)
		DeleteObject(bulletBitmap->DEATHEXBITMAP[i]);
}

void PaintBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap)
{
	BULLETNODE* cus = bullet->head;
	while (cus != NULL) {
		if (!cus->data.exist) {
			cus = cus->link;
			continue;
		}

		if (cus->data.direction) {
			if (cus->data.EX) {
				SelectObject(ObjectDC, bulletBitmap.EXBITMAP);
				TransparentBlt(backMemDC, cus->data.Pos.left,
					cus->data.Pos.top,
					bulletBitmap.EXBitData.bmWidth/2,
					bulletBitmap.EXBitData.bmHeight/2,
					ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
			else {
				SelectObject(ObjectDC, bulletBitmap.LOOPBITMAP);
				TransparentBlt(backMemDC, cus->data.Pos.left,
					cus->data.Pos.top,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					RGB(255, 0, 255)
				);
			}
		}
		else {
			HBITMAP temp;
			HDC tempDC;
			if (cus->data.EX) {
				tempDC = CreateCompatibleDC(ObjectDC);
				SelectObject(tempDC, bulletBitmap.EXBITMAP);

				temp = CreateCompatibleBitmap(backMemDC, bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight);

				SelectObject(ObjectDC, temp);

				StretchBlt(ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					tempDC,
					bulletBitmap.EXBitData.bmWidth - 1, 0,
					-bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					SRCCOPY);

				TransparentBlt(backMemDC, cus->data.Pos.left,
					cus->data.Pos.top,
					bulletBitmap.EXBitData.bmWidth/2,
					bulletBitmap.EXBitData.bmHeight/2,
					ObjectDC, 0, 0,
					bulletBitmap.EXBitData.bmWidth,
					bulletBitmap.EXBitData.bmHeight,
					RGB(255, 0, 255)
				);
				DeleteDC(tempDC);
				DeleteObject(temp);
			}
			else {
				tempDC = CreateCompatibleDC(ObjectDC);
				SelectObject(tempDC, bulletBitmap.LOOPBITMAP);
				temp = CreateBitmap(bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight, 1, 24, NULL);
				SelectObject(ObjectDC, temp);

				StretchBlt(ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					tempDC,
					bulletBitmap.LOOPBitData.bmWidth - 1, 0,
					-bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					SRCCOPY);

				TransparentBlt(backMemDC, cus->data.Pos.left,
					cus->data.Pos.top,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					ObjectDC, 0, 0,
					bulletBitmap.LOOPBitData.bmWidth,
					bulletBitmap.LOOPBitData.bmHeight,
					RGB(255, 0, 255)
				);
				DeleteDC(tempDC);
				DeleteObject(temp);
			}
		}
		cus = cus->link;
	}
}

void PaintDeathBullet(HDC backMemDC, HDC ObjectDC, BLinkedList* bullet, BULLETBITMAP bulletBitmap)
{
	BULLETNODE* cus = bullet->head;
	while (cus != NULL) {
		if (cus->data.exist) {
			cus = cus->link;
			continue;
		}
		
		if (cus->data.EX) {
			SelectObject(ObjectDC, bulletBitmap.DEATHEXBITMAP[cus->data.animationNum]);
			TransparentBlt(backMemDC,
				(cus->data.Pos.right + cus->data.Pos.left) / 2 - bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmWidth / 6,
				cus->data.Pos.bottom - bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmHeight / 3,
				bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmWidth / 3,
				bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmHeight / 3,
				ObjectDC, 0, 0,
				bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmWidth,
				bulletBitmap.DEATHEXBitData[cus->data.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		else {
			SelectObject(ObjectDC, bulletBitmap.DEATHLOOPBITMAP[cus->data.animationNum]);
			TransparentBlt(backMemDC,
				(cus->data.Pos.right + cus->data.Pos.left)/2 - bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmWidth / 4,
				cus->data.Pos.bottom - bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmHeight / 4,
				bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmWidth / 2,
				bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmHeight / 2,
				ObjectDC, 0, 0,
				bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmWidth,
				bulletBitmap.DEATHLOOPBitData[cus->data.animationNum].bmHeight,
				RGB(255, 0, 255)
			);
		}
		cus = cus->link;
	}
}

BLinkedList* CreateBList(void)
{
	BLinkedList* temp = (BLinkedList*)malloc(sizeof(BLinkedList));
	temp->head = NULL;

	return temp;
}
void CreateBullet(BLinkedList* bullet, MainCharacter mainCharacter, BULLETBITMAP bulletBit)
{
	BULLETNODE* temp = (BULLETNODE*)malloc(sizeof(BULLETNODE));
	temp->data.direction = mainCharacter.direction;
	temp->data.exist = TRUE;
	temp->data.animationNum = 0;
	
	if (mainCharacter.state != 5)
		temp->data.EX = FALSE;
	else
		temp->data.EX = TRUE;

	if (temp->data.EX) {
		temp->data.deathTime = 45;
		if (temp->data.direction) {
			temp->data.Pos.left = mainCharacter.Pos.right - 40;
			temp->data.Pos.right = temp->data.Pos.left + bulletBit.EXBitData.bmWidth / 2;

			temp->data.hitBox.left = temp->data.Pos.left + 158 / 2;
			temp->data.hitBox.right = temp->data.hitBox.left + EXBULWIDTH;
		}
		else {
			temp->data.Pos.right = mainCharacter.Pos.left + 40;
			temp->data.Pos.left = temp->data.Pos.right - bulletBit.EXBitData.bmWidth / 2;

			temp->data.hitBox.right = temp->data.Pos.right - 158 / 2;
			temp->data.hitBox.left = temp->data.hitBox.right - EXBULWIDTH;
		}
		temp->data.Pos.top = (mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 - bulletBit.EXBitData.bmHeight / 4;
		temp->data.Pos.bottom = (mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 + bulletBit.EXBitData.bmHeight / 4;

		temp->data.hitBox.top = temp->data.Pos.top + 9;
		temp->data.hitBox.bottom = temp->data.hitBox.top + EXBULHEIGHT;
	}
	else {
		temp->data.deathTime = 45;
		if (temp->data.direction) {
			temp->data.Pos.left = mainCharacter.Pos.right - 20;
			temp->data.Pos.right = temp->data.Pos.left + bulletBit.LOOPBitData.bmWidth;

			temp->data.hitBox.left = temp->data.Pos.left + 11;
			temp->data.hitBox.right = temp->data.hitBox.left + BULWIDTH;
		}
		else {
			temp->data.Pos.right = mainCharacter.Pos.left + 20;
			temp->data.Pos.left = temp->data.Pos.right - bulletBit.LOOPBitData.bmWidth;

			temp->data.hitBox.right = temp->data.Pos.right - 11;
			temp->data.hitBox.left = temp->data.hitBox.right - BULWIDTH;
		}
		temp->data.Pos.top = (mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 - bulletBit.LOOPBitData.bmHeight / 2;
		temp->data.Pos.bottom = (mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 + bulletBit.LOOPBitData.bmHeight / 2;

		temp->data.hitBox.top = temp->data.Pos.top + 11;
		temp->data.hitBox.bottom = temp->data.hitBox.top + BULHEIGHT;
	}
	temp->link = bullet->head;
	bullet->head = temp;
}

void DeleteBullet(BLinkedList* bullet)
{
	BULLETNODE* cus = bullet->head;
	BULLETNODE* prev = NULL;
	while (cus != NULL) {
		if (cus->data.exist == FALSE && cus->data.deathTime<= 0) {
			if (prev == NULL) {
				bullet->head = cus->link;
				free(cus);
			}
			else {
				prev->link = cus->link;
				free(cus);
			}
			break;
		}
		prev = cus;
		cus = cus->link;
	}
}

void MoveBullet(BLinkedList* bullet , RECT rect)
{
	BULLETNODE* cus = bullet->head;
	while (cus != NULL) {
		if (!cus->data.exist) {
			cus = cus->link;
			continue;
		}

		if (cus->data.direction) {
			cus->data.Pos.left += 20;
			cus->data.Pos.right += 20;

			cus->data.hitBox.left += 20;
			cus->data.hitBox.right += 20;

			if (cus->data.hitBox.left >= rect.right)
				cus->data.exist = FALSE;
		}
		else {
			cus->data.Pos.left -= 20;
			cus->data.Pos.right -= 20;

			cus->data.hitBox.left -= 20;
			cus->data.hitBox.right -= 20;

			if (cus->data.hitBox.right <= rect.left)
				cus->data.exist = FALSE;
		}
		cus = cus->link;
	}
}

void DeathBullet(BLinkedList* bullet)
{
	BULLETNODE* cus = bullet->head;
	while (cus != NULL) {
		if (cus->data.exist) {
			cus = cus->link;
			continue;
		}
			
		cus->data.deathTime--;
		if (cus->data.deathTime % 5 == 0)cus->data.animationNum++;

		cus = cus->link;
	}
}