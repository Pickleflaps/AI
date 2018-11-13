#include "DecisionTreesApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include <KeyboardBehaviour.h>
#include <BooleanDecisionNode.h>
#include "WanderDecisionNode.h"
#include "AttackDecisionNode.h"
#include "SeekDecisionNode.h"
#include "Condition.h"


DecisionTreesApp::DecisionTreesApp() {

}

DecisionTreesApp::~DecisionTreesApp() {

}

bool DecisionTreesApp::startup() {

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	aie::Texture* texture = new aie::Texture("./textures/ship.png");

	// Setup player
	KeyboardBehaviour kb;
	playerAgent.addBehaviour(&kb);

	// Setup AI Agent
	//aiAgent = new GameObject(texture, 400.0f, 400.0f);
	//to be controlled by the decision tree

	//BooleanDecisionNode* rootNode = new BooleanDecisionNode();
	//BooleanDecisionNode* checkWithinAttackRange = new BooleanDecisionNode();
	//WanderDecisionNode* wanderDecisionNode = new WanderDecisionNode();
	//SeekDecisionNode* seekDecisionNode = new SeekDecisionNode(playerAgent, 50.0f);
	//AttackDecisionNode* attackDecisionNode = new AttackDecisionNode();
	//
	//rootNode->condition = new WithinRangeCondition(playerAgent, 200.0f);
	//rootNode->trueNode = checkWithinAttackRange;
	//rootNode->falseNode = wanderDecisionNode;
	//
	//checkWithinAttackRange->condition = new WithinRangeCondition(playerAgent, 50.0f);
	//checkWithinAttackRange->trueNode = attackDecisionNode;
	//checkWithinAttackRange->falseNode = seekDecisionNode;
	//
	//aiAgent->addBehaviour(rootNode);




	return true;
}

void DecisionTreesApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void DecisionTreesApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	playerAgent.update(deltaTime);
	//aiAgent->update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void DecisionTreesApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}