#pragma once
#include "Behaviour.h"

class FollowBehaviour : public Behaviour
{
public:
	FollowBehaviour();
	~FollowBehaviour();

	virtual bool execute(GameObject* gameObject, float deltaTime);

	void setSpeed(float speed) { m_speed = speed; };
	void setTarget(GameObject* target) { m_target = target; };

private:

	float			m_speed;
	GameObject*		m_target;

};
