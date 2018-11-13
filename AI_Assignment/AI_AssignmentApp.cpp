#include "AI_AssignmentApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "GuardStates.h"

AI_AssignmentApp::AI_AssignmentApp() {

}

AI_AssignmentApp::~AI_AssignmentApp() {

}

bool AI_AssignmentApp::startup() {

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	


	//add Nodes to predator graph 
	for (int i = 0; i < xGridSize; ++i) {
		for (int j = 0; j < yGridSize; ++j) {
			NodeType nodeType = NodeType::Grass;
			int myRandNumber = rand() % 75;
			if (myRandNumber < 10) {
				nodeType = NodeType::Mountain;
			}
			else if (myRandNumber >= 10 && myRandNumber < 40) {
				nodeType = NodeType::Forest;
			}

			Node* node = new Node(glm::vec2(i * tileSize, j * tileSize), glm::vec2(i, j), nodeType);
			node->highlighted = false;
			graphPredator.AddNode(node);
		}
	}
	//add Nodes to prey graph
	for (int i = 0; i < xGridSize; ++i) {
		for (int j = 0; j < yGridSize; ++j) {
			NodeType nodeType = graphPredator.FindNodeAtIndex(i, j)->nodeType;
			// New node because if i refence the predator graph, the connection weights get overwritten.
			// By copying the node type, the layout of the random map is the same.
			Node* node = new Node(glm::vec2(i * tileSize, j * tileSize), glm::vec2(i, j), nodeType);
			graphPrey.AddNode(node);
		}
	}	
	//Add Connections for Predator
	for (auto& a : graphPredator.nodes) {
		for (auto& b : graphPredator.nodes) {
			if (a == b) continue;

			float x = b->position.x - a->position.x;
			float y = b->position.y - a->position.y;
			float squaredDistance = x * x + y * y;
			float distance = sqrt(squaredDistance);
			//for pathfinding, if maxDistance is set to tilesize, pathfinding will be up/down/left/right only.
			float maxDistance = (float)tileSize; //tileSize * 1.5 will allow for diagonal pathfinding.
			//set up terrain costs 
			float terrainCost = 1;
			if (b->nodeType == NodeType::Forest) {
				terrainCost = 3;
			}
			else if (b->nodeType == NodeType::Mountain) {
				terrainCost = 5;
			}
			if (distance <= maxDistance) {
				graphPredator.AddConnection(a, b, squaredDistance * terrainCost);
			}

		}
	}
	//add connections for prey
	for (auto& a : graphPrey.nodes) {
		for (auto& b : graphPrey.nodes) {
			if (a == b) continue;

			float x = b->position.x - a->position.x;
			float y = b->position.y - a->position.y;
			float squaredDistance = x * x + y * y;
			float distance = sqrt(squaredDistance);
			//for pathfinding, if maxDistance is set to tilesize, pathfinding will be up/down/left/right only.
			float maxDistance = tileSize * 1.5f; //this will allow for diagonal pathfinding.
			//set up terrain costs 
			float terrainCost = 1;
			if (b->nodeType == NodeType::Forest) {
				terrainCost = 2;
			}
			else if (b->nodeType == NodeType::Mountain) {
				terrainCost = 3;
			}
			if (distance <= maxDistance) {
				graphPrey.AddConnection(a, b, squaredDistance / terrainCost);
			}

		}
	}

	auto wanderState_Pred = new PatrolState(75);
	auto attackState_Pred = new AttackState(&m_prey, 150);


	float xPos, yPos;
	int x = 0, y = 0;
	for (int i = 0; i < 15; i++) {
		
		//m_predator.getPosition(&xPos, &yPos);
		xPos = x;
		yPos = y;
		Node* startNode = graphPredator.FindNodeAtIndex((int)xPos, (int)yPos);
		x = rand() % xGridSize;
		y = rand() % yGridSize;
		Node* endnode = graphPredator.FindNodeAtIndex(x, y);
		graphPredator.AStarSearch(endnode, startNode, Node::heuristicDistance); //this is backwards because the pathfinding reverses the order when it is finished.
		for (auto t : graphPredator.path) {
			wanderState_Pred->addWaypoint(t->position.x, t->position.y);
		}

		

	}
	//pathfind to the start and repeat
	Node* startNode = graphPredator.FindNodeAtIndex((int)x,(int) y);
	Node* endnode = graphPredator.FindNodeAtIndex(0, 0);
	graphPredator.AStarSearch(endnode, startNode, Node::heuristicDistance);		
	for (auto t : graphPredator.path) {
		wanderState_Pred->addWaypoint(t->position.x, t->position.y);
	}


	Condition* attackRangeCondition = new WithinRangeCondition(&m_prey, tileSize * 4);
	Condition* attackToWanderCondition = new NotCondition(attackRangeCondition);

	auto attackToIdleTransition = new Transition(wanderState_Pred, attackToWanderCondition);
	auto toAttackTransition = new Transition(attackState_Pred, attackRangeCondition);

	attackState_Pred->addTransition(attackToIdleTransition);

	wanderState_Pred->addTransition(toAttackTransition);

	m_FSM_Predator.addState(attackState_Pred);
	m_FSM_Predator.addState(wanderState_Pred);

	m_FSM_Predator.addCondition(attackRangeCondition);
	m_FSM_Predator.addCondition(attackToWanderCondition);

	m_FSM_Predator.addTransition(attackToIdleTransition);
	m_FSM_Predator.addTransition(toAttackTransition);

	m_FSM_Predator.setInitialState(wanderState_Pred);

	m_predator.addBehaviour(&m_FSM_Predator);
	
	//set up prey
	auto idleState_Prey = new IdleState();
	auto wanderState_Prey = new PatrolState(200);
	auto fleeState_Prey = new FleeState(&m_predator, 100);
	
	m_prey.setPosition(getWindowWidth()-32, getWindowHeight()-32);
	m_prey.getPosition(&x, &y);// dont start the prey in the same position as the predator or it will be eaten
	x = x / tileSize;
	y = y / tileSize;
	for (int i = 0; i < 10; i++) {
		xPos = x;
		yPos = y;
		Node* startNode = graphPrey.FindNodeAtIndex((int)xPos, (int)yPos);
		x = rand() % xGridSize;
		y = rand() % yGridSize;
		Node* endnode = graphPrey.FindNodeAtIndex(x, y);
		graphPrey.AStarSearch(endnode, startNode, Node::heuristicDistance); //this is backwards because the pathfinding reverses the order when it is finished.
		for (auto t : graphPrey.path) {
			wanderState_Prey->addWaypoint(t->position.x, t->position.y);
		}
		
	}
	startNode = graphPrey.FindNodeAtIndex((int)x, (int)y);
	endnode = graphPrey.FindNodeAtIndex((getWindowWidth() - 32) / tileSize, (getWindowHeight() - 32) / tileSize);
	graphPrey.AStarSearch(endnode, startNode, Node::heuristicDistance);
	for (auto t : graphPrey.path) {
		wanderState_Prey->addWaypoint(t->position.x, t->position.y);
	}

	Condition* fleeRangeCondition = new WithinRangeCondition(&m_predator, 0); //tileSize * 4);
	Condition* fleeToIdleCondition = new NotCondition(fleeRangeCondition);
	Condition* wanderToIdleCondition = new FloatGreaterCondition(wanderState_Prey->getTimerPtr(), 3);
	Condition* idleToWanderCondition = new FloatGreaterCondition(idleState_Prey->getTimerPtr(), 2);

	auto fleeToIdleTransition = new Transition(idleState_Prey, fleeToIdleCondition);
	auto toFleeTransition = new Transition(fleeState_Prey, fleeRangeCondition);
	auto idleToWanderTransition = new Transition(idleState_Prey, idleToWanderCondition);
	auto wanderToIdleTransition = new Transition(wanderState_Prey, wanderToIdleCondition);

	idleState_Prey->addTransition(idleToWanderTransition);
	idleState_Prey->addTransition(toFleeTransition);

	wanderState_Prey->addTransition(wanderToIdleTransition);
	wanderState_Prey->addTransition(toFleeTransition);

	fleeState_Prey->addTransition(fleeToIdleTransition);

	m_FSM_Prey.addState(idleState_Prey);
	m_FSM_Prey.addState(wanderState_Prey);
	m_FSM_Prey.addState(fleeState_Prey);

	m_FSM_Prey.addCondition(fleeRangeCondition);
	m_FSM_Prey.addCondition(fleeToIdleCondition);
	m_FSM_Prey.addCondition(wanderToIdleCondition);
	m_FSM_Prey.addCondition(idleToWanderCondition);

	m_FSM_Prey.addTransition(fleeToIdleTransition);
	m_FSM_Prey.addTransition(toFleeTransition);
	m_FSM_Prey.addTransition(idleToWanderTransition);
	m_FSM_Prey.addTransition(wanderToIdleTransition);
		
	m_FSM_Prey.setInitialState(wanderState_Prey);

	m_prey.addBehaviour(&m_FSM_Prey);

	//m_keyboardBehaviour.setSpeed(400);
	//m_prey.addBehaviour(&m_keyboardBehaviour);
	//m_prey.setPosition(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);



	return true;
}

