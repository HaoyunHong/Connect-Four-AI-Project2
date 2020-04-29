#pragma once
#include "Node.h"

class UCT
{
protected:
	Node *root;

public:
	static clock_t startTime;

	// ��ʼ�����ڵ�
	UCT(bool player) :root(new Node(new Status(player), nullptr))
	{}

	~UCT()
	{
		delete root;
	}

	float UCB1(Node *parent, Node *child);

	Node *bestChild(Node *parent);

	void backTrace(Node *node, int value);

	Node *treePolicy();

	void moveRoot(std::pair<int, int> put);

	std::pair<int, int> UCTSearch(bool isMyFirst);
};
