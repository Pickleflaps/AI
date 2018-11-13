#include "AttackDecisionNode.h"
#include <iostream>



AttackDecisionNode::AttackDecisionNode()
{
}


AttackDecisionNode::~AttackDecisionNode()
{
}

void AttackDecisionNode::MakeDecision(GameObject * gameObject, float deltaTime)
{
	std::cout << "Attack!!!" << std::endl;
}
