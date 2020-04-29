#pragma once
#include<cstdlib>
#include<ctime>
#include <cstring>
#include <iostream>
#include <cmath>
#include "Judge.h"

// Linux����΢��
#define TIME_LIMIT 2800000 //���ܻ�Ҫ��
#define INF 1000000000
#define C 1.4

// ���������ױ�ɶ����Ʊ���
#define USER 0
#define MACHINE 1

typedef unsigned int uint;

class Status
{
	uint *chessState, *top;
	int nextChildColumn;
	std::pair<int, int> place;
	bool player; // ��ʾ��ǰ���ķ�Ϊ���Ƕ���

	std::pair<int, int> randomPut();

	std::pair<int, int> orderPut();

	// ���������ʼ��
	static int **board;

	// ת���ɶ�ά����
	void toBoard();

public:
	// ��Щһ�������˾Ͳ�����
	static int width, height, noX, noY;

	Status(bool player);

	Status(const Status &preStatus, int x, int y);

	~Status();

	std::pair<int, int> getPlace();

	Status *nextChildStatus();

	std::pair<int, int> decideNext(bool isMyFirst);

	float defaultPolicy();
};
