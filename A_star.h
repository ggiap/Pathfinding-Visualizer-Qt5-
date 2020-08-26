#pragma once
#include "SearchAlgorithm.h"

class A_star : public SearchAlgorithm
{
public:
	A_star() = default;
	A_star(Grid &grid, Node &_start, Node &_finish, int _heuristicChoice);

	virtual void Start_Search(sf::RenderWindow &window, bool allowDiagonal);
	Node* Lowest_fCost_Node();
};