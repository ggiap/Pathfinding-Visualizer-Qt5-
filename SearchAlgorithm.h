#pragma once
#include <iostream>
#include "Grid.h"


class SearchAlgorithm
{
public:
	SearchAlgorithm() = default;
	SearchAlgorithm(Grid &grid, Node &_start, Node &_finish);

	virtual void Start_Search(sf::RenderWindow &window, bool allowDiagonal) = 0;
	virtual double GetDistance(Node &nodeA, Node &nodeB);
	double Break_Ties_Heuristic(Node &current);
	void reconstruct_path(Node *current);

	Grid *map;
	Node *start;
	Node *finish;
	vector<Node *> open;
	vector<Node *> closed;
	vector<Node *> path;
	int heuristicChoice;
};

namespace visualizer
{
	enum Alg_Choice { Random = 1, GreedyBFS, Astar, Dijkstra };
	enum Heuristic_Choice { Manhattan = 1, Chebyshev, Euclidean };
};
