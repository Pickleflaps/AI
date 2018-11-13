#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "GameObject.h"



class DecisionTreesApp : public aie::Application {
public:

	DecisionTreesApp();
	virtual ~DecisionTreesApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	GameObject			playerAgent;
	GameObject*			aiAgent;



};

