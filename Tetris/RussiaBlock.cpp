#include "RussiaBlock.h"
#include<time.h>
#include<cmath>

GameArea gameArea[GAMEBOTWALL+1][GAMERIGHTWALL+1];//游戏主界面
int gameAreaTopLine;//游戏主界面最顶层行号

/*
随机产生俄罗斯方块，产生方式如下：
1.设定一个方块的初始位置为(4,-1)，即在还没有进入游戏区域处产生一个方块，以这个方块的初始位置加上一个偏移量(offsetX,offsetY)得到新方块的坐标值
2.定义偏移量为 下（0,1） 上(0,-1)  左(-1,0) 右(1,0)
3.为满足不让下一个方块位置与上一个方块位置重合，只需要满足下一个方块偏移量不等于上一次偏移量取反即可
*/
void RandomCreateRussiaBlock(RussiaBlock &russiaBlock)
{
	russiaBlock.obj[0].x = russiaBlock.obj[3].x = 4;//定义初始值
	russiaBlock.obj[0].y = russiaBlock.obj[3].y = -1;

	int offsetX = -2, offsetY = -2;//初始化偏移量
	for (int i = 1; i <= 2; i++){
		RandomizeOffset(offsetX, offsetY);
		russiaBlock.obj[i].x = russiaBlock.obj[i - 1].x + offsetX;
		russiaBlock.obj[i].y = russiaBlock.obj[i - 1].y + offsetY;
		offsetX = -offsetX;
		offsetY = -offsetY;
	}

	UpdateLimit(russiaBlock);//更新上下界
	//当前面三个方块形成"1"或"-"形时,有0.5概率形成"T"字方块
	static bool isFirst = true;
	if (isFirst){
		srand((unsigned)time(NULL));
		isFirst = false;
	}
	int randNum = rand() % 100;//形成0~99之间的数
	if (randNum < 50 && (russiaBlock.bottomBorder - russiaBlock.topBorder == 2 || russiaBlock.rightBorder - russiaBlock.leftBorder == 2))
	{
		if (abs(russiaBlock.obj[0].y - russiaBlock.obj[1].y) == 1 && abs(russiaBlock.obj[1].y - russiaBlock.obj[2].y) == 1)//为竖的1字，须满足offsetX与offsetY只能是(-1,0)与(1,0)
		{
			do{
				RandomizeOffset(offsetX, offsetY);
			} while (abs(offsetX) != 1);
		}
		else//为横的1字，须满足offsetX与offsetY只能是(0,-1)与(0,1)
		{
			do{
				RandomizeOffset(offsetX, offsetY);
			} while (abs(offsetY) != 1);
		}
		russiaBlock.obj[3].x = russiaBlock.obj[1].x + offsetX;//使用中间的块产生T形块
		russiaBlock.obj[3].y = russiaBlock.obj[1].y + offsetY;
	}
	else
	{
		RandomizeOffset(offsetX, offsetY);
		russiaBlock.obj[3].x = russiaBlock.obj[2].x + offsetX;//使用第三块产生第四块
		russiaBlock.obj[3].y = russiaBlock.obj[2].y + offsetY;
	}
	russiaBlock.fillingColor = GetRandomColor();
	russiaBlock.bdrColor = GetRandomColor();
	UpdateLimit(russiaBlock);
	//形成随机块后，统一将块最低位bottomBdr与-1齐平，为落入主区域做准备
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
//更新上下界，左上取最小，右下取最大
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
	
	//先重绘上一个物体所在位置为背景色
	
	for (int i = 0; i < 4; i++)
	{
		CRect rc = GetClientRect(preObj.obj[i]);
		if (!rc.IsRectNull())//这个组成物体的小方块出现在游戏区域内
		{
			COLORREF bgColor = GetSysColor(COLOR_BTNFACE);//获取背景色
			CBrush brush(bgColor);
			CBrush *oldBursh = pDC->SelectObject(&brush);
			Block block = preObj.obj[i];
			//CRect rect(block.x*BLOCKSIZE, (block.y - 1)*BLOCKSIZE, (block.x + 1)*BLOCKSIZE, block.y*BLOCKSIZE);
			pDC->FillRect(rc, &brush);
			pDC->SelectObject(oldBursh);
		}
		
	}
	//绘制当前方块
	CBrush fillingBrush(currentObj.fillingColor);
	CBrush bdrBrush(currentObj.bdrColor);
	CBrush *pOld = pDC->SelectObject(&fillingBrush);
	for (int i = 0; i < 4; i++)
	{
		CRect rc = GetClientRect(currentObj.obj[i]);
		if (!rc.IsRectNull())  //这个组成物体的小方块出现在游戏区域内
		{
			pDC->Rectangle(&rc);
			pDC->FrameRect(&rc, &bdrBrush); // 绘制边界,看起来会漂亮点哦.
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
	for (int i = 0; i < 4; i++)//判断当前Object的四个方块左方是否有物体
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
				pDC->FrameRect(&rt, &bdrBrush); // 绘制边界,看起来会漂亮点哦.
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
	//由当前物体的下界开始向上扫描，直到物体上界（一个物体落下最多造成4格满行），看gameArea.hasObj是否整行都是true
	for (int i = pBegin; i>=pEnd;)
	{
		int k;
		for (k = 0; k <= GAMERIGHTWALL; k++){//扫描一行，看是否全部为false
			if (gameArea[i][k].hasObj == false)
				break;
		}
		if (k == GAMERIGHTWALL+1){//一行全满，从这一行往上都需要下降一行
			for (int j = i; j > gameAreaTopLine; j--)
			{
				for (int l = 0; l <= GAMERIGHTWALL; l++)
				{
					gameArea[j][l] = gameArea[j - 1][l];
				}
			}
			for (int j = 0; j <= GAMERIGHTWALL; j++)
				gameArea[gameAreaTopLine][j].hasObj = false;
			LineNum++;//消去了多少行，最多4行
			gameAreaTopLine++;//游戏顶行下降一行
			pEnd++;//物体上界下降一行
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
		//将方块放置到nextObj框框的中心位置
		int l = nextObj.leftBorder;
		int r = nextObj.rightBorder;
		int t = nextObj.topBorder;
		int b = nextObj.bottomBorder;
		int offsetX = 2 - (l + r) / 2;
		int offsetY = 2 - (b + t) / 2;
		rc = CRect(rc.left+offsetX,rc.top+offsetY,rc.right+offsetX,rc.bottom+offsetY);
		rc = CRect(rc.left*BLOCKSIZE, rc.top*BLOCKSIZE, rc.right*BLOCKSIZE, rc.bottom*BLOCKSIZE);
		if (!rc.IsRectNull())  //这个组成物体的小方块出现在游戏区域内
		{
			pDC->Rectangle(&rc);
			pDC->FrameRect(&rc, &bdrBrush); // 绘制边界
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
	RussiaBlock tempObj = currentObj;//如果旋转后有碰撞则复原这次旋转
	//旋转中心（旋转原点）
	int centerX = (currentObj.rightBorder + currentObj.leftBorder) / 2;
	int centerY = (currentObj.topBorder + currentObj.bottomBorder) / 2;
	//获取相对旋转中心的各个方块坐标
	/*for (int i = 0; i < 4; i++)
	{
		currentObj.obj[i].x -= centerX;
		currentObj.obj[i].y -= centerY;
	}*/
	MoveObject(currentObj, -centerX, -centerY);
	//依照屏幕坐标系顺时针旋转即是依照标准笛卡尔坐标系逆时针旋转，旋转公式
	//X = xcosa-ysina ; Y = -xsina+ycosa;这里a为旋转角，此处取90度，得到如下公式：
	//X = -y ; Y = x
	for (int i = 0; i < 4; i++)
	{
		int temp = currentObj.obj[i].x;
		currentObj.obj[i].x = (-1)*currentObj.obj[i].y;
		currentObj.obj[i].y = temp;
	}
	MoveObject(currentObj, centerX, centerY);
	//判断是否变形失败，失败则返回上一个状态
	//变形后超出游戏范围
	if (currentObj.leftBorder < GAMELEFTWALL || currentObj.rightBorder > GAMERIGHTWALL || currentObj.bottomBorder > GAMEBOTWALL){
		currentObj = tempObj;
		return;
	}
	//变形后有某个小块与其他项重合
	for (int i = 0; i<4; i++)
	{
		int posX = currentObj.obj[i].x;
		int posY = currentObj.obj[i].y;
		if (posY >= 0 && gameArea[posY][posX].hasObj == true){
			currentObj = tempObj;
			return;
		}
	}
	//由于中心不固定，因此连续旋转两次则会出现向左漂移一位的情况，需要矫正
	if (transfromTimes == 2){
		MoveObject(currentObj, OBJ_RIGHT);
		transfromTimes = 0;
	}
}