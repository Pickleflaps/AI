#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Renderer2D.h"

class Node;
const float m_floatMax = std::numeric_limits<float>::max();

enum NodeType
{
	Grass,
	Forest,
	Mountain,
};

class Edge
{
public:
	//constructor
	Edge(Node* start, Node* end, float cost);

	//member variables
	float cost;
	Node* start;
	Node* end;
};

class Node
{
public:
	Node(glm::vec2 position, glm::vec2 index, NodeType nodeType);

	//Member Variables
	glm::vec2 position;
	glm::vec2 index;

	bool highlighted;

	std::vector<Edge*> connections;
	NodeType nodeType;

	//Functions
	void Render(aie::Renderer2D* spriteBatch);
	void Reset();

	//Scores
	float gScore;

	//Traversal only
	Node* parent;

	static bool compareGScore(Node* a, Node* b)
	{
		return a->gScore < b->gScore;
	}

};

class Graph
{
public:
	Graph();

	//Add Functions
	void AddNode(Node* node);
	void AddConnection(Node* a, Node* b, float cost);

	//Find Functions
	Node* FindNodeAtIndex(int x, int y);

	//Searching Algorithms
	std::vector<Node*> DjikstraSearch(Node* startNode, Node* target);

	//Render Function
	void Render(aie::Renderer2D* spriteBatch);

	std::vector<Node*> nodes;

	~Graph();

private:

};