#include "Node.h"

Node::Node(Status* _status, Node* _parent)
{
	status = _status;
	parent = _parent;
	value = 0;
	times = 0;
	firstChild = nextSibling = nullptr;
}

//析构节点
Node:: ~Node()
{
	for (Node *child = firstChild, *next; child != nullptr; child = next)
	{
		next = child->nextSibling;
		delete child;
	}
	delete status;
}

int Node::getValue() const { return value; }

int Node::getTimes() const { return times; }

void Node::addDelta(const float delta) { value += delta; times++; }

Node* Node::getFirstChild() const
{
	return firstChild;
}

Node* Node::getNextSibling() const
{
	return nextSibling;
}

Node* Node::getParent() const
{
	return parent;
}

Status* Node::getStatus() const
{
	return status;
}

//设定扩展节点的长子
void Node::setFirstChild(Node* _firstChild)
{
	firstChild = _firstChild;
}

//设定节点的兄弟
void Node::setNextSibling(Node* _nextSibling)
{
	nextSibling = _nextSibling;
}

Node* Node::expand()
{
	Status *newStatus = status->nextChildStatus();

	// 此时已无法扩展，已到终点
	if (newStatus == nullptr)
		return nullptr;

	Node *newNode = new Node(newStatus, this);

	if (firstChild == nullptr) // 若还没有长子
	{
		firstChild = newNode;
	}
	else
	{	// 若已有长子，倒着将长子和兄弟连起来
		newNode->setNextSibling(firstChild);
		firstChild = newNode;
	}
	return newNode;
}