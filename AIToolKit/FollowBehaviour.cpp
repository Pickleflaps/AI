#include "FollowBehaviour.h"

FollowBehaviour::FollowBehaviour() 
	: m_speed(1)
	, m_target(nullptr)
{
}

FollowBehaviour::~FollowBehaviour()
{
}

bool FollowBehaviour::execute(GameObject * gameObject, float deltaTime)
{
	if (m_target == nullptr)
		return false; //I like false because we "didn't execute this behaviour"

	//get the target position
	float tx = 0, ty = 0;
	m_target->getPosition(&tx, &ty);

	//get my position
	float x = 0, y = 0;
	gameObject->getPosition(&x, &y);

	//compare the two and get the distance between them
	float xDiff = tx - x;
	float yDiff = ty - y;
	float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

	//if not at the target, then move towards it
	if (distance > 0)
	{
		xDiff /= distance;
		yDiff /= distance;
		gameObject->translate(xDiff * m_speed * deltaTime, yDiff * m_speed * deltaTime);
	}

	return true; // and true is "we did execute this behaviour"
}
