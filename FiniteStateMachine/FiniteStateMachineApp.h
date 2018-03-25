#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "GameObject.h"
#include "KeyboardBehaviour.h"
#include "State.h"

class FiniteStateMachineApp : public aie::Application {
public:

	FiniteStateMachineApp();
	virtual ~FiniteStateMachineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	GameObject			m_player;
	KeyboardBehaviour	m_keyboardBehaviour;

	GameObject			m_enemy;
	FiniteStateMachine	m_guardFSM;
};