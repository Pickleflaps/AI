#include "PathfindingApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"


PathfindingApp::PathfindingApp() {

}

PathfindingApp::~PathfindingApp() {

}

bool PathfindingApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	int xGridSize = 20;
	int yGridSize = 20;
	
	//add nodes to graph
	for (int i = 0; i < xGridSize; ++i) {
		for (int j = 0; j < yGridSize; ++j)
		{
			Node* node = new Node(glm::vec2(i * 32, j * 32), glm::vec2(i, j), NodeType::Grass);
			graph.AddNode(node);
		}
	}

	//Add Connections
	for (auto a : graph.nodes) {
		for (auto b : graph.nodes) {
			if (a == b) continue;

			float x = b->position.x - a->position.x;
			float y = b->position.y - a->position.y;
			float sqrDist = x * x + y * y;
			float dist = sqrt(sqrDist);

			float maxDistance = 48.0f;
			
			if (dist <= maxDistance)
				graph.AddConnection(a, b, sqrDist);
		}
	}

	//perform a Djikstra's search
	Node* startNode = graph.FindNodeAtIndex(0, 0);
	Node* endnode = graph.FindNodeAtIndex(9, 3);

	std::vector<Node*> nodes = graph.DjikstraSearch(startNode, endnode);

	for (auto node : nodes)
		node->highlighted = true;

	return true;
}

void PathfindingApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PathfindingApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PathfindingApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	m_2dRenderer->setCameraPos(-50, -50);
	// draw your stuff here!
	for (auto v : graph.nodes) {
		v->Render(m_2dRenderer);
	}	
	
	// output some text, uses the last used colour
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}