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
					
					//if (target->gScore > currentNode->gScore + edge->cost) {
						target->gScore = currentNode->gScore + edge->cost;
						target->parent = currentNode;
					//}
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
	for (auto edge : connections)
		spriteBatch->drawLine(position.x, position.y, edge->end->position.x, edge->end->position.y);

	if (highlighted)
		renderColour = glm::vec4(1, 0, 0, 1);
	spriteBatch->setRenderColour(renderColour.r, renderColour.g, renderColour.b, renderColour.a);
	spriteBatch->drawBox(position.x, position.y, 10, 10);

}

void Node::Reset()
{
	gScore = 9999;
	parent = NULL;
	highlighted = false;
}

Edge::Edge(Node * start, Node * end, float cost)
{
	this->start = start;
	this->end = end;
	this->cost = cost;
}
