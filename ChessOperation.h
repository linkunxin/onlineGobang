#pragma once
#include "Chess.h"
class Operation {
public:
	Chess* chess;
	Operation* nextOperation;
	Operation* previewOperation;
	void setPreOperation(Operation* pre);//����previewOperation��ֵ
	void setNextOperation(Operation* next);//����nextOperation��ֵ
	Operation(Chess* c);//��ʼ������
	Operation* getNextOperation();//����nextOperation
};
