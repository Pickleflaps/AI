#include "GameObject.h"
#include "Behaviour.h"
#include <imgui.h>

GameObject::GameObject() : 
	m_x(0), 
	m_y(0),
	m_vx(0),
	m_vy(0),
	m_maxVelocity(0),
	m_maxForce(0),
	m_wanderOffset(0),
	m_wanderRadius(0),
	m_wanderJitter(0),
	m_wanderX(0),
	m_wanderY(0) {

}

GameObject::GameObject(aie::Texture * tex, float x, float y) :
m_x(0),
m_y(0),
m_vx(0),
m_vy(0),
m_maxVelocity(0),
m_maxForce(0),
m_wanderOffset(0),
m_wanderRadius(0),
m_wanderJitter(0),
m_wanderX(0),
m_wanderY(0),
m_texture(tex),
m_tex_width(x),
m_tex_height(y)
{

}

GameObject::~GameObject()
{
}

void GameObject::addBehaviour(Behaviour * behaviour)
{
	m_behaviours.push_back(behaviour);
}

void GameObject::update(float deltaTime)
{
	//execute all of the behaviours
	for (auto& behaviour : m_behaviours)	
		behaviour->execute(this, deltaTime); 

	float magnitudeSqr = m_vx * m_vx + m_vy * m_vy;
	if (magnitudeSqr > (m_maxVelocity *  m_maxVelocity)) { 
		float magnitude = sqrt(magnitudeSqr);
		m_vx = m_vx / magnitude * m_maxVelocity;
		m_vy = m_vy / magnitude * m_maxVelocity;
	}
	
	m_x += m_vx * deltaTime;
	m_y += m_vy * deltaTime;

}
