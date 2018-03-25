#include "KeyboardBehaviour.h"
#include "Input.h"

KeyboardBehaviour::KeyboardBehaviour() : m_speed(1)
{
}

KeyboardBehaviour::~KeyboardBehaviour()
{
}

bool KeyboardBehaviour::execute(GameObject * gameObject, float deltaTime)
{
	float x = 0, y = 0;

	//get access to input
	aie::Input* input = aie::Input::getInstance();

	//determine the direction to move
	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		y += 1;
	}
	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		y -= 1;
	}
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		x -= 1;
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		x += 1;
	}

	//we need to adjust the distance when heading diagonally
	float magnitude = sqrt(x*x + y*y);
	if (magnitude > 0)
	{
		x /= magnitude;
		y /= magnitude;
	}

	gameObject->translate(x * m_speed * deltaTime, y * m_speed * deltaTime);

	return false;
}
