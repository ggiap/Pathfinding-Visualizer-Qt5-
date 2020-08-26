/* 
 * THIS IS JUST A RANDOM SELCTED NODE SEARCH FOR TEST PURPOSES!
 */


#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include "Grid.h"

class Search
{
public:
	Grid *map;					// Reference to the grid we are gonna search
	vector<Node *> explored;	// Explored nodes
	vector<Node *> reachable;	// Nodes that are adjacent
	vector<Node *> path;		// Shortest path
	Node *start;				// Start node
	Node *finish;				// Finish node
	size_t iterations;			// Number of iteration our search performs
	bool finished;				// Keep track of whether the search has completed

	Search();
	Search(Grid &grid);

	void Start(Node &_start, Node &_finish);
	void Step(sf::RenderWindow &window, bool allowDiagonal);
	void AddAdjacent(Node &node, Node &adjacent);
	Node* ChooseNode();
};