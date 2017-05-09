# include "process.h"

using namespace std;

bool can_eat (int row, int col, int color, VECTORINT2* dead_body, cChessboard* Chessboard);
bool record_dead_body (VECTORINT2* db, cChessboard* Chessboard);
void clean_dead_body (VECTORINT2* db, cChessboard* Chessboard);
bool fill_block_have_air (int row, int col, int color, cChessboard* Chessboard);
bool anti_fill_block_have_air (int color, cChessboard* Chessboard);
void make_shadow (cChessboard* Chessboard);
void flood_fill (int row, int col, int color, cChessboard* Chessboard);
bool have_air (int row, int col, cChessboard* Chessboard);
void stone_down (int row, int col, cChessboard* Chessboard);
bool is_suicide (int row, int col, VECTORINT2* db, cChessboard* Chessboard);

cChessboard::cChessboard () {
	for (int i = 0; i < 21; i++)
		pan[0][i] = cBOUN;
	for (int i = 0; i < 21; i++)
		pan[i][0] = cBOUN;
	for (int i = 0; i < 21; i++)
		pan[20][i] = cBOUN;
	for (int i = 0; i < 21; i++)
		pan[i][20] = cBOUN;

	for (int i = 0; i < 21; i++)
		shadow[0][i] = cBOUN;
	for (int i = 0; i < 21; i++)
		shadow[i][0] = cBOUN;
	for (int i = 0; i < 21; i++)
		shadow[20][i] = cBOUN;
	for (int i = 0; i < 21; i++)
		shadow[i][20] = cBOUN;

}
cChessboard::~cChessboard () {
}

int play (int row, int col, cChessboard* Chessboard) {
	VECTORINT2 dead_body;
	can_eat (row, col, cWHI, &dead_body, Chessboard);
	can_eat (row, col, cBLA, &dead_body, Chessboard);
	bool suicide_flag = is_suicide (row, col, &dead_body, Chessboard);
	clean_dead_body (&dead_body, Chessboard);
	stone_down (row, col, Chessboard);
	
	//自杀
	if (suicide_flag)
		(*Chessboard).pan[row][col] = 0;
	return sOK;
}

bool is_suicide (int row, int col, VECTORINT2* db, cChessboard* Chessboard) {
	int r = 0, c = 0, sum = 0;
	for (int i = 0; i < (*db).size (); i++) {
		r = (*db)[i].a;
		c = (*db)[i].b;
		if (r == row + 1 && c == col ||
			r == row - 1 && c == col ||
			r == row&&c == col + 1 ||
			r == row&&c == col - 1)
			sum++;
	}
	if ((*Chessboard).pan[row + 1][col] == cBOUN)
		sum++;
	if ((*Chessboard).pan[row - 1][col] == cBOUN)
		sum++;
	if ((*Chessboard).pan[row][col + 1] == cBOUN)
		sum++;
	if ((*Chessboard).pan[row][col - 1] == cBOUN)
		sum++;

	if (sum == 4)
		return true;
	else
		return false;

}


