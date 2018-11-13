#include "WanderDecisionNode.h"
#include <glm/common.hpp>
#include <glm/ext.hpp>

WanderDecisionNode::WanderDecisionNode()
{
}


WanderDecisionNode::~WanderDecisionNode()
{
}


void WanderDecisionNode::MakeDecision(GameObject * gameObject, float deltaTime)
{
	glm::vec2 jitterOffset = glm::circularRand(gameObject->getWanderJitter());
	float wanderX = 0, wanderY = 0;
	gameObject->getWanderTarget(&wanderX, &wanderY);

	// apply the jitter
	wanderX += jitterOffset.x;
	wanderY += jitterOffset.y;

	//bring is back to a radius around the game object
	float magnitude = sqrt(wanderX * wanderX + wanderY * wanderY);
	// apply the radius
	wanderX = (wanderX / magnitude) * gameObject->getWanderRadius();
	wanderY = (wanderY / magnitude) * gameObject->getWanderRadius();

	//store the target beck into the game object
	gameObject->setWanderTarget(wanderX, wanderY);

	//access the game object's velocity as a unit vector (normalised)
	float vx = 0, vy = 0;
	gameObject->getVelocity(&vx, &vy);

	//normalise and protect from divide-by-zero
	magnitude = (vx * vx + vy * vy);
	if (magnitude > 0)
	{
		magnitude = sqrt(magnitude);
		vx /= magnitude;
		vy /= magnitude;
	}

	//combine velocity direction with wander target to offset
	wanderX += vx * gameObject->getWanderOffset();
	wanderY += vy * gameObject->getWanderOffset();

	//normalise the new direction
	magnitude = wanderX * wanderX + wanderY * wanderY;
	if (magnitude > 0) {

		magnitude = sqrt(magnitude);
		wanderX /= magnitude;
		wanderY /= magnitude;

	}

	gameObject->addForce(wanderX * gameObject->getMaxForce(), wanderY * gameObject->getMaxForce());


}
