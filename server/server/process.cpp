# include "process.h"

using namespace std;

bool is_jie(int row, int col, VECTORINT2* dead_body, cChessboard* Chessboard);
bool can_eat(int row, int col, int color, VECTORINT2* dead_body, cChessboard* Chessboard);
bool record_dead_body(VECTORINT2* db, cChessboard* Chessboard);
void clean_dead_body(VECTORINT2* db, cChessboard* Chessboard);
bool fill_block_have_air(int row, int col, int color, cChessboard* Chessboard);
bool anti_fill_block_have_air(int color, cChessboard* Chessboard);
void make_shadow(cChessboard* Chessboard);
void shadow_to_pan(cChessboard* Chessboard);
void flood_fill(int row, int col, int color, cChessboard* Chessboard);
bool have_air(int row, int col, cChessboard* Chessboard);
bool have_my_people(int row, int col, cChessboard* Chessboard);
void stone_down(int row, int col, cChessboard* Chessboard);

cChessboard::cChessboard (){
	for (int i = 0; i<21; i++)
		pan[0][i] = cBOUN;
	for (int i = 0; i<21; i++)
		pan[i][0] = cBOUN;
	for (int i = 0; i<21; i++)
		pan[20][i] = cBOUN;
	for (int i = 0; i<21; i++)
		pan[i][20] = cBOUN;

	for (int i = 0; i<21; i++)
		shadow[0][i] = cBOUN;
	for (int i = 0; i<21; i++)
		shadow[i][0] = cBOUN;
	for (int i = 0; i<21; i++)
		shadow[20][i] = cBOUN;
	for (int i = 0; i<21; i++)
		shadow[i][20] = cBOUN;

}
cChessboard::~cChessboard (){
}


int play(int row, int col, cChessboard* Chessboard){
	bool can_down = false; // 是否可落子
	tCOLOR color = cWHI; // 白 //棋子颜色
	if ((*Chessboard).move_count % 2 == 0) { // 未落子前是白
		color = cBLA;
	}
	//TODO 这部分放到js里面
	//if (row < 0 || row > 19 || col < 0 || col > 19){
	//	//alert("index error....");
	//	return;
	//}
	//// 处理已有棋子在此
	//if ((*Chessboard).pan[row][col] != 0){
	//	//alert("此处已有棋子！");
	//	return;
	//}

	// 得到将落子的棋子的颜色


	if (!have_air(row, col, Chessboard)){
		if (have_my_people(row, col, Chessboard)){
			make_shadow(Chessboard);

			flood_fill(row, col, color, Chessboard);
			if (fill_block_have_air(row, col, color, Chessboard)){
				can_down = true;
				VECTORINT2 dead_body;
				can_eat(row, col, color, &dead_body, Chessboard);
				clean_dead_body(&dead_body, Chessboard);
			}
			else{
				VECTORINT2 dead_body;
				int cret = can_eat(row, col, color, &dead_body, Chessboard);
				clean_dead_body(&dead_body, Chessboard);

				if (cret){
					can_down = true;
				}
				else{
					//alert("无气，不能落子！！");
					return sWUQI;
				}
			}
		}
		else{
			VECTORINT2 dead_body;
			int cret = can_eat(row, col, color, &dead_body, Chessboard);

			// 劫争也应该在此处理，只在此处理？
			if (cret){
				if (!is_jie(row, col, &dead_body, Chessboard)){
					clean_dead_body(&dead_body, Chessboard);
					can_down = true;
				}
				else{
					//alert("劫, 不能落子, 请至少隔一手棋！");
					return sJIE;

				}
			}
		}
	}
	else{
		can_down = true;
		VECTORINT2 dead_body;
		can_eat(row, col, color, &dead_body, Chessboard);
		clean_dead_body(&dead_body, Chessboard);
	}
	if (can_down){
		stone_down(row, col, Chessboard);
	}
	return sOK;
}

