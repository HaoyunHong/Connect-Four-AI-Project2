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

		// ѡ����ҷ���õ��ӽڵ�
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
	// ��������ֵ���Է��Ǹ�ֵ
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

	// ����չ��������չ���
	while ((newNode = current->expand()) == nullptr && current->getFirstChild() != nullptr)
	{
		// ÿһ����ѡ��õĺ���
		current = bestChild(current);
	}

	// ���currentû�к��ӣ���current������к��ӵ���û��ȫ��չ���ߺ���
	return current->getFirstChild() == nullptr ? current : newNode;
}

void UCT::moveRoot(std::pair<int, int> place)
{
	// �������û��expand���Ȱ���expand��expand
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
		// �ҵ��¸���λ��
		if (newRoot->getStatus()->getPlace() == place)
			break;
	}
	// ���¸����ֵܸ�ɾ��
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
