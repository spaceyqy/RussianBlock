#include "RussiaBlock.h"
#include<time.h>
#include<cmath>

GameArea gameArea[GAMEBOTWALL+1][GAMERIGHTWALL+1];//��Ϸ������
int gameAreaTopLine;//��Ϸ����������к�

/*
�����������˹���飬������ʽ���£�
1.�趨һ������ĳ�ʼλ��Ϊ(4,-1)�����ڻ�û�н�����Ϸ���򴦲���һ�����飬���������ĳ�ʼλ�ü���һ��ƫ����(offsetX,offsetY)�õ��·��������ֵ
2.����ƫ����Ϊ �£�0,1�� ��(0,-1)  ��(-1,0) ��(1,0)
3.Ϊ���㲻����һ������λ������һ������λ���غϣ�ֻ��Ҫ������һ������ƫ������������һ��ƫ����ȡ������
*/
void RandomCreateRussiaBlock(RussiaBlock &russiaBlock)
{
	russiaBlock.obj[0].x = russiaBlock.obj[3].x = 4;//�����ʼֵ
	russiaBlock.obj[0].y = russiaBlock.obj[3].y = -1;

	int offsetX = -2, offsetY = -2;//��ʼ��ƫ����
	for (int i = 1; i <= 2; i++){
		RandomizeOffset(offsetX, offsetY);
		russiaBlock.obj[i].x = russiaBlock.obj[i - 1].x + offsetX;
		russiaBlock.obj[i].y = russiaBlock.obj[i - 1].y + offsetY;
		offsetX = -offsetX;
		offsetY = -offsetY;
	}

	UpdateLimit(russiaBlock);//�������½�
	//��ǰ�����������γ�"1"��"-"��ʱ,��0.5�����γ�"T"�ַ���
	static bool isFirst = true;
	if (isFirst){
		srand((unsigned)time(NULL));
		isFirst = false;
	}
	int randNum = rand() % 100;//�γ�0~99֮�����
	if (randNum < 50 && (russiaBlock.bottomBorder - russiaBlock.topBorder == 2 || russiaBlock.rightBorder - russiaBlock.leftBorder == 2))
	{
		if (abs(russiaBlock.obj[0].y - russiaBlock.obj[1].y) == 1 && abs(russiaBlock.obj[1].y - russiaBlock.obj[2].y) == 1)//Ϊ����1�֣�������offsetX��offsetYֻ����(-1,0)��(1,0)
		{
			do{
				RandomizeOffset(offsetX, offsetY);
			} while (abs(offsetX) != 1);
		}
		else//Ϊ���1�֣�������offsetX��offsetYֻ����(0,-1)��(0,1)
		{
			do{
				RandomizeOffset(offsetX, offsetY);
			} while (abs(offsetY) != 1);
		}
		russiaBlock.obj[3].x = russiaBlock.obj[1].x + offsetX;//ʹ���м�Ŀ����T�ο�
		russiaBlock.obj[3].y = russiaBlock.obj[1].y + offsetY;
	}
	else
	{
		RandomizeOffset(offsetX, offsetY);
		russiaBlock.obj[3].x = russiaBlock.obj[2].x + offsetX;//ʹ�õ�����������Ŀ�
		russiaBlock.obj[3].y = russiaBlock.obj[2].y + offsetY;
	}
	russiaBlock.fillingColor = GetRandomColor();
	russiaBlock.bdrColor = GetRandomColor();
	UpdateLimit(russiaBlock);
	//�γ�������ͳһ�������λbottomBdr��-1��ƽ��Ϊ������������׼��
	MoveObject(russiaBlock, 0, -1 - russiaBlock.bottomBorder);
}
void RandomizeOffset(int &offsetX, int &offsetY)
{
	static bool isFirst = true;
	if (isFirst){
		srand((unsigned)time(NULL));
		isFirst = false;
	}
	int offsetArray[] = { 0, 1, 0, -1, -1, 0, 1, 0 };
	int oldX = offsetX;
	int oldY = offsetY;
	do{
		offsetX = offsetArray[(rand() % 4) * 2];
		offsetY = offsetArray[offsetX + 1];
	} while (oldX == offsetX && oldY == offsetY);
}
//�������½磬����ȡ��С������ȡ���
void UpdateLimit(RussiaBlock &russiaBlock)
{
	russiaBlock.leftBorder = russiaBlock.obj[0].x;
	russiaBlock.rightBorder = russiaBlock.obj[0].x;
	russiaBlock.topBorder = russiaBlock.obj[0].y;
	russiaBlock.bottomBorder = russiaBlock.obj[0].y;
	for (int i = 1; i <= 3; i++)
	{
		if (russiaBlock.leftBorder > russiaBlock.obj[i].x)
			russiaBlock.leftBorder = russiaBlock.obj[i].x;
		if (russiaBlock.rightBorder < russiaBlock.obj[i].x)
			russiaBlock.rightBorder = russiaBlock.obj[i].x;
		if (russiaBlock.topBorder > russiaBlock.obj[i].y)
			russiaBlock.topBorder = russiaBlock.obj[i].y;
		if (russiaBlock.bottomBorder < russiaBlock.obj[i].y)
			russiaBlock.bottomBorder = russiaBlock.obj[i].y;
	}
}

