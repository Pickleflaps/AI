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

//weighted steering force
struct WeightedForce {
	SteeringForce* force;
	float weight;
};

class SteeringBehaviour : public Behaviour {
public:
	SteeringBehaviour() {}
	virtual ~SteeringBehaviour() {}

	void addForce(SteeringForce* force, float weight = 1.0f) {
		m_forces.push_back({ force, weight });
	}

	void setWeightForForce(SteeringForce* force, float weight) {
		for (auto& wf : m_forces) {		 
			if (wf.force == force)
				wf.weight = weight;
		}
	}

	virtual bool execute(GameObject* gameObject, float deltaTime) {
	
		Force force = { 0,0 };
		for (auto& wf : m_forces)
		{
			Force temp = wf.force->getForce(gameObject);

			//accumulate forces
			force.x += temp.x * wf.weight;
			force.y += temp.y * wf.weight;
		}

		gameObject->addForce(force.x * deltaTime, force.y * deltaTime);		
		return true;
	}

protected:

	std::vector<WeightedForce> m_forces;

};

//Steering behaviour for FSM
class SteeringState : public State {
public:

	SteeringState() {}
	virtual ~SteeringState() {}

	void addForce(SteeringForce* force, float weight = 1.0f) {
		m_forces.push_back({ force, weight });
	}

	void setWeightForForce(SteeringForce* force, float weight) {
		for (auto& wf : m_forces) {
			if (wf.force == force)
				wf.weight = weight;
		}
	}

	virtual void update(GameObject* gameObject, float deltaTime) {

		Force force = { 0,0 };
		for (auto& wf : m_forces)
		{
			Force temp = wf.force->getForce(gameObject);

			//accumulate forces
			force.x += temp.x * wf.weight;
			force.y += temp.y * wf.weight;
		}

		gameObject->addForce(force.x * deltaTime, force.y * deltaTime);
	}

protected:

	std::vector<WeightedForce> m_forces;

};

class SeekForce : public SteeringForce {
public: 
	
	SeekForce(GameObject* target = nullptr) : m_target(target) {}
	virtual ~SeekForce() {}

	void setTarget(GameObject* target) { m_target = target; }

	virtual Force getForce(GameObject* gameObject) const;

protected:
	
	GameObject* m_target;

};

class FleeForce : public SteeringForce {
public:

	FleeForce(GameObject* target = nullptr) : m_target(target) {}
	virtual ~FleeForce() {}

	void setTarget(GameObject* target) { m_target = target; }

	virtual Force getForce(GameObject* gameObject) const;

protected:

	GameObject* m_target;

};

class PursueForce : public SteeringForce {
public:

	PursueForce(GameObject* target = nullptr) : m_target(target) {}
	virtual ~PursueForce() {}

	void setTarget(GameObject* target) { m_target = target; }

	virtual Force getForce(GameObject* gameObject) const;

protected:

	GameObject* m_target;

};

class EvadeForce : public SteeringForce {
public:

	EvadeForce(GameObject* target = nullptr) : m_target(target) {}
	virtual ~EvadeForce() {}

	void setTarget(GameObject* target) { m_target = target; }

	virtual Force getForce(GameObject* gameObject) const;

protected:

	GameObject* m_target;

};

class WanderForce : public SteeringForce {
public:
	WanderForce() {}
	virtual ~WanderForce() {}

	virtual Force getForce(GameObject* gameObject) const;

protected:

	GameObject* m_target;
};


struct Obstacle {
	float x, y, r;
};

class ObstacleAvoidanceForce : public SteeringForce {
	
public:
	ObstacleAvoidanceForce() {}
	virtual ~ObstacleAvoidanceForce() {}

	void setFeelerLength(float length) { m_feelerLength = length; }

	void addObstacle(float x, float y, float radius) {
		m_obstacles.push_back({ x, y, radius });
	}
	void clearObstacles() { m_obstacles.clear(); }

	virtual Force getForce(GameObject* gameObject) const;

public:

	float m_feelerLength;

	std::vector<Obstacle> m_obstacles;

};