#pragma once
#include "SearchAlgorithm.h"

class Dijkstra : public SearchAlgorithm
{
public:
	Dijkstra() = default;
	Dijkstra(Grid &grid, Node &_start, Node &_finish);

	virtual void Start_Search(sf::RenderWindow &window, bool allowDiagonal);
};