COLORREF GetRandomColor()
{
	static bool isFirst = true;
	if (isFirst){
		srand((unsigned)time(NULL));
		isFirst = false;
	}
	return RGB(rand() % 256, rand() % 256, rand() % 256);
}

void DrawFallingObj(RussiaBlock &currentObj, RussiaBlock &preObj, CDC *pDC)
{
	
	//���ػ���һ����������λ��Ϊ����ɫ
	
	for (int i = 0; i < 4; i++)
	{
		CRect rc = GetClientRect(preObj.obj[i]);
		if (!rc.IsRectNull())//�����������С�����������Ϸ������
		{
			COLORREF bgColor = GetSysColor(COLOR_BTNFACE);//��ȡ����ɫ
			CBrush brush(bgColor);
			CBrush *oldBursh = pDC->SelectObject(&brush);
			Block block = preObj.obj[i];
			//CRect rect(block.x*BLOCKSIZE, (block.y - 1)*BLOCKSIZE, (block.x + 1)*BLOCKSIZE, block.y*BLOCKSIZE);
			pDC->FillRect(rc, &brush);
			pDC->SelectObject(oldBursh);
		}
		
	}
	//���Ƶ�ǰ����
	CBrush fillingBrush(currentObj.fillingColor);
	CBrush bdrBrush(currentObj.bdrColor);
	CBrush *pOld = pDC->SelectObject(&fillingBrush);
	for (int i = 0; i < 4; i++)
	{
		CRect rc = GetClientRect(currentObj.obj[i]);
		if (!rc.IsRectNull())  //�����������С�����������Ϸ������
		{
			pDC->Rectangle(&rc);
			pDC->FrameRect(&rc, &bdrBrush); // ���Ʊ߽�,��������Ư����Ŷ.
		}
	}
	pDC->SelectObject(pOld);
	
}
void MoveObject(RussiaBlock &russiaBlock, int offsetX, int offsetY)
{
	for (int i = 0; i < 4; i++)
	{
		russiaBlock.obj[i].x += offsetX;
		russiaBlock.obj[i].y += offsetY;
	}
	UpdateLimit(russiaBlock);
}
void MoveObject(RussiaBlock &russiaBlock, Direction direction)
{
	int offsetX=0, offsetY=0;
	switch (direction)
	{
	case OBJ_DOWN:
		offsetY = 1;
		break;
	case OBJ_LEFT:
		offsetX = -1;
		break;
	case OBJ_RIGHT:
		offsetX = 1;
		break;
	}
	for (int i = 0; i < 4; i++)
	{
		russiaBlock.obj[i].x += offsetX;
		russiaBlock.obj[i].y += offsetY;
	}
	UpdateLimit(russiaBlock);
}

