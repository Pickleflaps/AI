#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Graph.h"
#include "GameObject.h"
#include "State.h" // this has my FSM
#include "KeyboardBehaviour.h"

class AI_AssignmentApp : public aie::Application {
public:

	AI_AssignmentApp();
	virtual ~AI_AssignmentApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// The graph connections have different weights. if they share a node on one graph. the pointers refernce the same nodes and overwrite the connection data
	Graph graphPredator;
	Graph graphPrey;
	bool drawPrey = false;

	GameObject m_predator;
	GameObject m_prey;

	FiniteStateMachine	m_FSM_Predator;	// FSM TO SET THE STEERING WEIGHTS
	FiniteStateMachine	m_FSM_Prey;

	KeyboardBehaviour	m_keyboardBehaviour;



protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	// Graph sizes
	int xGridSize = 41; 
	int yGridSize = 23; 
	int tileSize = 32;



};

/*
ABSTRACT CLASS FOR BEHAVIOUR WITH EXECUTE FUNCTION
COMPOSITE CLASS DERIVED FROM BEHAVIOUR WITH A LIST OF CHILDREN, EXECUTE IS STILL ABSTRACT
SELECTOR CLASS DERIVED FROM COMPOSITE, IMPLEMENT EXECUTE
SEQUENCE CLASS DERIVED FROM COMPOSITE, IMPLEMENT EXECUTE
*/