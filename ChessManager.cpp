#include "stdafx.h"
#include "ChessManager.h"


ChessManager::ChessManager() {
	nextColorInt = 1;//默认黑色
	allChess = new Chess*[15];
	for (int i = 0; i < 15; i++) {
		allChess[i] = new Chess[15];
	}
	
	currentOperation = nullptr;
}

ChessManager::~ChessManager() {
	for (int i = 0; i < 15; i++) {
		delete[] allChess[i];
	}
	delete[] allChess;//删除棋子
	Operation* tem;
	while (currentOperation != nullptr) {
		tem = currentOperation->previewOperation;
		delete currentOperation;
		currentOperation = tem;
	}
}				

bool ChessManager::checkInBoard(int h) {
	return h >= 0 && h < 15;
}

bool ChessManager::checkHorizontal(int h, int v, int color) {
	int hitCount = 0;
	bool continous = false;
	for (int i = -5; i < 5; i++) {
		if (hitCount == 5) {
			break;
		}
		if (checkInBoard(h + i) && allChess[h + i][v].color == color) {
			if (continous) {
				hitCount++;
			}
			else {
				continous = true;
				hitCount = 1;
			}
		}
		else {
			continous = false;
		}
	}
	if (hitCount == 5) {
		return true;
	}
	return false;

}

bool ChessManager::checkVertical(int h, int v, int color) {
	int hitCount = 0;
	bool continous = false;
	for (int i = -5; i < 5; i++) {
		if (hitCount == 5) {
			break;
		}
		if (checkInBoard(v + i) && allChess[h][v + i].color == color) {
			if (continous) {
				hitCount++;
			}
			else {
				continous = true;
				hitCount = 1;
			}
		}
		else {
			continous = false;
		}
	}
	if (hitCount == 5) {
		return true;
	}
	return false;

}

bool ChessManager::checkUpToDown(int h, int v, int color) {
	int hitCount = 0;
	bool continous = false;
	for (int i = -5; i < 5; i++) {
		if (hitCount == 5) {
			break;
		}
		if (checkInBoard(h + i) && checkInBoard(v + i) && allChess[h + i][v + i].color == color) {
			if (continous) {
				hitCount++;
			}
			else {
				continous = true;
				hitCount = 1;
			}
		}
		else {
			continous = false;
		}
	}
	if (hitCount == 5) {
		return true;
	}
	return false;

}

bool ChessManager::checkDownToUp(int h, int v, int color) {
	int hitCount = 0;
	bool continous = false;
	for (int i = -5; i < 5; i++) {
		if (hitCount == 5) {
			break;
		}
		if (checkInBoard(h + i) && checkInBoard(v - i) && allChess[h + i][v - i].color == color) {
			if (continous) {
				hitCount++;
			}
			else {
				continous = true;
				hitCount = 1;
			}
		}
		else {
			continous = false;
		}
	}
	if (hitCount == 5) {
		return true;
	}
	return false;

}

bool ChessManager::checkGameOver(int h, int v, int color)
{
	//检查横向
	if (checkHorizontal(h, v, color)) {
		return true;
	}
	if (checkVertical(h, v, color)) {
		return true;
	}
	if (checkUpToDown(h, v, color)) {
		return true;
	}
	if (checkDownToUp(h, v, color)) {
		return true;
	}
	return false;
}

bool ChessManager::addChess(int h, int v)
{
		if (allChess[h][v].color == -1) {
		if (gameBegin == true||gameBeginAI==true||gameBeginPK==true||gameBeginPK_enemy==true) {
			allChess[h][v].color = nextColorInt;
			allChess[h][v].h = h;
			allChess[h][v].v = v;
			//生成一个操作，并且将其压入栈
			Operation* newOpe = new Operation(&allChess[h][v]);
			if (currentOperation == nullptr) {
				currentOperation = newOpe;
				return true;
			}
			currentOperation->nextOperation = newOpe;
			newOpe->previewOperation = currentOperation;
			currentOperation = newOpe;
			return true;
		}
	}
	else {
		return false;
	}
}

int ChessManager::getNextColor()
{
	int currentColor = nextColorInt;
	nextColorInt = (nextColorInt + 1) % 2;
	return currentColor;
}

int ChessManager::getCurrentColor()
{
	return nextColorInt;
}

Chess * ChessManager::getLastChess()
{
	return currentOperation->chess;
}

Chess ** ChessManager::getAllChess()
{
	return allChess;
}

void ChessManager::startFromZero()
{
	while (currentOperation != nullptr) {
		undoOperation();
	}
}

void ChessManager::undoOperation()
{
	if (currentOperation != nullptr) {
		currentOperation->chess->color = -1;
		Operation* tem;
		tem = currentOperation->previewOperation;
		delete currentOperation;
		currentOperation = tem;
		if(gameBegin == true||gameBeginAI==true )
			nextColorInt = (nextColorInt + 1) % 2;
	}
}