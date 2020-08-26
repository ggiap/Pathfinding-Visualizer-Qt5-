#pragma once
#include "SearchAlgorithm.h"

class GreedyBestFS : public SearchAlgorithm
{
public:
	GreedyBestFS() = default;
	GreedyBestFS(Grid &grid, Node &_start, Node &_finish, int _heuristicChoice);
	virtual void Start_Search(sf::RenderWindow &window, bool allowDiagonal);
};