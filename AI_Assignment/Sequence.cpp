#include "Sequence.h"



Sequence::Sequence()
{
}


Sequence::~Sequence()
{
}

bool Sequence::execute(GameObject * gameObject, float deltaTime)
{
	for (auto child : m_children) {
		if (child->execute(gameObject, deltaTime)) {
			return true;
		}
	}
	return false;
}