bool can_eat (int row, int col, int color, VECTORINT2* dead_body, cChessboard* Chessboard) { // color 是当前要落子的颜色
	int ret = false;
	int anti_color = cWHI;
	if (color == cWHI)
		anti_color = cBLA;

	if (row + 1 <= (*Chessboard).lengthEnd - 1 && (*Chessboard).pan[row + 1][col] == anti_color) {
		make_shadow (Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill (row + 1, col, anti_color, Chessboard);
		if (!anti_fill_block_have_air (anti_color, Chessboard)) {
			// 记录下这些cFILL的坐标，以及(row+1,col)，表示可以吃的对方棋子
			int rret = record_dead_body (dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (row - 1 >= (*Chessboard).lengthBegin && (*Chessboard).pan[row - 1][col] == anti_color) {
		make_shadow (Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill (row - 1, col, anti_color, Chessboard);
		if (!anti_fill_block_have_air (anti_color, Chessboard)) {
			int rret = record_dead_body (dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (col + 1 <= (*Chessboard).widthEnd - 1 && (*Chessboard).pan[row][col + 1] == anti_color) {
		make_shadow (Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill (row, col + 1, anti_color, Chessboard);
		if (!anti_fill_block_have_air (anti_color, Chessboard)) {
			int rret = record_dead_body (dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (col - 1 >= (*Chessboard).widthBegin && (*Chessboard).pan[row][col - 1] == anti_color) {
		make_shadow (Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill (row, col - 1, anti_color, Chessboard);
		if (!anti_fill_block_have_air (anti_color, Chessboard)) {
			int rret = record_dead_body (dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	return ret;
}

bool record_dead_body (VECTORINT2* db, cChessboard* Chessboard) {
	int ret = false;
	for (int row = (*Chessboard).widthBegin; row < (*Chessboard).widthEnd; row++) {
		for (int col = (*Chessboard).lengthBegin; col < (*Chessboard).lengthEnd; col++) {
			if ((*Chessboard).shadow[row][col] == cFILL) {
				(*db).push_back ({ row, col });
				ret = true;
			}
		}
	}
	return ret;
}

void clean_dead_body (VECTORINT2* db, cChessboard* Chessboard) {
	int n = 0, m = 0;
	for (int i = 0; i < (*db).size (); i++) {
		n = (*db)[i].a;
		m = (*db)[i].b;
		(*Chessboard).pan[n][m] = 0;
	}
}

/* 填充的区域周围是否有空 */
bool fill_block_have_air (int row, int col, int color, cChessboard* Chessboard) {
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++) {
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++) {
			if (i != row || j != col) {
				if ((*Chessboard).shadow[i][j] == cFILL && (*Chessboard).pan[i][j] != color) {
					return true; // 此块有空，可下
				}
			}
		}
	}
	return false;
}

/* 提吃判断专用 */
bool anti_fill_block_have_air (int color, cChessboard* Chessboard) {
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++) {
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++) {
			if ((*Chessboard).shadow[i][j] == cFILL && (*Chessboard).pan[i][j] != color) {
				return true; // 活
			}
		}
	}
	return false; //死
}

/* 将盘面做个影分身 */
void make_shadow (cChessboard* Chessboard) {
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++) {
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++) {
			(*Chessboard).shadow[i][j] = (*Chessboard).pan[i][j];
		}
	}
}

/* 泛洪填充，只操作影分身 */
void flood_fill (int row, int col, int color, cChessboard* Chessboard) { // color 为当前要填充的颜色
	if (row < (*Chessboard).lengthBegin
		|| row > (*Chessboard).lengthEnd - 1
		|| col < (*Chessboard).widthBegin
		|| col > (*Chessboard).widthEnd - 1)
		return;

	int anti_color = cWHI;
	if (color == cWHI)
		anti_color = cBLA;

	if ((*Chessboard).shadow[row][col] != anti_color && (*Chessboard).shadow[row][col] != cFILL) { // 非color颜色，且未被填充
		(*Chessboard).shadow[row][col] = cFILL; // 表示已被填充
		flood_fill (row + 1, col, color, Chessboard);
		flood_fill (row - 1, col, color, Chessboard);
		flood_fill (row, col + 1, color, Chessboard);
		flood_fill (row, col - 1, color, Chessboard);
	}
}

/* 坐标周围4交叉点有气否？ */
bool have_air (int row, int col, cChessboard* Chessboard) {

	if ((*Chessboard).pan[row + 1][col] != cEmp &&
		(*Chessboard).pan[row - 1][col] != cEmp &&
		(*Chessboard).pan[row][col + 1] != cEmp &&
		(*Chessboard).pan[row][col - 1] != cEmp) {
		return false;
	}
	else {
		return true;
	}

}


// 真正落子
void stone_down (int row, int col, cChessboard* Chessboard) {
	if ((*Chessboard).move_count % 2 == 0) { //未落子前是白
		(*Chessboard).pan[row][col] = cBLA; //就放黑
	}
	else {
		(*Chessboard).pan[row][col] = cWHI;
	}
	(*Chessboard).move_count++;
	tCELL3 p = { row, col, (*Chessboard).move_count };
	(*Chessboard).move_record.push_back (p); // 记录手数

}