void AI_AssignmentApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void AI_AssignmentApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		int x = ((input->getMouseX() + 16) / 32);
		int y = ((input->getMouseY() + 16) / 32);
		if (x >= xGridSize) { x = xGridSize - 1; }
		if (y >= yGridSize) { y = yGridSize - 1; }

		Node* startNode = graphPredator.FindNodeAtIndex(0, 0);
		Node* endnode = graphPredator.FindNodeAtIndex(x, y);
		graphPredator.AStarSearch(startNode, endnode, Node::heuristicDistance);
		drawPrey = false;
	}
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
		int x = ((input->getMouseX() + 16) / 32);
		int y = ((input->getMouseY() + 16) / 32);
		if (x >= xGridSize) { x = xGridSize - 1; }
		if (y >= yGridSize) { y = yGridSize - 1; }

		Node* startNode = graphPrey.FindNodeAtIndex(0, 0);
		Node* endnode = graphPrey.FindNodeAtIndex(x, y);
		graphPrey.AStarSearch(startNode, endnode, Node::heuristicDistance);
		drawPrey = true;
	}

	
	

	m_predator.update(deltaTime);
	m_prey.update(deltaTime);
	

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void AI_AssignmentApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();



	// draw your stuff here!
	if (drawPrey) {
		for (auto v : graphPrey.nodes) {
			v->Render(m_2dRenderer);
		}
	}
	if (!drawPrey) {
		for (auto t : graphPredator.nodes) {
			t->Render(m_2dRenderer);
		}
	}

	
	
	// output some text, uses the last used colour
	m_2dRenderer->setRenderColour(1, 1, 1, 1);
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	float x, y;
	m_predator.getPosition(&x, &y);
	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	m_2dRenderer->drawCircle(x, y, 10, 0);

	m_prey.getPosition(&x, &y);
	m_2dRenderer->setRenderColour(0, 0, 1, 1);
	m_2dRenderer->drawCircle(x, y, 10, 0);

	// done drawing sprites
	m_2dRenderer->end();
}