#pragma once
#include "DecisionNode.h"



class SeekDecisionNode :
	public DecisionNode
{
public:
	SeekDecisionNode(GameObject* target, float maxSpeed);
	~SeekDecisionNode();

	void MakeDecision(GameObject* gameObject, float deltaTime);

protected:

	GameObject* m_target;
	float		m_maxSpeed;

};

