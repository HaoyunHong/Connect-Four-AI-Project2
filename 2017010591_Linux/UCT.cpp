#include "UCT.h"

float UCT::UCB1(Node *parent, Node *child)
{
	int &&N = child->getTimes(), &&Q = child->getValue();
	return -(float)Q / N + C * sqrtf(2 * logf(parent->getTimes()) / N);
}

Node *UCT::bestChild(Node *parent)
{
	float maxValue = -INF;
	Node *bestChild = nullptr;
	for (Node *child = parent->getFirstChild(); child != nullptr; child = child->getNextSibling())
	{
		float &&ucb1 = UCB1(parent, child);

		// 选择对我方最好的子节点
		if (ucb1 > maxValue)
		{
			maxValue = ucb1;
			bestChild = child;
		}
	}
	return bestChild;
}

void UCT::backTrace(Node *node, int value)
{
	// 己方是正值，对方是负值
	while (node != nullptr)
	{
		node->addDelta(value);
		value = -value;
		node = node->getParent();
	}
}

Node* UCT::treePolicy()
{
	Node *current = root, *newNode;

	// 曾扩展过，已扩展完成
	while ((newNode = current->expand()) == nullptr && current->getFirstChild() != nullptr)
	{
		// 每一步都选最好的孩子
		current = bestChild(current);
	}

	// 如果current没有孩子，走current，如果有孩子但还没完全扩展，走孩子
	return current->getFirstChild() == nullptr ? current : newNode;
}

void UCT::moveRoot(std::pair<int, int> place)
{
	// 如果根还没有expand，先把能expand都expand
	if (root->getFirstChild() == nullptr)
	{
		Node *newNode;
		while (true)
		{
			newNode = root->expand();
			if (newNode == nullptr)
				break;
		}
	}

	Node *newRoot;
	for (newRoot = root->getFirstChild(); newRoot != nullptr; newRoot = newRoot->getNextSibling())
	{
		// 找到新根的位置
		if (newRoot->getStatus()->getPlace() == place)
			break;
	}
	// 把新根的兄弟给删掉
	for (Node *child = root->getFirstChild(), *next; child != nullptr; child = next)
	{
		next = child->getNextSibling();
		if (child != newRoot)
			delete child;
	}
	root->setFirstChild(newRoot);
	newRoot->setNextSibling(nullptr);
	root = newRoot;
}

std::pair<int, int> UCT::UCTSearch(bool isMyFirst)
{

	while ((clock() - startTime)< TIME_LIMIT)
	{
		Node *newNode = treePolicy();

		backTrace(newNode, newNode->getStatus()->defaultPolicy());
	}

	std::pair<int, int> mustPut = root->getStatus()->decideNext(isMyFirst);

	//std::cerr << "mustPut (" << mustPut.first << ", " << mustPut.second << ")" << std::endl;

	int maxTime = -INF;
	Node *bestChild = nullptr;
	for (Node *child = root->getFirstChild(); child != nullptr; child = child->getNextSibling())
	{
		if ((mustPut.first != -1) && (child->getStatus()->getPlace() == mustPut))
		{
			return mustPut;
		}
		if (child->getTimes() > maxTime)
		{
			maxTime = child->getTimes();
			bestChild = child;
		}
	}

	std::pair<int, int> put = bestChild->getStatus()->getPlace();
	return put;
};
