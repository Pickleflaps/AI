#include "BooleanDecisionNode.h"

bool BooleanDecisionNode::MakeDecision(GameObject * agent, float deltaTime)
{
	if (condition->test(agent))
		trueNode->MakeDecision(agent, deltaTime);
	else 
		falseNode->MakeDecision(agent, deltaTime);
		
	return condition->test(agent);
}
