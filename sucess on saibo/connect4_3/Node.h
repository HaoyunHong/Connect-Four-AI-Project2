#pragma once
#include "Status.h"

class Node
{
protected:
	// 长子兄弟法建树
	Node *firstChild, *nextSibling, *parent;
	Status *status;
	float value;
	int times;

public:
	Node(Status *state, Node *parent);

	//析构新生成的节点
	~Node();

	int getValue() const;

	int getTimes() const;

	void addDelta(const float delta);

	Node *getFirstChild() const;

	Node *getNextSibling() const;

	Node *getParent() const;

	Status *getStatus() const;

	//设定扩展节点的第一个孩子
	void setFirstChild(Node *firstChild);

	//设定下一个孩子
	void setNextSibling(Node *nextSibling);

	Node* expand();
};