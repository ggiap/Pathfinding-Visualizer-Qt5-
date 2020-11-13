#include "A_star.h"


A_star::A_star(Grid &grid, Node &_start, Node &_finish, int _heuristicChoice)
	: SearchAlgorithm(grid, _start, _finish)
{
	heuristicChoice = _heuristicChoice;

#ifdef _DEBUG
	printf("Start(%.1f, %.1f)", start->cell.getPosition().x, start->cell.getPosition().y);
	printf("  Finish(%.1f, %.1f)\n", finish->cell.getPosition().x, finish->cell.getPosition().y);
	printf("Cell size: (%.f x %.f)\n", (map->nodes.at(0).cell.getSize().x), (map->nodes.at(0).cell.getSize().y));
	printf("HeuristicCost: %.1f \n", Break_Ties_Heuristic(*start));
	printf("Map size: %zu \n\n", map->nodes.size());
#endif // DEBUG

	for (auto &node : map->nodes)
	{
		node.Clear();
	}
}

void A_star::Start_Search(sf::RenderWindow &window, bool allowDiagonal)
{
	open.push_back(start);
	start->fCost = Break_Ties_Heuristic(*start);

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

		Node *current = Lowest_fCost_Node();
		if (*current == *finish)
		{
			reconstruct_path(current);
			return;
		}

		auto it = find(open.begin(), open.end(), current);
		if (it != open.end())
			open.erase(it);
		closed.push_back(current);

		// Handle horizontal and vertical neighbours
		for (auto neighbour = current->adjacent.begin(); neighbour != current->adjacent.end(); ++neighbour)
		{
			if (find(closed.begin(), closed.end(), *neighbour) != closed.end())
				continue;

			double tentative_gCost = current->gCost + GetDistance(*current, *(*neighbour));

			if ((find(open.begin(), open.end(), *neighbour)) == open.end())
				open.push_back(*neighbour);
			else if (tentative_gCost >= (*neighbour)->gCost)
				continue;

			(*neighbour)->previous = current;
			(*neighbour)->gCost = tentative_gCost;
			(*neighbour)->fCost = (*neighbour)->gCost + Break_Ties_Heuristic(*(*neighbour));
		}

		if (allowDiagonal)
		{
			// Handle diagonal neighbours
			for (auto neighbour = current->diagonally_adjacent.begin(); neighbour != current->diagonally_adjacent.end(); ++neighbour)
			{
				if (find(closed.begin(), closed.end(), *neighbour) != closed.end())
					continue;

				double tentative_gCost = current->gCost + 5 + GetDistance(*current, *(*neighbour));

				if ((find(open.begin(), open.end(), *neighbour)) == open.end())
					open.push_back(*neighbour);
				else if (tentative_gCost >= (*neighbour)->gCost)
					continue;

				(*neighbour)->previous = current;
				(*neighbour)->gCost = tentative_gCost;
				(*neighbour)->fCost = (*neighbour)->gCost + Break_Ties_Heuristic(*(*neighbour));
			}
		}

	// ==========Drawing======================
		for (auto &node : open)
		{
			if (node->value != 1 &&
				node->nodeIndex != start->nodeIndex &&
				node->nodeIndex != finish->nodeIndex)
			{
				node->cell.setFillColor(sf::Color::Cyan);
			}
		}
		window.clear(sf::Color::Black);
		for (size_t i = 0; i < map->nodes.size(); ++i)
			window.draw(map->nodes.at(i).cell);
		window.display();
	// ==============================================
	}
}

Node* A_star::Lowest_fCost_Node()
{
	double min = numeric_limits<double>::max();
	int minIndex = 0;
	for(auto &node : open)
		if (node->fCost < min && node->value == 0)
		{
			min = node->fCost;
			minIndex = node->nodeIndex;
		}
	if (min == numeric_limits<double>::max())
	{
		finish->value = -1;
		return finish;
	}
	return &map->nodes.at(minIndex);
}