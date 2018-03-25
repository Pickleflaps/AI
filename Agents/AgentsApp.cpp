#include "AgentsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

AgentsApp::AgentsApp() {

}

AgentsApp::~AgentsApp() {

}

bool AgentsApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//behaviours
	m_keyboardBehaviour.setSpeed(400);

	m_followBehaviour.setSpeed(250);
	m_followBehaviour.setTarget(&m_player);

	//agents
	m_player.addBehaviour(&m_keyboardBehaviour);
	m_player.setPosition(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);

	m_enemy.addBehaviour(&m_followBehaviour);
	m_enemy.setPosition(10, 10);

	return true;
}

void AgentsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void AgentsApp::update(float deltaTime) {
	
	m_player.update(deltaTime);
	m_enemy.update(deltaTime);

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	

}

void AgentsApp::draw() {

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
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();

}
