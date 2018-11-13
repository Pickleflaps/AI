#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Renderer2D.h"
#include <functional>

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
	bool searched;

	std::vector<Edge*> connections;
	NodeType nodeType;
	
	//Functions
	void Render(aie::Renderer2D* spriteBatch);
	void Reset();

	//Scores
	float gScore;
	float hScore; //Heuristic value. if left at 0, behaviour will be like Djikstra's 
	float fScore;

	//Traversal only
	Node* parent;

	static bool compareGScore(Node* a, Node* b) {
		return a->gScore < b->gScore;
	}

	static bool compareFScore(Node* a, Node* b) {
		return a->fScore < b->fScore;
	}

	static bool heuristicDjikstras(Node* a, Node* b) {
		return 0; // djikstra's equivilant
	}

	static float heuristicDistance(Node* a, Node* b) {
		float x = b->position.x - a->position.x;
		float y = b->position.y - a->position.y;
		return sqrt(x * x + y * y); // this is the slowest 
	}


	static float heuristicManhattan(Node* a, Node*b) {
		return (b->index.x - a->index.x) + (b->index.y - a->index.y); // this
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

	//
	//	A*
	//
	typedef std::function<float(Node*a, Node*b)> HeuristicCheck;
	std::vector<Node*> AStarSearch(Node* startNode, Node* endNode, HeuristicCheck heuristic);

	void Reset();
	int GetNodeSize() { return (int)nodes.size(); }

	//Render Function
	void Render(aie::Renderer2D* spriteBatch);

	std::vector<Node*> nodes;
	std::vector<Node*> path;

	~Graph();

private:

};