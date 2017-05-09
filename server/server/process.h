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
	int pan[21][21] = {0};
	int shadow[21][21] = {0};

	int lengthBegin = 1;
	int widthBegin = 1;
	int lengthEnd = 20;
	int widthEnd = 20;
	int move_count = 1;

	std::string name1 = "";
	std::string name2 = "";
	bool isSingle = false;

	cChessboard ();
	~cChessboard ();
};


int play(int row, int col, cChessboard* Chessboard);
