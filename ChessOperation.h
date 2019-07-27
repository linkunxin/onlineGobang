#pragma once
#include "Chess.h"
class Operation {
public:
	Chess* chess;
	Operation* nextOperation;
	Operation* previewOperation;
	void setPreOperation(Operation* pre);//设置previewOperation的值
	void setNextOperation(Operation* next);//设置nextOperation的值
	Operation(Chess* c);//初始化棋子
	Operation* getNextOperation();//返回nextOperation
};
