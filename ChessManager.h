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
	bool checkGameOver(int h, int v, int color);//û����һ�����ӵ�ʱ�򣬾ͼ���Ƿ�ʤ��
	bool addChess(int h, int v);
	int getNextColor();
	int getCurrentColor();
	void undoOperation();//����
	Operation* currentOperation;
	Chess* getLastChess();//ȡ���һ�����ӣ����ڻ����ɾ��
	Chess** getAllChess();//���ڻ�������������
	void startFromZero();//�½���Ϸ
	ChessManager();
	~ChessManager();
	int nextColorInt;
private:
	Chess** allChess;
	bool checkInBoard(int h);//�������Ƿ���������
	//����Ƿ�ʤ��
	bool checkHorizontal(int h, int v, int color);
	bool checkVertical(int h, int v, int color);
	bool checkDownToUp(int h, int v, int color);
	bool checkUpToDown(int h, int v, int color);
};

