#include "Node.h"

Node::Node(Status* _status, Node* _parent)
{
	status = _status;
	parent = _parent;
	value = 0;
	times = 0;
	firstChild = nextSibling = nullptr;
}

//�����ڵ�
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

//�趨��չ�ڵ�ĳ���
void Node::setFirstChild(Node* _firstChild)
{
	firstChild = _firstChild;
}

//�趨�ڵ���ֵ�
void Node::setNextSibling(Node* _nextSibling)
{
	nextSibling = _nextSibling;
}

Node* Node::expand()
{
	Status *newStatus = status->nextChildStatus();

	// ��ʱ���޷���չ���ѵ��յ�
	if (newStatus == nullptr)
		return nullptr;

	Node *newNode = new Node(newStatus, this);

	if (firstChild == nullptr) // ����û�г���
	{
		firstChild = newNode;
	}
	else
	{	// �����г��ӣ����Ž����Ӻ��ֵ�������
		newNode->setNextSibling(firstChild);
		firstChild = newNode;
	}
	return newNode;
}