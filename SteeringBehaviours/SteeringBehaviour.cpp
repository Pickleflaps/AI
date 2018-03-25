#include "SteeringBehaviour.h"
#include <glm\ext.hpp>
#include "aiUtilities.h"


Force SeekForce::getForce(GameObject * gameObject) const
{

	//get the target position
	float tx = 0, ty = 0;
	m_target->getPosition(&tx, &ty);

	//get my position
	float x = 0, y = 0;
	gameObject->getPosition(&x, &y);

	//compare the two and get the distance between them
	float xDiff = tx - x;
	float yDiff = ty - y;
	//SQUARED distance
	float distance = xDiff * xDiff + yDiff * yDiff;

	//if not at the target, then move towards it
	if (distance > 0)
	{
		distance = sqrt(distance);
		xDiff /= distance;
		yDiff /= distance;
		
	}

	return Force{ xDiff * gameObject->getMaxForce(), yDiff * gameObject->getMaxForce() };
}

Force FleeForce::getForce(GameObject * gameObject) const
{
	//get the target position
	float tx = 0, ty = 0;
	m_target->getPosition(&tx, &ty);

	//get my position
	float x = 0, y = 0;
	gameObject->getPosition(&x, &y);

	//compare the two and get the distance between them
	float xDiff = x - tx;
	float yDiff = y - ty;
	//SQUARED distance
	float distance = xDiff * xDiff + yDiff * yDiff;

	//if not at the target, then move towards it
	if (distance > 0)
	{
		distance = sqrt(distance);
		xDiff /= distance;
		yDiff /= distance;

	}

	return Force{ xDiff * gameObject->getMaxForce(), yDiff * gameObject->getMaxForce() };
}

Force PursueForce::getForce(GameObject * gameObject) const
{
	//get the target position
	float tx = 0, ty = 0;
	m_target->getPosition(&tx, &ty);

	//get my position
	float x = 0, y = 0;
	gameObject->getPosition(&x, &y);

	//get velocity
	float vx, vy;
	m_target->getVelocity(&vx, &vy);

	tx += vx;
	ty += vy;

	//compare the two and get the distance between them
	float xDiff = tx - x;
	float yDiff = ty - y;

	//SQUARED distance
	float distance = xDiff * xDiff + yDiff * yDiff;

	//if not at the target, then move towards it
	if (distance > 0)
	{
		distance = sqrt(distance);
		xDiff /= distance;
		yDiff /= distance;

	}

	return Force{ xDiff * gameObject->getMaxForce(), yDiff * gameObject->getMaxForce() };
}

Force EvadeForce::getForce(GameObject * gameObject) const
{
	//get the target position
	float tx = 0, ty = 0;
	m_target->getPosition(&tx, &ty);

	//get my position
	float x = 0, y = 0;
	gameObject->getPosition(&x, &y);

	//get velocity
	float vx, vy;
	m_target->getVelocity(&vx, &vy);

	tx += vx;
	ty += vy;

	//compare the two and get the distance between them
	float xDiff = x - tx;
	float yDiff = y - ty;

	//SQUARED distance
	float distance = xDiff * xDiff + yDiff * yDiff;

	//if not at the target, then move towards it
	if (distance > 0)
	{
		distance = sqrt(distance);
		xDiff /= distance;
		yDiff /= distance;

	}

	return Force{ xDiff * gameObject->getMaxForce(), yDiff * gameObject->getMaxForce() };
}

Force WanderForce::getForce(GameObject * gameObject) const {
	//
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
	
	return Force{ wanderX * gameObject->getMaxForce(), wanderY * gameObject->getMaxForce() };

}

Force ObstacleAvoidanceForce::getForce(GameObject * gameObject) const
{
	Force force = { 0,0 };

	//create the "feeler"
	float x, y, vx, vy;
	gameObject->getPosition(&x, &y);
	gameObject->getVelocity(&vx, &vy);

	float ix, iy, t;

	float magSqr = vx * vx + vy * vy;
	if (magSqr > 0) {

		//loop through all obstacles and find collisions
		for (auto& obstacle : m_obstacles) {
			if (rayCircleIntersection(x, y,
				vx, vy,
				obstacle.x, obstacle.y, obstacle.r,
				ix, iy,
				&t)) {
				//within range?
				if (t >= 0 &&  //a collision occured
					t <= m_feelerLength) { //and the intersection is within range

					
					force.x += (ix - obstacle.x) / obstacle.r;
					force.y += (iy - obstacle.y) / obstacle.r;
					
				}
			}
		}
	}

	magSqr = force.x * force.x + force.y * force.y;
	if (magSqr > 0) {
		magSqr = sqrt(magSqr);
		force.x /= magSqr;
		force.y /= magSqr;
	}

	return { force.x * gameObject->getMaxForce(), force.y * gameObject->getMaxForce() };
}
