#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Graph.h"

class PathfindingApp : public aie::Application {
public:

	PathfindingApp();
	virtual ~PathfindingApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	Graph graph;

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};