#pragma once
#include "DecisionNode.h"

class WanderDecisionNode :
	public DecisionNode
{
public:
	WanderDecisionNode();

	~WanderDecisionNode();

	void MakeDecision(GameObject* gameObject, float deltaTime);

};

