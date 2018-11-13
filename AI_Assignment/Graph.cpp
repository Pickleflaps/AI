#include "Graph.h"

Graph::Graph()
{
}

void Graph::AddNode(Node * node)
{
	nodes.push_back(node);
}

void Graph::AddConnection(Node * a, Node * b, float cost)
{
	//make a new edge from 'a' to 'b' and add it to the connections vector
	Edge* edge = new Edge(a, b, cost);

	a->connections.push_back(edge);
}

Node * Graph::FindNodeAtIndex(int x, int y)
{
	for (auto node : nodes)
	{
		if (node->index.x == x && node->index.y == y)
		{
			return node;
		}
	}
	return nullptr;
}

std::vector<Node*> Graph::DjikstraSearch(Node * startNode, Node * target)
{

	//Reset parent pointers and G Scores
	for (auto node : nodes)	{
		node->Reset();
	}

	//Let openlist be a list of nodes
	std::list<Node*> openList;
	std::set<Node*> closedList;

	//Set startNode's parent to null
	startNode->parent = NULL;
	startNode->gScore = 0;
	

	//Add start node to list
	openList.push_back(startNode);

	//while openlist is not empty
	while (openList.empty() == false)
	{
		//sort openList by Node.gScore
		openList.sort(Node::compareGScore);

		//let currentNode = first item in openList
		Node* currentNode = openList.front();
		//target->h = (target->position.x - currentNode->position.x) + (target->position.y - currentNode->position.y);
		
		//Process the node
		if (currentNode == target)
			break;

		//remove currentNode from the openList
		openList.pop_front();
		//Add currentNode to closedList
		closedList.insert(currentNode);	

		//For all edges in currentNode
		for (auto edge : currentNode->connections)
		{
			Node* target = edge->end;

			//Add edge's target to openList is not already in closed list
			if (closedList.find(target) == closedList.end()) // Target not in closed list
			{
				//also make sure it is not on the open list to be safe
				auto iter = std::find(openList.begin(), openList.end(), target);

				if (iter == openList.end())
				{
					openList.push_back(target);
					
					if (target->gScore > currentNode->gScore + edge->cost) {
						target->gScore = currentNode->gScore + edge->cost;
						target->parent = currentNode;
					}
				}
				//calculate gScore
				float gScore = currentNode->gScore + edge->cost;
			}
		}
	}
	std::vector<Node*> path;

	Node* currentPathNode = target;

	while (currentPathNode != NULL)
	{
		path.push_back(currentPathNode);
		currentPathNode = currentPathNode->parent;
	}

	return path;
}

std::vector<Node*> Graph::AStarSearch(Node * startNode, Node * endNode, HeuristicCheck heuristic)
{
	Reset();

	//let openlist be a list of nodes
	std::list<Node*> openList;
	std::set<Node*> closedList;

	//set startNode's parent to null
	startNode->parent = NULL;
	startNode->gScore = 0;
	startNode->hScore = heuristic(startNode, endNode);
	startNode->fScore = startNode->gScore + startNode->hScore;

	//Add startNode to parent list
	openList.push_back(startNode);

	//while openist is not empty
	while (openList.empty() == false) {

		//sort openList by Node.fScore
		openList.sort(Node::compareFScore);

		//let currentNode = first item in the openList
		Node * currentNode = openList.front();

		//process the current node
		if (currentNode == endNode)
			break;

		//remove currentNode from the openList
		openList.pop_front();
		//add currentNode to the closedList
		closedList.insert(currentNode);

		//for all edges in currentNode
		for (auto edge : currentNode->connections) {
			Node* target = edge->end;

			//Add edge's target to openList if not already in closedList
			if (closedList.find(target) == closedList.end()) { // target is not in the closedList
				//Also make sure it is not in the openList to be safe
				auto iter = std::find(openList.begin(), openList.end(), target);
				if (iter == openList.end()) {
					openList.push_back(target);
					target->searched = true;
				}

				//calculate gScore
				float gScore = currentNode->gScore + edge->cost;
				float hScore = heuristic(target, endNode);
				float fScore = gScore + hScore;

				if (target->fScore > fScore) {
					target->gScore = gScore;
					target->hScore = hScore;
					target->fScore = fScore;
					target->parent = currentNode;
				}
			}


		}
	}
	//std::vector<Node*> path;
	if (!path.empty()) {
		path.clear();
	}
	Node* currentPathNode = endNode;

	while (currentPathNode != NULL)
	{
		path.push_back(currentPathNode);
		currentPathNode = currentPathNode->parent;
	}

	for (auto node : path)
		node->highlighted = true;

	return path;
}


void Graph::Reset()
{
	for (auto node : nodes) {
		node->Reset();
	}

}

void Graph::Render(aie::Renderer2D * spriteBatch)
{
	for (auto node : nodes)
		node->Render(spriteBatch);
}

Graph::~Graph()
{
	//TODO destroy stuff here
}

Node::Node(glm::vec2 position, glm::vec2 index, NodeType nodeType)
{
	this->position = position;
	this->index = index;
	this->nodeType = nodeType;
}

void Node::Render(aie::Renderer2D * spriteBatch)
{
	glm::vec4 renderColour = glm::vec4(1);
	spriteBatch->setRenderColour(renderColour.r, renderColour.g, renderColour.b, renderColour.a);
	//render connections'
	//for (auto edge : connections)
		//spriteBatch->drawLine(position.x, position.y, edge->end->position.x, edge->end->position.y);

	if (this->nodeType == NodeType::Forest)
		renderColour = glm::vec4(0, 0.81640625, 0.1484375, 1);
	if (this->nodeType == NodeType::Grass)
		renderColour = glm::vec4(0.75390625, .99, 0.27734375, 1);
	if (this->nodeType == NodeType::Mountain)
		renderColour = glm::vec4(0.546875, 0.171875, 0, 1);
	if (highlighted)
		renderColour += glm::vec4(0, 0, .5, 1);
	
	spriteBatch->setRenderColour(renderColour.r, renderColour.g, renderColour.b, renderColour.a);
	spriteBatch->drawBox(position.x, position.y, 32, 32);

}

void Node::Reset()
{
	gScore = m_floatMax;
	hScore = m_floatMax;
	fScore = m_floatMax;
	parent = NULL;
	highlighted = false;
	searched = false;
}

Edge::Edge(Node * start, Node * end, float cost)
{
	this->start = start;
	this->end = end;
	this->cost = cost;
}
