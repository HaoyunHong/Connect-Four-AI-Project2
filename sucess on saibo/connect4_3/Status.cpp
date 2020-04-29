#include "Status.h"


Status::Status(bool player)
{
	this->player = player;
	chessState = new uint[width];
	top = new uint[width];
	memset(chessState, 0, width * sizeof(uint));
	memset(top, 0, width * sizeof(uint));
	if (noX == 0) top[noY] = 1; //ӳ����������ײ㲻���£�����noY�еĵײ����һ����
	nextChildColumn = 0;

	//board�Ƕ�ά����
	board = new int*[Status::height];
	for (int i = 0; i < Status::height; i++)
		board[i] = new int[Status::width];
	srand((uint)time(0));
}

Status::Status(const Status &preStatus, int x, int y)
{
	this->place.first = x;
	this->place.second = y;
	player = 1 - preStatus.player; //��õ�ǰ��player
	chessState = new uint[width];
	top = new uint[width];
	//��֮ǰ�Ķ����Ʊ�ʾ��ÿ������״̬��ֵ�����ڵ�����
	memcpy(chessState, preStatus.chessState, width * sizeof(uint));
	memcpy(top, preStatus.top, width * sizeof(uint));

	chessState[y] |= player ? 0 : (1 << x);//�ı��ʱ������
	top[y] = (uint)((x + 1 == noX && y == noY) ? (x + 2) : (x + 1));//����top
	// ��һ������
	nextChildColumn = 0;
	// �ж���̬
	toBoard();
	// ����ʱplayer���Ķ���һ��Ӯ
	if ((!player && machineWin(height - place.first - 1, place.second, height, width, board))
		|| (player && userWin(height - place.first - 1, place.second, height, width, board)))
		nextChildColumn = width;//��ɸ�Խ���ֵ��˵����һ����������
	else
	{
		// ȷ����״̬��һ�����������˳��Ѱ����״̬
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
	// �����ߵ�λ��
	int cnt = 0;
	for (int i = 0; i < width; ++i)
		if (top[i] < height)
			cnt++;

	// û�п����ߵ�λ����
	if (cnt == 0)
		return std::make_pair(-1, -1);

	// ����ҵ�һ�������ߵ�λ��
	cnt = rand() % cnt;

	for (int i = 0; i < width; ++i)
		if (top[i] < height)
		{
			// ȡ��cnt�������ߵ�λ��
			if (cnt-- == 0)
				return std::make_pair(top[i], i);
		}
	return std::make_pair(-1, -1);

}

std::pair<int, int> Status::orderPut()
{
	std::pair<int, int> &&location = std::make_pair((int)top[nextChildColumn], nextChildColumn);
	nextChildColumn++;
	//  �ҵ���ǰ�ڵ����һ�����ܵĺ���
	for (; nextChildColumn < width; ++nextChildColumn)
		if (top[nextChildColumn] < height)
			break;
	return location;
}

// ��ת���ɶ�ά����board
void Status::toBoard()
{
	for (int i = 0; i < height; i++)
	{
		memset(board[i], 0, width * sizeof(int));
	}

	// ����ά�����ϵ���Ϣ��ԭ
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
	//��ʱ��һ��û������
	if (nextChildColumn == width) return nullptr;

	// ��˳���
	std::pair<int, int> location = orderPut();

	return new Status(*this, location.first, location.second);
}

// �Ը����н�Ҫ��ģ��
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
			// ��������ⲽplayer��Ӯ��,�ǵ�ǰ��playerһ����Ӯ
			board[height - 1 - top[j]][j] = 2;
			if (machineWin(height - 1 - top[j], j, height, width, board))
			{
				//std::cerr << "machine will win at (" << top[j] << ", " << j << ")" << std::endl;
				return std::make_pair(top[j], j);
			}
			board[height - 1 - top[j]][j] = 0;
		}
	}
	// ������ⲽ�ҷ�������Ӯ
	for (int j = 0; j < width; ++j)
	{
		if (top[j] < height)
		{
			//std::cerr << "(" << top[j] << ", " << j << ")" << std::endl;
			// ��������ⲽplayer��Ӯ��,�ǵ�ǰ��playerһ����Ӯ
			board[height - 1 - top[j]][j] = 1;
			// �������ⲽ�Է��᲻��Ӯ
			if (userWin(height - 1 - top[j], j, height, width, board))
			{
				// ����Է���Ӯ����һ��Ҫ���ⲽ
				//std::cerr << "user will win at (" << this->top[j] << ", " << j << ")" << std::endl;
				return std::make_pair(top[j], j);
			}
			board[height - 1 - top[j]][j] = 0;
		}
	}

	// ����ұ����Ժ���Щ������Ƕ�û��
	// �Ե�ǰ��top����
	// ��������һ��
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
	// ����˵��֮ǰ���Ѿ�����
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

		// ��ת��Ϊ��ά����
		board[height - location.first - 1][location.second] = _player + 1;

		// ����Ѿ����Ծ���ʤ�����Ͳ�����ģ����
		if ((_player && machineWin(height - location.first - 1, location.second, height, width, board))
			|| (!_player && userWin(height - location.first - 1, location.second, height, width, board)))
		{
			break;
		}

		// ������ڻ��ǲ��ܾ���ʤ�������ȿ���һ��
		_player = 1 - _player; // ���ڱ���������
	}

	// �ָ�top
	memcpy(this->top, _top, width * sizeof(uint));
	delete[] _top;

	// Խ���õ�����ģ�Խ��Ҫ
	float delta = expf(1 - farCnt);
	//  û�ط����£�����Tie / ��player�� Win / Lose
	return location.first == -1 ? 0 : (_player == this->player ? 1+ delta : -1- delta);
}