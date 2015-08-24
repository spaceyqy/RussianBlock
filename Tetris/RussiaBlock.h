#pragma once

#include <iostream>
#include "afxwin.h"
using namespace std;

const int BLOCKSIZE = 24;//定义一个小块为24*24大小
const int GAMELEFTWALL = 0;
const int GAMERIGHTWALL = 9;
const int GAMETOPWALL = 0;
const int GAMEBOTWALL = 17;

struct Block
{
	Block(){ x = -1; y = -1; }
	int x;
	int y;
};
struct GameArea//游戏主界面，分为18*10块，每块24*24像素
{
	bool hasObj;//设定当前地方是否有物体
	COLORREF fillingColor;//设定当前地方方块填充颜色
	COLORREF bdrColor;//设定当前方块外框颜色
};

struct RussiaBlock
{
	RussiaBlock(){
		topBorder = -1; leftBorder = -1; rightBorder = -1; bottomBorder = -1;
	}
	~RussiaBlock(){

	}
	Block obj[4];//4个方块合在一起形成一个俄罗斯方块
	COLORREF fillingColor;//每个俄罗斯方块都有自己的填充颜色
	COLORREF bdrColor;//边框颜色
	int topBorder;//最顶部row
	int leftBorder;//最左端col
	int rightBorder;//最右端col
	int bottomBorder;//最底端row
};
enum Direction{
	OBJ_DOWN,
	OBJ_LEFT,
	OBJ_RIGHT
};
void RandomCreateRussiaBlock(RussiaBlock &russiaBlock);
void RandomizeOffset(int &offsetX, int &offsetY);
void UpdateLimit(RussiaBlock &russiaBlock);
void UpdateTopLine(RussiaBlock &currentObj);
COLORREF GetRandomColor();
void MoveObject(RussiaBlock &russiaBlock, Direction direction);
void MoveObject(RussiaBlock &russiaBlock, int offsetX, int offsetY);
void DrawGameArea(CDC *pDC);
void DrawFallingObj(RussiaBlock &currentObj ,RussiaBlock &preObj , CDC *pDC);
void DrawNextObj(RussiaBlock &nextObj, CDC *pDC);
CRect GetClientRect(Block &block);
bool IsBorder(RussiaBlock &currentObj, Direction direction);//某个方向是否有物体遮挡
void SetGameAreaBit(RussiaBlock&currentObj);
void ResetGameArea();
bool IsLineFull(RussiaBlock& currentObj, int &LineNum);
int GetGameAreaTop();
void Transform(RussiaBlock &currentObj, int &transfromTimes);