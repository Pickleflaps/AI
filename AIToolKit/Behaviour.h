#pragma once
#include "GameObject.h"

class Behaviour
{
public:
	// abstract class
	Behaviour() {};
	~Behaviour() {};
	//pure virtual function for executing the behaviour
	virtual bool execute(GameObject* gameObject, float deltaTime) = 0;

private:

};

