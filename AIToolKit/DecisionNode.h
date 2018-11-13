#pragma once
#include "Behaviour.h"

class DecisionNode	: public Behaviour
{
public:
	DecisionNode() {}
	virtual ~DecisionNode() {}

	virtual bool MakeDecision(GameObject* agent, float deltaTime) = 0;
	virtual bool execute(GameObject* agent, float deltaTime) { MakeDecision(agent, deltaTime); };


private:

};