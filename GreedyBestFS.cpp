#include "GreedyBestFS.h"


GreedyBestFS::GreedyBestFS(Grid &grid, Node &_start, Node &_finish, int _heuristicChoice)
	:SearchAlgorithm(grid, _start, _finish)
{
	heuristicChoice = _heuristicChoice;

	for (auto &node : map->nodes)
	{
		node.Clear();
	}
}

void GreedyBestFS::Start_Search(sf::RenderWindow &window, bool allowDiagonal)
{
	start->hCost = Break_Ties_Heuristic(*start);
	open.push_back(start);

	while (!open.empty())
	{
		//Make window able to respond to events
		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return;
			}
		}

		sort(open.begin(), open.end(), [](Node *a, Node *b) {return a->hCost > b->hCost; });
		Node *n = open.at(open.size() - 1);
		open.pop_back();
		closed.push_back(n);

		if (n->nodeIndex == finish->nodeIndex)
			reconstruct_path(n);

		for (auto neighbour : n->adjacent)
		{
			if (neighbour->nodeIndex == finish->nodeIndex)
			{
				finish->previous = n;
				reconstruct_path(finish);
				return;
			}
			if (find(closed.begin(), closed.end(), neighbour) == closed.end() &&
				find(open.begin(), open.end(), neighbour) == open.end() &&
				neighbour->value != 1)
			{
				neighbour->hCost = Break_Ties_Heuristic(*neighbour);
				neighbour->previous = n;
				open.push_back(neighbour);
			}
		}
		if (allowDiagonal)
		{
			for (auto neighbour : n->diagonally_adjacent)
			{
				if (neighbour->nodeIndex == finish->nodeIndex)
				{
					finish->previous = n;
					reconstruct_path(finish);
					return;
				}
				if (find(closed.begin(), closed.end(), neighbour) == closed.end() &&
					find(open.begin(), open.end(), neighbour) == open.end() &&
					neighbour->value != 1)
				{
					neighbour->hCost = Break_Ties_Heuristic(*neighbour);
					neighbour->previous = n;
					open.push_back(neighbour);
				}
			}
		}
		// ==========Drawing======================
		for (auto &node : open)
			if (node->cell.getFillColor() != sf::Color::Cyan &&
				node->value != 1 && node->nodeIndex != start->nodeIndex &&
				node->nodeIndex != finish->nodeIndex)
			{
				node->cell.setFillColor(sf::Color::Cyan);
			}
		window.clear(sf::Color::Black);
		for (size_t i = 0; i < map->nodes.size(); ++i)
			window.draw(map->nodes.at(i).cell);
		window.display();
		// ==============================================
	}
}