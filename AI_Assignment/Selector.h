#pragma once
#include "Composite.h"
class Selector :
	public Composite
{
public:
	Selector();
	~Selector();

	virtual bool execute(GameObject* gameObject, float deltaTime);


};

