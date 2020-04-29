/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/
#pragma once
#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Point.h"
#include "UCT.h"
#include <cstring>
#include <ctime>

extern "C" Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" void clearPoint(Point* p);

void clearArray(int M, int N, int** board);

bool isNewGame(const int *_board, int M, int N);

void initGame(int M, int N, int noX, int noY);


#endif