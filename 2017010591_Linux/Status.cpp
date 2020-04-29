#include "Status.h"


Status::Status(bool player)
{
	this->player = player;
	chessState = new uint[width];
	top = new uint[width];
	memset(chessState, 0, width * sizeof(uint));
	memset(top, 0, width * sizeof(uint));
	if (noX == 0) top[noY] = 1; //映射过后，如果最底层不能下，就在noY列的底层的上一层下
	nextChildColumn = 0;

	//board是二维数组
	board = new int*[Status::height];
	for (int i = 0; i < Status::height; i++)
		board[i] = new int[Status::width];
	srand((uint)time(0));
}

Status::Status(const Status &preStatus, int x, int y)
{
	this->place.first = x;
	this->place.second = y;
	player = 1 - preStatus.player; //获得当前的player
	chessState = new uint[width];
	top = new uint[width];
	//将之前的二进制表示的每列棋盘状态赋值给现在的棋盘
	memcpy(chessState, preStatus.chessState, width * sizeof(uint));
	memcpy(top, preStatus.top, width * sizeof(uint));

	chessState[y] |= player ? 0 : (1 << x);//改变此时的棋盘
	top[y] = (uint)((x + 1 == noX && y == noY) ? (x + 2) : (x + 1));//更新top
	// 下一步的列
	nextChildColumn = 0;
	// 判断终态
	toBoard();
	// 若此时player方的对手一定赢
	if ((!player && machineWin(height - place.first - 1, place.second, height, width, board))
		|| (player && userWin(height - place.first - 1, place.second, height, width, board)))
		nextChildColumn = width;//设成刚越界的值，说明下一步不存在了
	else
	{
		// 确定子状态下一步可以下哪里，顺序寻找子状态
		for (; nextChildColumn < width; ++nextChildColumn)
			if (top[nextChildColumn] < height)
				break;
	}
}

Status::~Status()
{
	delete[] chessState;
	delete[] top;
}

std::pair<int, int> Status::randomPut()
{
	// 可以走的位置
	int cnt = 0;
	for (int i = 0; i < width; ++i)
		if (top[i] < height)
			cnt++;

	// 没有可以走的位置了
	if (cnt == 0)
		return std::make_pair(-1, -1);

	// 随机找到一个可以走的位置
	cnt = rand() % cnt;

	for (int i = 0; i < width; ++i)
		if (top[i] < height)
		{
			// 取第cnt个可以走的位置
			if (cnt-- == 0)
				return std::make_pair(top[i], i);
		}
	return std::make_pair(-1, -1);

}

std::pair<int, int> Status::orderPut()
{
	std::pair<int, int> &&location = std::make_pair((int)top[nextChildColumn], nextChildColumn);
	nextChildColumn++;
	//  找到当前节点的下一个可能的孩子
	for (; nextChildColumn < width; ++nextChildColumn)
		if (top[nextChildColumn] < height)
			break;
	return location;
}

// 再转化成二维数组board
void Status::toBoard()
{
	for (int i = 0; i < height; i++)
	{
		memset(board[i], 0, width * sizeof(int));
	}

	// 将二维棋盘上的信息复原
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < top[j]; i++)
		{
			if (!(i == noX && j == noY))
			{
				board[height - i - 1][j] = (((1 << i) & chessState[j]) > 0) + 1;
			}
		}
	}
}

std::pair<int, int> Status::getPlace()
{
	return place;
}

Status* Status::nextChildStatus()
{
	//此时下一步没得下了
	if (nextChildColumn == width) return nullptr;

	// 按顺序放
	std::pair<int, int> location = orderPut();

	return new Status(*this, location.first, location.second);
}

