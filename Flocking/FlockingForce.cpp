#include "FlockingForce.h"



FlockingForce::FlockingForce()
{
}


FlockingForce::~FlockingForce()
{
}

void FlockingForce::execute(GameObject * gameObject, float deltaTime)
{
	SeperationForce();
	AlignmentForce();
	CohesionForce();
}

void FlockingForce::NeighborhoodWatch(std::vector<GameObject*> gameObjects)
{

}

void FlockingForce::SeperationForce()
{

}

void FlockingForce::AlignmentForce()
{
}

void FlockingForce::CohesionForce()
{
}
