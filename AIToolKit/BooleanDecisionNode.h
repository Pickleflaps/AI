#pragma once
#include "DecisionNode.h"
#include "Condition.h"

class BooleanDecisionNode : public DecisionNode
{
public:

	BooleanDecisionNode() {}
	~BooleanDecisionNode() {}

	DecisionNode* trueNode;
	DecisionNode* falseNode;
	Condition* condition;

	virtual bool MakeDecision(GameObject* agent, float deltaTime);


};
