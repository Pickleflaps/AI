#include "Selector.h"



Selector::Selector()
{
}


Selector::~Selector()
{
}

bool Selector::execute(GameObject * gameObject, float deltaTime)
{
	for (Behaviour* child : m_children) {
		if (child->execute(gameObject, deltaTime)) {
			return false;
		}
	}
	return true;
}
