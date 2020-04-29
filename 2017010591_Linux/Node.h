#pragma once
#include "Status.h"

class Node
{
protected:
	// �����ֵܷ�����
	Node *firstChild, *nextSibling, *parent;
	Status *status;
	float value;
	int times;

public:
	Node(Status *state, Node *parent);

	//���������ɵĽڵ�
	~Node();

	int getValue() const;

	int getTimes() const;

	void addDelta(const float delta);

	Node *getFirstChild() const;

	Node *getNextSibling() const;

	Node *getParent() const;

	Status *getStatus() const;

	//�趨��չ�ڵ�ĵ�һ������
	void setFirstChild(Node *firstChild);

	//�趨��һ������
	void setNextSibling(Node *nextSibling);

	Node* expand();
};