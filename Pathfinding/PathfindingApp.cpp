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
	//m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	
	//add nodes to graph
	for (int i = 0; i < xGridSize; ++i) {
		for (int j = 0; j < yGridSize; ++j)
		{
			//TODO: assign an image based on node type
			NodeType nodeType = NodeType::Grass;
			float myRandNumber(rand() % 100);
			if (myRandNumber < 10)
			{
				nodeType = NodeType::Mountain;
			}
			if (myRandNumber > 10 && myRandNumber < 40)
			{
				nodeType = NodeType::Forest;
			}

			Node* node = new Node(glm::vec2(i * 32, j * 32), glm::vec2(i, j), nodeType);
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

			float maxDistance = 48;
			
			//TODO: ADD A COST PER NODE TYPE RATHER THAN DISTANCE
			//EG: IF DISTANCE IS GREATER THAN OR EQUAL TO MAX DISTANCE ASSIGN A COST BASED ON CONNECTION TYPE 
			//EG: GRASS MAY BE DISTANCE, FOREST = DISTANCE * 2 AND MOUNTAIN = DISTANCE * 5
			float terrainCost = 1;
			if (b->nodeType == NodeType::Forest)
				terrainCost = 2;
			if (b->nodeType == NodeType::Mountain)
				terrainCost = 5;
			if (dist <= maxDistance)
				graph.AddConnection(a, b, sqrDist * terrainCost);

		}
	}

	//perform a Djikstra's search
	Node* startNode = graph.FindNodeAtIndex(0, 0); //set this to the players location
	Node* endnode = graph.FindNodeAtIndex(40, 20); // set this to the node (use find node at location)

	graph.AStarSearch(startNode, endnode, Node::heuristicDistance); // call this on mouse release


	//std::vector<Node*> nodes = graph.DjikstraSearch(startNode, endnode);

	//for (auto node : nodes)
		//node->highlighted = true;

	return true;
}

void PathfindingApp::shutdown() {

	//delete m_font;
	//delete m_2dRenderer;
}

void PathfindingApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	float x, y;
	m_2dRenderer->getCameraPos(x, y);
	/* moving the camera 
	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_2dRenderer->setCameraPos(x, y - 5);
	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_2dRenderer->setCameraPos(x, y + 5);
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_2dRenderer->setCameraPos(x + 5, y);
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_2dRenderer->setCameraPos(x - 5, y);
		*/
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		int x = ((input->getMouseX() + 16) / 32);
		int y = ((input->getMouseY() + 16) / 32);
		if (x >= xGridSize) { x = xGridSize - 1; }
		if (y >= yGridSize) { y = yGridSize - 1; }

		Node* startNode = graph.FindNodeAtIndex(0, 0); //set this to the players location
		Node* endnode = graph.FindNodeAtIndex(x, y); // set this to the node (use find node at location)

		graph.AStarSearch(startNode, endnode, Node::heuristicDistance); // call this on mouse release
	}
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PathfindingApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();


	// draw your stuff here!
	for (auto v : graph.nodes) {
		v->Render(m_2dRenderer);
	}	
	
	// output some text, uses the last used colour
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}