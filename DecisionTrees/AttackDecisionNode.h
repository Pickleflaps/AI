#pragma once
#include "DecisionNode.h"

class AttackDecisionNode :
	public DecisionNode
{
public:
	AttackDecisionNode();
	~AttackDecisionNode();

	void MakeDecision(GameObject* gameObject, float deltaTime);
};

