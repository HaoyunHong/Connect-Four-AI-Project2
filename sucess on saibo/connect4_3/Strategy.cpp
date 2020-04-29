#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"

using namespace std;

// �����ض���
std::pair<int, int> place;
int** Status::board;
clock_t UCT::startTime;
int Status::height,
Status::width,
Status::noX,
Status::noY;
UCT *chessTree = nullptr;

/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
extern "C" Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below

	// ��ʼ��ʱ
	UCT::startTime = clock();

	// ��������б任��ԭ�������µķ���
	int myLastX = M - 1 - lastX, 
		myLastY = lastY, 
		myNoX = M - 1 - noX, 
		myNoY = noY;

	//  �ж��Ƿ�Ϊ�µĿ�ʼ
	if (isNewGame(_board, M, N))
	{
		initGame(M, N, myNoX, myNoY);

		// ���ǰһ��������
		if (lastX == -1 && lastY == -1)
		{
			// ˵�����ҷ�����
			chessTree = new UCT(1);
		}
		else
		{
			// ˵���ǶԷ�����
			chessTree = new UCT(0);

			// �ȶԶԷ�������չ��Ȼ��õ��Է���ѡ����Ϊ�µĸ�
			chessTree->moveRoot(std::make_pair(myLastX, myLastY));
		}

		// �õ��ҷ���Ҫ�ߵ���һ��
		place = chessTree->UCTSearch(true);

		// ������任����
		x = M - 1 - place.first;
		y = place.second;
	}
	else
	{		
		// ���Ƶ��Լ�����һ���������ϲ���
		chessTree->moveRoot(place);
		// ���Ƶ��Է��ǲ������ϲ���
		chessTree->moveRoot(std::make_pair(myLastX, myLastY));

		// �õ��ҷ���Ҫ�ߵ���һ��
		place = chessTree->UCTSearch(false);

		// ������任����
		x = M - 1 - place.first;
		y = place.second;
	}
	
	
	/*
		��Ҫ������δ���
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" void clearPoint(Point* p)
{
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/
bool isNewGame(const int *_board, int M, int N)
{
	int cnt = 0;
	for (int i = M * N - 1; i > 0; --i)
	{
		cnt += _board[i];
		if (cnt > 1) 
		{
			return false;
		}
	}
	return true;
}

void initGame(int M, int N, int noX, int noY)
{
	Status::height = M;
	Status::width = N;
	Status::noX = noX;
	Status::noY = noY;
	if (chessTree != nullptr)
	{
		delete chessTree;
	}	
}