// 对根进行紧要的模拟
std::pair<int, int> Status::decideNext(bool isMyFirst)
{
	if (isMyFirst)
	{
		if (top[width / 2] == 0)
		{
			return std::make_pair(top[width / 2], width / 2);
		}
		else if (top[width / 2 - 1] == 0)
		{
			return std::make_pair(top[width / 2 - 1], width / 2 - 1);
		}
		else if (width / 2 - 1 == noY)
		{
			return std::make_pair(top[width / 2 + 1], width / 2 + 1);
		}
		else
		{
			return std::make_pair(top[width / 2 - 2], width / 2 - 2);
		}
	}

	toBoard();
	for (int j = 0; j < width; ++j)
	{
		if (top[j] < height)
		{
			//std::cerr << "(" << top[j] << ", " << j << ")" << std::endl;
			// 如果走了这步player方赢了,那当前的player一定会赢
			board[height - 1 - top[j]][j] = 2;
			if (machineWin(height - 1 - top[j], j, height, width, board))
			{
				//std::cerr << "machine will win at (" << top[j] << ", " << j << ")" << std::endl;
				return std::make_pair(top[j], j);
			}
			board[height - 1 - top[j]][j] = 0;
		}
	}
	// 如果走这步我方并不会赢
	for (int j = 0; j < width; ++j)
	{
		if (top[j] < height)
		{
			//std::cerr << "(" << top[j] << ", " << j << ")" << std::endl;
			// 如果走了这步player方赢了,那当前的player一定会赢
			board[height - 1 - top[j]][j] = 1;
			// 考察走这步对方会不会赢
			if (userWin(height - 1 - top[j], j, height, width, board))
			{
				// 如果对方会赢那我一定要走这步
				//std::cerr << "user will win at (" << this->top[j] << ", " << j << ")" << std::endl;
				return std::make_pair(top[j], j);
			}
			board[height - 1 - top[j]][j] = 0;
		}
	}

	// 如果找遍了以后，那些情况还是都没有
	// 对当前的top备份
	// 再往后考虑一步
	uint *_top = new uint[width];
	memcpy(_top, this->top, width * sizeof(uint));
	for (int j = 0; j < width; ++j)
	{
		if (top[j] < height)
		{
			board[height - 1 - top[j]][j] = 1;
			_top[j] += 1;
			int winCnt = 0;
			for (int k = 0; k < width; ++k)
			{
				if (_top[k] < height)
				{
					board[height - 1 - _top[k]][k] = 1;
					if (userWin(height - 1 - _top[k], k, height, width, board))
					{
						winCnt++;
						if (winCnt > 1)
						{
							//std::cerr << "user will win at (" << _top[k] << ", " << k << ")" << std::endl;
							//std::cerr << "user will win next at (" << top[j] << ", " << j << ")" << std::endl;
							return std::make_pair(top[j], j);
						}
					}
					board[height - 1 - _top[k]][k] = 0;
				}
			}
			board[height - 1 - top[j]][j] = 0;
			_top[j] -= 1;
		}
	}

	for (int j = 0; j < width; ++j)
	{
		if (top[j] < height)
		{
			board[height - 1 - top[j]][j] = 2;
			_top[j] += 1;
			int winCnt = 0;
			for (int k = 0; k < width; ++k)
			{
				if (_top[k] < height)
				{
					board[height - 1 - _top[k]][k] = 2;
					if (userWin(height - 1 - _top[k], k, height, width, board))
					{
						winCnt++;
						if (winCnt > 1)
						{
							//std::cerr << "machine will win at (" << _top[k] << ", " << k << ")" << std::endl;
							//std::cerr << "machine will win next at (" << top[j] << ", " << j << ")" << std::endl;
							return std::make_pair(top[j], j);
						}
					}
					board[height - 1 - _top[k]][k] = 0;
				}
			}
			board[height - 1 - top[j]][j] = 0;
			_top[j] -= 1;
		}
	}

	return std::make_pair(-1, -1);
}

float Status::defaultPolicy()
{
	// 这是说明之前就已经输了
	if (nextChildColumn == width)
	{
		return -1;
	}

	bool _player = this->player;
	std::pair<int, int> location;
	toBoard();

	uint *_top = new uint[width];
	memcpy(_top, this->top, width * sizeof(uint));

	int farCnt = 0;
	while ((location = randomPut()).first != -1)
	{
		farCnt++;
		this->top[location.second] = (uint)((location.first + 1 == noX && location.second == noY) ? (location.first + 2) : (location.first + 1));

		// 再转换为二维矩阵
		board[height - location.first - 1][location.second] = _player + 1;

		// 如果已经可以决定胜负，就不用再模拟了
		if ((_player && machineWin(height - location.first - 1, location.second, height, width, board))
			|| (!_player && userWin(height - location.first - 1, location.second, height, width, board)))
		{
			break;
		}

		// 如果现在还是不能决定胜负，那先看下一步
		_player = 1 - _player; // 现在本方换人了
	}

	// 恢复top
	memcpy(this->top, _top, width * sizeof(uint));
	delete[] _top;

	// 越近得到结果的，越重要
	float delta = expf(1 - farCnt);
	//  没地方可下，就是Tie / 本player方 Win / Lose
	return location.first == -1 ? 0 : (_player == this->player ? 1+ delta : -1- delta);
}