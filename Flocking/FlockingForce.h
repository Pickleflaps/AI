#pragma once
#include "Behaviour.h"

class FlockingForce : public Behaviour
{
public:
	FlockingForce();
	~FlockingForce();

	virtual void execute(GameObject* gameObject, float deltaTime);

	void NeighborhoodWatch(std::vector<GameObject*> gameObjects);
	void SeperationForce();
	void AlignmentForce();
	void CohesionForce();

	std::vector<GameObject*> Neighborhood;

	

};

