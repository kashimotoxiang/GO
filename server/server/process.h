#pragma once

#include <vector>

//处理状态
#define sOK 1
#define sJIE 2
#define sWUQI 3

typedef struct//二元胞
{
	int a;
	int b;
} tCELL2;

typedef struct//三元胞
{
	int a;
	int b;
	int c;
} tCELL3;

typedef enum//棋盘点的属性
{
	cBOUN = -1,
	cEmp = 0,
	cBLA = 1,
	cWHI = 2,
	cFILL = 3,
} tCOLOR;

typedef std::vector<tCELL2> VECTORINT2;
typedef std::vector<tCELL3> VECTORINT3;

class cChessboard
{
public:
	VECTORINT3 move_record;
	VECTORINT3 jie;

	int pan[19][19] = {0};
	int shadow[19][19] = {0};

	int length = 19;
	int width = 19;

	int move_count = 1;
};


int play(int row, int col);
