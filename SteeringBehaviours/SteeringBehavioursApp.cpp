#include "SteeringBehavioursApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "aiUtilities.h"

SteeringBehavioursApp::SteeringBehavioursApp() {

}

SteeringBehavioursApp::~SteeringBehavioursApp() {

}

bool SteeringBehavioursApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//set up the player
	m_player.setPosition(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);
	m_keyboardBehaviour.setSpeed(400);
	m_player.addBehaviour(&m_keyboardBehaviour);

	//set up enemy
	for (auto& enemy : m_enemies) {
		enemy.addBehaviour(&m_steeringBehaviour);
		enemy.setMaxForce(400);
		enemy.setMaxVelocity(200);		
		enemy.setWanderData(100, 75, 25);
		srand(1);
		enemy.setPosition(rand() % getWindowWidth(), rand() % getWindowHeight());
	}

	m_steeringBehaviour.addForce(&m_pursue, 0);	
	m_steeringBehaviour.addForce(&m_wander, 1);
	m_steeringBehaviour.addForce(&m_seek, 0);
	m_steeringBehaviour.addForce(&m_flee, 0);
	m_steeringBehaviour.addForce(&m_avoid, 25);

	m_seek.setTarget(&m_player);
	m_flee.setTarget(&m_player);
	m_pursue.setTarget(&m_player);
	m_evade.setTarget(&m_player);
	
	m_avoid.setFeelerLength(50);

	//set up obstacles
	for (int i = 0; i < 10; ++i) {
		Circle c;
		c.x = rand() % (getWindowWidth() - 100) + 50;
		c.y = rand() % (getWindowHeight() - 100) + 50;
		c.r = rand() % 40 + 40;

		m_obstacles.push_back(c);

		m_avoid.addObstacle(c.x, c.y, c.r);
	}

	return true;
}

void SteeringBehavioursApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void SteeringBehavioursApp::update(float deltaTime) {

	m_player.update(deltaTime);
	for (auto& enemy : m_enemies) {
		enemy.update(deltaTime);
	}
	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	
	if (input->wasKeyPressed(aie::INPUT_KEY_F)) {
		m_steeringBehaviour.setWeightForForce(&m_flee, 1);
		m_steeringBehaviour.setWeightForForce(&m_seek, 0.0f);
		m_steeringBehaviour.setWeightForForce(&m_wander, 1.0f);
		m_steeringBehaviour.setWeightForForce(&m_pursue, 0.0f);
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_S)) {
		m_steeringBehaviour.setWeightForForce(&m_seek, 1);
		m_steeringBehaviour.setWeightForForce(&m_pursue, 0.0f);
		m_steeringBehaviour.setWeightForForce(&m_flee, 0.0f);
		m_steeringBehaviour.setWeightForForce(&m_wander, 1.0f);
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_W)) {
		m_steeringBehaviour.setWeightForForce(&m_wander, 1.0f);
		m_steeringBehaviour.setWeightForForce(&m_seek, 0.0f);
		m_steeringBehaviour.setWeightForForce(&m_flee, 0.0f);
		m_steeringBehaviour.setWeightForForce(&m_pursue, 0.0f);
	}
}

void SteeringBehavioursApp::screenWrap(float & x, float & y)
{
	//wrap enemy around the screen
	x = fmod(x, (float)getWindowWidth());
	if (x < 0)
		x += getWindowWidth();
	y = fmod(y, (float)getWindowHeight());
	if (y < 0)
		y += getWindowHeight();

}

void SteeringBehavioursApp::draw() {

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

	screenWrap(x, y);
	m_player.setPosition(x, y);

	float vx, vy;

	//draw the enemy
	for (auto& enemy : m_enemies) {
		enemy.getPosition(&x, &y);

		m_2dRenderer->setRenderColour(1, 0, 0);
		m_2dRenderer->drawCircle(x, y, 5, 5);

		screenWrap(x, y);
		enemy.setPosition(x, y);
		
		enemy.getVelocity(&vx, &vy);

		float magSqr = vx*vx + vy*vy;
		if (magSqr > 0) {
			magSqr = sqrt(magSqr);
			vx /= magSqr;
			vy /= magSqr;

			vx *= 50;
			vy *= 50;
			m_2dRenderer->drawLine(x, y, x + vx, y + vy);
		}
	}
	m_2dRenderer->setRenderColour(1, 0, 1, 0.25f);
	for (auto& obstacle : m_obstacles)
	{
		m_2dRenderer->drawCircle(obstacle.x, obstacle.y, obstacle.r);
	}

	// output some text, uses the last used colour

	m_2dRenderer->setRenderColour(1, 1, 1);
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

