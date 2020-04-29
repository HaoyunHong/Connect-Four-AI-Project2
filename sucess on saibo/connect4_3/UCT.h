#pragma once
#include "Node.h"

class UCT
{
protected:
	Node *root;

public:
	static clock_t startTime;

	// 初始化根节点
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
