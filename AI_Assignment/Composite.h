#pragma once
#include "Behaviour.h"
class Composite :
	public Behaviour
{
public:
	Composite();
	~Composite();

	virtual bool execute(GameObject* gameObject, float deltaTime) = 0;

	std::vector<Behaviour*> m_children;

};

