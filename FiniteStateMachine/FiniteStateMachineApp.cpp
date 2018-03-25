#include "FiniteStateMachineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "GuardStates.h"

FiniteStateMachineApp::FiniteStateMachineApp() {

}

FiniteStateMachineApp::~FiniteStateMachineApp() {

}

bool FiniteStateMachineApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//behaviours
	m_keyboardBehaviour.setSpeed(400);

	//agents
	m_player.addBehaviour(&m_keyboardBehaviour);
	m_player.setPosition(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);

	//m_enemy.addBehaviour(&m_followBehaviour);
	m_enemy.setPosition(10, 10);

	//guard
	m_enemy.addBehaviour(&m_guardFSM);

	auto idleState = new IdleState();
	auto attackState = new AttackState(&m_player, 150);
	auto patrolState = new PatrolState(75);

	patrolState->addWaypoint(getWindowWidth() * 0.25f, getWindowHeight() * 0.25f);
	patrolState->addWaypoint(getWindowWidth() * 0.25f, getWindowHeight() * 0.75f);
	patrolState->addWaypoint(getWindowWidth() * 0.75f, getWindowHeight() * 0.75f);
	patrolState->addWaypoint(getWindowWidth() * 0.75f, getWindowHeight() * 0.25f);

	Condition* attackTimerCondition = new FloatGreaterCondition(attackState->getTimerPtr(), 5);
	Condition* idleTimerCondition = new FloatGreaterCondition(idleState->getTimerPtr(), 2);
	Condition* distanceCondition = new WithinRangeCondition(&m_player, 200);
	NotCondition* outOfRange = new NotCondition(distanceCondition);

	auto attackToIdleTransition = new Transition(idleState, outOfRange);
	auto toAttackTransition = new Transition(attackState, distanceCondition);
	auto idleToPatrol = new Transition(patrolState, idleTimerCondition);
	

	//attack to idle
	attackState->addTransition(attackToIdleTransition);

	//idle to attack
	idleState->addTransition(idleToPatrol);
	idleState->addTransition(toAttackTransition);

	//patrol to attack
	patrolState->addTransition(toAttackTransition);
	
	m_guardFSM.addState(attackState);
	m_guardFSM.addState(idleState);
	m_guardFSM.addState(patrolState);

	m_guardFSM.addCondition(distanceCondition);
	m_guardFSM.addCondition(idleTimerCondition);
	m_guardFSM.addCondition(attackTimerCondition);
	m_guardFSM.addCondition(outOfRange);

	m_guardFSM.addTransition(attackToIdleTransition);
	m_guardFSM.addTransition(toAttackTransition);
	m_guardFSM.addTransition(idleToPatrol);

	m_guardFSM.setInitialState(patrolState);

	return true;
}

void FiniteStateMachineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void FiniteStateMachineApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	m_player.update(deltaTime);
	m_enemy.update(deltaTime);


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FiniteStateMachineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	float x = 0, y = 0;
	//draw the player
	m_player.getPosition(&x, &y);
	m_2dRenderer->setRenderColour(0, 1, 0);
	m_2dRenderer->drawCircle(x, y, 10, 10);

	//draw the enemy
	m_enemy.getPosition(&x, &y);
	m_2dRenderer->setRenderColour(1, 0, 0);
	m_2dRenderer->drawCircle(x, y, 5, 5);
	
	// output some text, uses the last used colour
	m_2dRenderer->setRenderColour(1, 1, 1);
	m_2dRenderer->drawText(m_font, "Press ESC to quit.  Use arrow keys to move.", 10, 10);

	// done drawing sprites
	m_2dRenderer->end();
}