CRect GetClientRect(Block &block)
{
	CRect rect;
	if (block.x < 0 || block.y < 0)
	{
		return CRect(0, 0, 0, 0);
	}
	else
	{
		return CRect(block.x*BLOCKSIZE, block.y*BLOCKSIZE, (block.x + 1)*BLOCKSIZE, (block.y + 1)*BLOCKSIZE);
	}
}

bool IsBorder(RussiaBlock &currentObj , Direction direction)
{
	int offsetX = 0;
	int offsetY = 0;
	switch (direction)
	{
	case OBJ_LEFT:
		if (currentObj.leftBorder <= GAMELEFTWALL){
			return true;
		}
		offsetX = -1;
		offsetY = 0;
		break;
	case OBJ_RIGHT:
		if (currentObj.rightBorder >= GAMERIGHTWALL){
			return true;
		}
		offsetX = 1;
		offsetY = 0;
		break;
	case OBJ_DOWN:
		if (currentObj.bottomBorder >= GAMEBOTWALL){
			return true;
		}
		offsetX = 0;
		offsetY = 1;
		break;
	}
	for (int i = 0; i < 4; i++)//�жϵ�ǰObject���ĸ��������Ƿ�������
	{
		int xPos = currentObj.obj[i].x;
		int yPos = currentObj.obj[i].y;
		if (gameArea[yPos + offsetY][xPos + offsetX].hasObj == true)
			return true;
	}
	return false;
}
void DrawGameArea(CDC *pDC)
{
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (gameArea[i][j].hasObj == true)
			{
				CBrush fillingBrush(gameArea[i][j].fillingColor);
				CBrush bdrBrush(gameArea[i][j].bdrColor);
				CBrush *oldBrush = pDC->SelectObject(&fillingBrush);
				CRect rt(j*BLOCKSIZE, i*BLOCKSIZE, (j + 1)*BLOCKSIZE, (i + 1)*BLOCKSIZE);
				pDC->Rectangle(&rt);
				pDC->FrameRect(&rt, &bdrBrush); // ���Ʊ߽�,��������Ư����Ŷ.
				pDC->SelectObject(oldBrush);

			}
		}
	}
	
}


void SetGameAreaBit(RussiaBlock&currentObj)
{
	for (int i = 0; i < 4; i++)
	{
		Block block = currentObj.obj[i];
		gameArea[block.y][block.x].hasObj = true;
		gameArea[block.y][block.x].fillingColor = currentObj.fillingColor;
		gameArea[block.y][block.x].bdrColor = currentObj.bdrColor;
	}
}

void ResetGameArea()
{
	gameAreaTopLine = GAMEBOTWALL+1;
	for (int i = 0; i < GAMEBOTWALL+1; i++)
	{
		for (int j = 0; j < GAMERIGHTWALL+1; j++)
		{
			gameArea[i][j].hasObj = false;
		}
	}

}

void UpdateTopLine(RussiaBlock &currentObj)
{
	if (currentObj.topBorder < gameAreaTopLine)
		gameAreaTopLine = currentObj.topBorder;
}

bool IsLineFull(RussiaBlock& currentObj , int &LineNum)
{
	LineNum = 0;
	int pBegin = currentObj.bottomBorder;
	int pEnd = currentObj.topBorder;
	//�ɵ�ǰ������½翪ʼ����ɨ�裬ֱ�������Ͻ磨һ����������������4�����У�����gameArea.hasObj�Ƿ����ж���true
	for (int i = pBegin; i>=pEnd;)
	{
		int k;
		for (k = 0; k <= GAMERIGHTWALL; k++){//ɨ��һ�У����Ƿ�ȫ��Ϊfalse
			if (gameArea[i][k].hasObj == false)
				break;
		}
		if (k == GAMERIGHTWALL+1){//һ��ȫ��������һ�����϶���Ҫ�½�һ��
			for (int j = i; j > gameAreaTopLine; j--)
			{
				for (int l = 0; l <= GAMERIGHTWALL; l++)
				{
					gameArea[j][l] = gameArea[j - 1][l];
				}
			}
			for (int j = 0; j <= GAMERIGHTWALL; j++)
				gameArea[gameAreaTopLine][j].hasObj = false;
			LineNum++;//��ȥ�˶����У����4��
			gameAreaTopLine++;//��Ϸ�����½�һ��
			pEnd++;//�����Ͻ��½�һ��
		}
		else
		{
			i--;
		}

	}
	if (LineNum == 0)
		return false;
	return true;
}

