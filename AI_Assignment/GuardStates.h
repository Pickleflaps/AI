#pragma once

#include "State.h"
#include <glm\ext.hpp>


class AttackState : public State
{
public: 

	AttackState(GameObject* target, float speed) : m_target(target), m_speed(speed)	{}
	virtual ~AttackState()	{}

	void addWaypoint(float x, float y) {
		Location loc = { x, y };
		m_warpath.emplace_back(loc); }
	void clearPath() { m_warpath.clear(); }

	virtual void	onEnter(GameObject* gameObject) {
		if (!m_warpath.empty())
			clearPath();



	}

	void setTarget(GameObject* gameObject) { m_target = gameObject; }
	virtual void	update(GameObject* gameObject, float deltaTime) {

		if (m_target == nullptr)
			return; 
		if (m_warpath.empty()) // then just seek the target
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
			float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

			//if not at the target, then move towards it
			if (distance > 0)
			{
				xDiff /= distance;
				yDiff /= distance;
				gameObject->translate(xDiff * m_speed * deltaTime, yDiff * m_speed * deltaTime);
			}
		}
		else // we have a path
		{
			Location loc = m_warpath[m_currentTarget];

			//get my position
			float x = 0, y = 0;
			gameObject->getPosition(&x, &y);

			//compare the two and get the distance between them
			float xDiff = loc.x - x;
			float yDiff = loc.y - y;
			float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

			//if not at the target, then move towards it
			if (distance > 1)
			{
				xDiff /= distance;
				yDiff /= distance;
				gameObject->translate(xDiff * m_speed * deltaTime, yDiff * m_speed * deltaTime);
			}
			else
			{
				//go to next target
				if (++m_currentTarget >= m_warpath.size())
					clearPath();
			}
		}
	}

protected:

	struct Location {
		float x, y;
	};
	
private:

	float			m_speed;
	GameObject*		m_target;
	int				m_currentTarget;
	std::vector<Location> m_warpath;
};

class IdleState : public State {
public:

	IdleState() {}
	virtual ~IdleState() {}

	virtual void	update(GameObject* gameObject, float deltaTime) { return; }


};

class WanderState : public State {
public:
	WanderState() {}
	virtual ~WanderState() {}

	virtual void update(GameObject* gameObject, float deltaTime) {
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

		gameObject->translate( wanderX * gameObject->getMaxForce(), wanderY * gameObject->getMaxForce());

	}
};

class PatrolState : public State {
public:
	PatrolState(float speed) : m_currentTarget(0), m_speed(speed) {}
	virtual ~PatrolState() {}

	void addWaypoint(float x, float y) {
		Location loc = { x, y };
		m_locations.push_back(loc);
	}

	virtual void	update(GameObject* gameObject, float deltaTime) {
		if (m_locations.empty())
			return;

		Location loc = m_locations[m_currentTarget];

		//get my position
		float x = 0, y = 0;
		gameObject->getPosition(&x, &y);

		//compare the two and get the distance between them
		float xDiff = loc.x - x;
		float yDiff = loc.y - y;
		float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

		//if not at the target, then move towards it
		if (distance > 2)
		{
			xDiff /= distance;
			yDiff /= distance;
			gameObject->translate(xDiff * m_speed * deltaTime, yDiff * m_speed * deltaTime);
		}
		else
		{
			//go to next target
			if (++m_currentTarget >= m_locations.size())
				m_currentTarget = 0;
		}

	}

	bool isLastLocation() { return m_currentTarget == m_locations.size(); }
	void clearLocations() { m_locations.clear(); }


protected:

	struct Location {
		float x, y;
	};


	float					m_speed;
	unsigned int			m_currentTarget;
	std::vector<Location>	m_locations;

};

// FLEESTATE IS BASICALLY ATTACK, however, it should be fed an opposite list of locations
class FleeState : public State { 
public:
	FleeState(GameObject* target, float speed) : m_target(target), m_speed(speed) {}
	FleeState(float speed) : m_target(nullptr), m_speed(speed) {}
	virtual ~FleeState() {}

	void addWaypoint(float x, float y) { 
		Location loc = { x, y }; 
		m_fleePath.emplace_back(loc); }
	void clearPath() { m_fleePath.clear(); }

	void setTarget(GameObject* gameObject) { m_target = gameObject; }

	virtual void update(GameObject* gameObject, float deltaTime) {
		if (m_target == nullptr)
			return;
		if (m_fleePath.empty()) // then just seek the target
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
			float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

			xDiff /= distance;
			yDiff /= distance;

			gameObject->translate(-xDiff * m_speed * deltaTime, -yDiff * m_speed * deltaTime);
			
		}
		else // we have a path
		{
			Location loc = m_fleePath[m_currentTarget];

			//get my position
			float x = 0, y = 0;
			gameObject->getPosition(&x, &y);

			//compare the two and get the distance between them
			float xDiff = loc.x - x;
			float yDiff = loc.y - y;
			float distance = sqrt(xDiff * xDiff + yDiff * yDiff);

			//if not at the target, then move towards it
			if (distance > 1)
			{
				xDiff /= distance;
				yDiff /= distance; 
				gameObject->translate(xDiff * m_speed * deltaTime, yDiff * m_speed * deltaTime);
			}
			else
			{
				//go to next target
				if (++m_currentTarget >= m_fleePath.size())
					clearPath();
			}
		}
	}

protected:

	struct Location
	{
		float x, y;
	};

private:

	GameObject*				m_target;
	float					m_speed;
	int						m_currentTarget;
	std::vector<Location>	m_fleePath;
};