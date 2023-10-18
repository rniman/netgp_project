#include "character.h"

void LoadStage1(HINSTANCE g_hInst, VILLAGE* village, CLOUD* cloud)
{
	village->sky = (HBITMAP)LoadImage(g_hInst, L"Stage1/BackGround/villageSky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(village->sky, sizeof(BITMAP), &village->skyData);
	village->land = (HBITMAP)LoadImage(g_hInst, L"Stage1/BackGround/village.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(village->land, sizeof(BITMAP), &village->landData);
	village->house = (HBITMAP)LoadImage(g_hInst, L"Stage1/BackGround/cupHouse.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(village->house, sizeof(BITMAP), &village->houseData);

	cloud->cloudBITMAP[0] = (HBITMAP)LoadImage(g_hInst, L"Stage1/BackGround/cloud01.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(cloud->cloudBITMAP[0], sizeof(BITMAP), &cloud->cloudData[0]);

	cloud->cloudBITMAP[1] = (HBITMAP)LoadImage(g_hInst, L"Stage1/BackGround/cloud02.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(cloud->cloudBITMAP[1], sizeof(BITMAP), &cloud->cloudData[1]);
	
}

void LoadPotal(CImage potal[])
{
	potal[0].Load(L"Stage1/Potal/potal1.png");
	potal[1].Load(L"Stage1/Potal/potal2.png");
	potal[2].Load(L"Stage1/Potal/potal3.png");
	potal[3].Load(L"Stage1/Potal/potal4.png");
}

void PaintStage1(HINSTANCE g_hInst, HDC backMemDC, HDC ObjectDC, VILLAGE village,CLOUD cloud, RECT rect)
{
	SelectObject(ObjectDC, village.sky);
	StretchBlt(backMemDC, 0, 0, rect.right, rect.bottom, ObjectDC, 0, 0, rect.right, village.skyData.bmHeight, SRCCOPY);

	SelectObject(ObjectDC, cloud.cloudBITMAP[0]);
	TransparentBlt(backMemDC, 200, 100, cloud.cloudData[0].bmWidth, cloud.cloudData[0].bmHeight,
		ObjectDC, 0, 0, cloud.cloudData[0].bmWidth, cloud.cloudData[0].bmHeight, RGB(255, 0, 255));

	SelectObject(ObjectDC, cloud.cloudBITMAP[0]);
	TransparentBlt(backMemDC, 750, 200, cloud.cloudData[0].bmWidth, cloud.cloudData[0].bmHeight,
		ObjectDC, 0, 0, cloud.cloudData[0].bmWidth, cloud.cloudData[0].bmHeight, RGB(255, 0, 255));

	SelectObject(ObjectDC, cloud.cloudBITMAP[1]);
	TransparentBlt(backMemDC, 25, 10, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight,
		ObjectDC, 0, 0, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight, RGB(255, 0, 255));

	SelectObject(ObjectDC, cloud.cloudBITMAP[1]);
	TransparentBlt(backMemDC, 500, 130, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight,
		ObjectDC, 0, 0, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight, RGB(255, 0, 255));

	SelectObject(ObjectDC, cloud.cloudBITMAP[1]);
	TransparentBlt(backMemDC, 670, 60, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight,
		ObjectDC, 0, 0, cloud.cloudData[1].bmWidth, cloud.cloudData[1].bmHeight, RGB(255, 0, 255));

	SelectObject(ObjectDC, village.land);
	TransparentBlt(backMemDC, -10, 300, rect.right+ 30, village.landData.bmHeight,
		ObjectDC, 0, 0, village.landData.bmWidth, village.landData.bmHeight, RGB(255, 0, 255));	

	SelectObject(ObjectDC, village.house);
	TransparentBlt(backMemDC, 600, 180, village.houseData.bmWidth, village.houseData.bmHeight,
		ObjectDC, 0, 0, village.houseData.bmWidth, village.houseData.bmHeight, RGB(255, 0, 255));

}

void PaintPotal(HDC backMemDC, CImage* potal)
{
	potal->TransparentBlt(backMemDC, 900, 300, potal->GetWidth()/3, potal->GetHeight()/3,
		0, 0, potal->GetWidth(), potal->GetHeight(), RGB(255, 0, 255));
}

void DeleteStage1(VILLAGE* village, CLOUD* cloud, CImage potal[])
{
	DeleteObject(village->house);
	DeleteObject(village->sky);
	DeleteObject(village->land);
	DeleteObject(cloud->cloudBITMAP[0]);
	DeleteObject(cloud->cloudBITMAP[1]);
	//potal[0].Destroy();
	//potal[1].Destroy();
	//potal[2].Destroy();
	//potal[3].Destroy();
}

BOOL CheckPotal(MainCharacter mainCharacter)
{
	POINT temp = { (mainCharacter.Pos.left + mainCharacter.Pos.right) / 2,(mainCharacter.Pos.top + mainCharacter.Pos.bottom) / 2 };
	RECT potal = { 900,300,900 + 250 / 3,300 + 592 / 3 };
	if (PtInRect(&potal, temp))
		return TRUE;
	else
		return FALSE;
}