#pragma once

#include "State.h"

struct Force {
	float x, y;
};

//Pure abstract class
class SteeringForce {

public:
	SteeringForce() {}
	virtual ~SteeringForce() {}

	virtual Force getForce(GameObject* gameObject) const = 0; // pure virtual function
};

class SteeringBehaviour : public Behaviour {
public:
	SteeringBehaviour() {}
	virtual ~SteeringBehaviour() {}

	void setForce(SteeringForce* force) { m_force = force; }

	virtual bool execute(GameObject* gameObject, float deltaTime) { 
	
		if (m_force != nullptr) {
			Force force = m_force->getForce(gameObject);

			gameObject->addForce(force.x * deltaTime, force.y * deltaTime);
			return true;
		}
		return false;
	}

protected:

	SteeringForce* m_force;

};

//Steering behaviour for FSM
class SteeringState : public State {
public:

	SteeringState() : m_force(nullptr) {}
	virtual ~SteeringState() {}

	void setForce(SteeringForce* force) { m_force = force; }

	virtual void update(GameObject* gameObject, float deltaTime) {

		if (m_force != nullptr) {
			Force force = m_force->getForce(gameObject);

			gameObject->addForce(force.x * deltaTime, force.y * deltaTime);
		}
	}

protected:

	SteeringForce* m_force;

};