// TODO 劫争处理的本质是防止全局同型，基于此，还是要处理连环劫之类的，再说吧
// 我先看看应氏围棋规则，研究研究
bool is_jie(int row, int col, VECTORINT2* dead_body, cChessboard* Chessboard){ //是否劫
	//只吃了一个？ 希望我对围棋的理解没错，单劫都是只互吃一个。
	if (dead_body->size() == true){
		for (int i = 0; i < (*Chessboard).jie.size(); i++){
			//若符合（有坐标，且(*Chessboard).move_count就是上一手）
			//注意此处比较的是死去的棋子，下面push的是本次落子的棋子
			if ((*Chessboard).jie[i].a == (*dead_body)[0].a &&
				(*Chessboard).jie[i].b == (*dead_body)[0].b &&
				(*Chessboard).jie[i].c == (*Chessboard).move_count){
				return true;
			}
		}
		//加入记录表

		tCELL3 p = {row, col, (*Chessboard).move_count};
		(*Chessboard).jie.push_back(p); // 记录手数
		return false;
	}
	return false;
}

/* 能提吃吗？ */
bool can_eat(int row, int col, int color, VECTORINT2* dead_body, cChessboard* Chessboard){ // color 是当前要落子的颜色
	int ret = false;
	int anti_color = cWHI;
	if (color == cWHI)
		anti_color = cBLA;

	if (row + 1 <= (*Chessboard).lengthEnd - 1 && (*Chessboard).pan[row + 1][col] == anti_color){
		make_shadow(Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill(row + 1, col, anti_color, Chessboard);
		if (!anti_fill_block_have_air(anti_color, Chessboard)){
			// 记录下这些cFILL的坐标，以及(row+1,col)，表示可以提吃的对方棋子
			//alert("提吃: "+(row+1).toString()+","+col.toString());
			int rret = record_dead_body(dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (row - 1 >= (*Chessboard).lengthBegin && (*Chessboard).pan[row - 1][col] == anti_color){
		make_shadow(Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill(row - 1, col, anti_color, Chessboard);
		if (!anti_fill_block_have_air(anti_color, Chessboard)){
			int rret = record_dead_body(dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (col + 1 <= (*Chessboard).widthEnd - 1 && (*Chessboard).pan[row][col + 1] == anti_color){
		make_shadow(Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill(row, col + 1, anti_color, Chessboard);
		if (!anti_fill_block_have_air(anti_color, Chessboard)){
			int rret = record_dead_body(dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	if (col - 1 >= (*Chessboard).widthBegin && (*Chessboard).pan[row][col - 1] == anti_color){
		make_shadow(Chessboard);
		(*Chessboard).shadow[row][col] = color;
		flood_fill(row, col - 1, anti_color, Chessboard);
		if (!anti_fill_block_have_air(anti_color, Chessboard)){
			int rret = record_dead_body(dead_body, Chessboard);
			ret = ret || rret;
		}

	}
	return ret;
}

bool record_dead_body(VECTORINT2* db, cChessboard* Chessboard){
	int ret = false;
	for (int row = (*Chessboard).widthBegin; row < (*Chessboard).widthEnd; row++){
		for (int col = (*Chessboard).lengthBegin; col < (*Chessboard).lengthEnd; col++){
			if ((*Chessboard).shadow[row][col] == cFILL){
				(*db).push_back({row, col});
				ret = true; // it's true have dead body
				//alert("DEAD: "+(row).toString()+","+col.toString());
			}
		}
	}
	return ret;
}

void clean_dead_body(VECTORINT2* db, cChessboard* Chessboard){
	int n = 0, m = 0;
	for (int i = 0; i < (*db).size(); i++){
		n = (*db)[i].a;
		m = (*db)[i].b;
		(*Chessboard).pan[n][m] = 0;
		//alert("OUT: "+(db[i][0]).toString()+","+(db[i][1]).toString());
	}
}

/* 填充的区域周围是否有空 */
bool fill_block_have_air(int row, int col, int color, cChessboard* Chessboard){
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++){
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++){
			if (i != row || j != col){
				if ((*Chessboard).shadow[i][j] == cFILL && (*Chessboard).pan[i][j] != color){
					return true; // 此块有空，可下
				}
			}
		}
	}
	//alert("fill block 无气！！！");
	return false;
}

/* 提吃判断专用 */
bool anti_fill_block_have_air(int color, cChessboard* Chessboard){
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++){
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++){
			if ((*Chessboard).shadow[i][j] == cFILL && (*Chessboard).pan[i][j] != color){
				return true; // 活
			}
		}
	}
	//alert("anti fill block 无气！！！");
	return false; //死
}

/* 将盘面做个影分身 */
void make_shadow(cChessboard* Chessboard){
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++){
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++){
			(*Chessboard).shadow[i][j] = (*Chessboard).pan[i][j];
		}
	}
}

void shadow_to_pan(cChessboard* Chessboard){
	for (int i = (*Chessboard).lengthBegin; i < (*Chessboard).lengthEnd; i++){
		for (int j = (*Chessboard).widthBegin; j < (*Chessboard).widthEnd; j++){
			(*Chessboard).pan[i][j] = (*Chessboard).shadow[i][j];
		}
	}
}

/* 泛洪填充，只操作影分身 */
void flood_fill(int row, int col, int color, cChessboard* Chessboard){ // color 为当前要填充的颜色
	if (row < (*Chessboard).lengthBegin 
		|| row > (*Chessboard).lengthEnd - 1 
		|| col < (*Chessboard).widthBegin 
		|| col > (*Chessboard).widthEnd - 1)
		return;

	int anti_color = cWHI;
	if (color == cWHI)
		anti_color = cBLA;

	if ((*Chessboard).shadow[row][col] != anti_color && (*Chessboard).shadow[row][col] != cFILL){ // 非color颜色，且未被填充
		(*Chessboard).shadow[row][col] = cFILL; // 表示已被填充
		flood_fill(row + 1, col, color, Chessboard);
		flood_fill(row - 1, col, color, Chessboard);
		flood_fill(row, col + 1, color, Chessboard);
		flood_fill(row, col - 1, color, Chessboard);
	}
}

/* 坐标周围4交叉点有气否？ */
bool have_air (int row, int col, cChessboard* Chessboard) {

		if ((*Chessboard).pan[row + 1][col] !=cEmp &&
			(*Chessboard).pan[row - 1][col] !=cEmp &&
			(*Chessboard).pan[row][col + 1] !=cEmp &&
			(*Chessboard).pan[row][col - 1] !=cEmp) {
			//alert("have no air");
			return false;
		}
		else {
			//alert("have air");
			return true;
		}

}

/* 坐标周围是否有我方的棋子 */
bool have_my_people (int row, int col, cChessboard* Chessboard) { //FIXME 边角没有处理呢
		if ((*Chessboard).move_count % 2 ==  0) { //未落子前是白
			if ((*Chessboard).pan[row + 1][col] ==  cBLA ||
				(*Chessboard).pan[row - 1][col] ==  cBLA ||
				(*Chessboard).pan[row][col + 1] ==  cBLA ||
				(*Chessboard).pan[row][col - 1] ==  cBLA) {
				//alert("have my people");
				return true;
			}
		}
		else {
			if ((*Chessboard).pan[row + 1][col] ==  cWHI ||
				(*Chessboard).pan[row - 1][col] ==  cWHI ||
				(*Chessboard).pan[row][col + 1] ==  cWHI ||
				(*Chessboard).pan[row][col - 1] ==  cWHI) {
				//alert("have my people");
				return true;
			}
		}
	return false;
}


// 真正落子
void stone_down(int row, int col, cChessboard* Chessboard){
	if ((*Chessboard).move_count % 2 == 0){ //未落子前是白
		(*Chessboard).pan[row][col] = cBLA; //就放黑
	}
	else{
		(*Chessboard).pan[row][col] = cWHI;
	}
	(*Chessboard).move_count++;
	tCELL3 p = {row, col, (*Chessboard).move_count};
	(*Chessboard).move_record.push_back(p); // 记录手数

}