void DrawNextObj(RussiaBlock &nextObj, CDC *pDC)
{
	CBrush fillingBrush(nextObj.fillingColor);
	CBrush bdrBrush(nextObj.bdrColor);
	CBrush *pOld = pDC->SelectObject(&fillingBrush);
	for (int i = 0; i < 4; i++)
	{
		Block block = nextObj.obj[i];
		CRect rc(block.x, block.y, (block.x + 1), (block.y + 1));
		//��������õ�nextObj��������λ��
		int l = nextObj.leftBorder;
		int r = nextObj.rightBorder;
		int t = nextObj.topBorder;
		int b = nextObj.bottomBorder;
		int offsetX = 2 - (l + r) / 2;
		int offsetY = 2 - (b + t) / 2;
		rc = CRect(rc.left+offsetX,rc.top+offsetY,rc.right+offsetX,rc.bottom+offsetY);
		rc = CRect(rc.left*BLOCKSIZE, rc.top*BLOCKSIZE, rc.right*BLOCKSIZE, rc.bottom*BLOCKSIZE);
		if (!rc.IsRectNull())  //�����������С�����������Ϸ������
		{
			pDC->Rectangle(&rc);
			pDC->FrameRect(&rc, &bdrBrush); // ���Ʊ߽�
		}
	}
	pDC->SelectObject(pOld);
}

int GetGameAreaTop()
{
	return gameAreaTopLine;
}

void Transform(RussiaBlock &currentObj , int &transfromTimes)
{
	RussiaBlock tempObj = currentObj;//�����ת������ײ��ԭ�����ת
	//��ת���ģ���תԭ�㣩
	int centerX = (currentObj.rightBorder + currentObj.leftBorder) / 2;
	int centerY = (currentObj.topBorder + currentObj.bottomBorder) / 2;
	//��ȡ�����ת���ĵĸ�����������
	/*for (int i = 0; i < 4; i++)
	{
		currentObj.obj[i].x -= centerX;
		currentObj.obj[i].y -= centerY;
	}*/
	MoveObject(currentObj, -centerX, -centerY);
	//������Ļ����ϵ˳ʱ����ת�������ձ�׼�ѿ�������ϵ��ʱ����ת����ת��ʽ
	//X = xcosa-ysina ; Y = -xsina+ycosa;����aΪ��ת�ǣ��˴�ȡ90�ȣ��õ����¹�ʽ��
	//X = -y ; Y = x
	for (int i = 0; i < 4; i++)
	{
		int temp = currentObj.obj[i].x;
		currentObj.obj[i].x = (-1)*currentObj.obj[i].y;
		currentObj.obj[i].y = temp;
	}
	MoveObject(currentObj, centerX, centerY);
	//�ж��Ƿ����ʧ�ܣ�ʧ���򷵻���һ��״̬
	//���κ󳬳���Ϸ��Χ
	if (currentObj.leftBorder < GAMELEFTWALL || currentObj.rightBorder > GAMERIGHTWALL || currentObj.bottomBorder > GAMEBOTWALL){
		currentObj = tempObj;
		return;
	}
	//���κ���ĳ��С�����������غ�
	for (int i = 0; i<4; i++)
	{
		int posX = currentObj.obj[i].x;
		int posY = currentObj.obj[i].y;
		if (posY >= 0 && gameArea[posY][posX].hasObj == true){
			currentObj = tempObj;
			return;
		}
	}
	//�������Ĳ��̶������������ת��������������Ư��һλ���������Ҫ����
	if (transfromTimes == 2){
		MoveObject(currentObj, OBJ_RIGHT);
		transfromTimes = 0;
	}
}