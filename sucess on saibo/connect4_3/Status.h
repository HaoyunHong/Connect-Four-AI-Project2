#pragma once
#include<cstdlib>
#include<ctime>
#include <cstring>
#include <iostream>
#include <cmath>
#include "Judge.h"

// Linux上是微秒
#define TIME_LIMIT 2800000 //可能还要调
#define INF 1000000000
#define C 1.4

// 这样更容易变成二进制编码
#define USER 0
#define MACHINE 1

typedef unsigned int uint;

class Status
{
	uint *chessState, *top;
	int nextChildColumn;
	std::pair<int, int> place;
	bool player; // 表示当前以哪方为考虑对象

	std::pair<int, int> randomPut();

	std::pair<int, int> orderPut();

	// 会在类外初始化
	static int **board;

	// 转化成二维棋盘
	void toBoard();

public:
	// 这些一旦传入了就不会变的
	static int width, height, noX, noY;

	Status(bool player);

	Status(const Status &preStatus, int x, int y);

	~Status();

	std::pair<int, int> getPlace();

	Status *nextChildStatus();

	std::pair<int, int> decideNext(bool isMyFirst);

	float defaultPolicy();
};
