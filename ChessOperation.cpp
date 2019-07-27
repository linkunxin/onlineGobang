#include "stdafx.h"
#include "ChessOperation.h"

void Operation::setPreOperation(Operation * pre)
{
	previewOperation = pre;
}

void Operation::setNextOperation(Operation* next) {
	nextOperation = next;
}
Operation::Operation(Chess* c) {
	chess = c;
	previewOperation = nullptr;
	nextOperation = nullptr;
}
Operation* Operation::getNextOperation() {
	return nextOperation;
}