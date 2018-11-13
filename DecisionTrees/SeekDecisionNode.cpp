#include "SeekDecisionNode.h"


SeekDecisionNode::SeekDecisionNode(GameObject * target, float maxSpeed)
{
	this->m_target = target;
	this->m_maxSpeed = maxSpeed;
}

SeekDecisionNode::~SeekDecisionNode()
{
}

void SeekDecisionNode::MakeDecision(GameObject * gameObject, float deltaTime)
{
	//seek to your target
	if (m_target == nullptr)
		return;

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
		gameObject->translate(xDiff * m_maxSpeed * deltaTime, yDiff * m_maxSpeed * deltaTime);
	}

}
