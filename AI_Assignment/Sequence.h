#pragma once
#include "Composite.h"
class Sequence :
	public Composite
{
public:
	Sequence();
	~Sequence();

	virtual bool execute(GameObject* gameObject, float deltaTime);
};

