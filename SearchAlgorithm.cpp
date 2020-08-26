#include "SearchAlgorithm.h"
#include <math.h>


SearchAlgorithm::SearchAlgorithm(Grid &grid, Node &_start, Node &_finish)
	: map(&grid), start(&_start), finish(&_finish)
{
}

double SearchAlgorithm::GetDistance(Node &nodeA, Node &nodeB)
{
	return sqrtl(pow(nodeA.cell.getPosition().x - nodeB.cell.getPosition().x, 2) + // Euclidean distance
		   pow(nodeA.cell.getPosition().y - nodeB.cell.getPosition().y, 2));
}

double SearchAlgorithm::Break_Ties_Heuristic(Node &current)
{
	double heuristic = 0;
	if (heuristicChoice == visualizer::Heuristic_Choice::Manhattan)
	{
		heuristic = abs(current.cell.getPosition().x - finish->cell.getPosition().x) + // Manhattan distance
			abs(current.cell.getPosition().y - finish->cell.getPosition().y);
	}
	else if (heuristicChoice == visualizer::Heuristic_Choice::Chebyshev)
	{
		heuristic = max(abs(current.cell.getPosition().x - finish->cell.getPosition().x),	// Chebyshev distance
			abs(current.cell.getPosition().y - finish->cell.getPosition().y));
	}
	else if (heuristicChoice == visualizer::Heuristic_Choice::Euclidean)
	{
		heuristic = sqrtl(pow(current.cell.getPosition().x - finish->cell.getPosition().x, 2) + // Euclidean distance
			pow(current.cell.getPosition().y - finish->cell.getPosition().y, 2));
	}

	// http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#S7
	// Break ties by preferring paths that are along the straight line from the starting point to the goal
	double dx1 = current.cell.getPosition().x - finish->cell.getPosition().x;
	double dy1 = current.cell.getPosition().y - finish->cell.getPosition().y;
	double dx2 = start->cell.getPosition().x - finish->cell.getPosition().x;
	double dy2 = start->cell.getPosition().y - finish->cell.getPosition().y;
	double cross = abs(dx1 * dy2 - dx2 * dy1);
	heuristic += cross * 0.001;
	return heuristic;
}

void SearchAlgorithm::reconstruct_path(Node *current)
{
	path.push_back(current);
	while (current->previous != nullptr)
	{
		current = current->previous;
		path.push_back(current);
	}
}