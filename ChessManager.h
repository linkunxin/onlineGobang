#pragma once
#include "Chess.h"
#include "ChessOperation.h"

class ChessManager {
public:
	bool gameBegin = false;
	bool gameBeginAI = false;
	bool gameBeginPK = false;
	bool gameBeginPK_enemy = false;
	bool gameBeginPK_paint = false;
	bool checkGameOver(int h, int v, int color);//没放下一个棋子的时候，就检测是否胜利
	bool addChess(int h, int v);
	int getNextColor();
	int getCurrentColor();
	void undoOperation();//悔棋
	Operation* currentOperation;
	Chess* getLastChess();//取最后一个棋子，用于悔棋后删掉
	Chess** getAllChess();//用于悔棋后的重置棋子
	void startFromZero();//新建游戏
	ChessManager();
	~ChessManager();
	int nextColorInt;
private:
	Chess** allChess;
	bool checkInBoard(int h);//检查鼠标是否在棋盘上
	//检查是否胜利
	bool checkHorizontal(int h, int v, int color);
	bool checkVertical(int h, int v, int color);
	bool checkDownToUp(int h, int v, int color);
	bool checkUpToDown(int h, int v, int color);
};

