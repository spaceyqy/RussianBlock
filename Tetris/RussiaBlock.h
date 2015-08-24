#pragma once

#include <iostream>
#include "afxwin.h"
using namespace std;

const int BLOCKSIZE = 24;//����һ��С��Ϊ24*24��С
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
struct GameArea//��Ϸ�����棬��Ϊ18*10�飬ÿ��24*24����
{
	bool hasObj;//�趨��ǰ�ط��Ƿ�������
	COLORREF fillingColor;//�趨��ǰ�ط����������ɫ
	COLORREF bdrColor;//�趨��ǰ���������ɫ
};

struct RussiaBlock
{
	RussiaBlock(){
		topBorder = -1; leftBorder = -1; rightBorder = -1; bottomBorder = -1;
	}
	~RussiaBlock(){

	}
	Block obj[4];//4���������һ���γ�һ������˹����
	COLORREF fillingColor;//ÿ������˹���鶼���Լ��������ɫ
	COLORREF bdrColor;//�߿���ɫ
	int topBorder;//���row
	int leftBorder;//�����col
	int rightBorder;//���Ҷ�col
	int bottomBorder;//��׶�row
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
bool IsBorder(RussiaBlock &currentObj, Direction direction);//ĳ�������Ƿ��������ڵ�
void SetGameAreaBit(RussiaBlock&currentObj);
void ResetGameArea();
bool IsLineFull(RussiaBlock& currentObj, int &LineNum);
int GetGameAreaTop();
void Transform(RussiaBlock &currentObj, int &transfromTimes);