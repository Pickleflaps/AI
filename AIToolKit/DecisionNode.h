#pragma once
#include "Behaviour.h"

class DecisionNode	: public Behaviour
{
public:
	DecisionNode();
	~DecisionNode();

	virtual void MakeDecision(GameObject* agent, float deltaTime) = 0;
	virtual bool execute(GameObject* agent, float deltaTime) { MakeDecision(agent, deltaTime);	return true; };


private:

};

DecisionNode::DecisionNode()
{
}

DecisionNode::~DecisionNode()
{
}