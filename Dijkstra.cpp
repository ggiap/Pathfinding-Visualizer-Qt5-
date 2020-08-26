#include "Dijkstra.h"
#include <limits>


Dijkstra::Dijkstra(Grid &grid, Node &_start, Node &_finish)
	: SearchAlgorithm(grid, _start, _finish)
{
	for (auto &node : map->nodes)
	{
		node.Clear();
	}
}

void Dijkstra::Start_Search(sf::RenderWindow &window, bool allowDiagonal)
{
	for (auto &node : map->nodes)
	{
		//node.gCost = INFINITY;
		node.gCost = std::numeric_limits<double>::infinity();
	}

	start->gCost = 0;
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

		sort(open.begin(), open.end(), [](Node *a, Node *b) { return a->gCost > b->gCost; });
		Node *current = open.at(open.size() - 1);
		open.pop_back();

		for (auto &neighbour : current->adjacent)
		{
			if (neighbour->nodeIndex == finish->nodeIndex)
			{
				finish->previous = current;
				reconstruct_path(finish);
				return;
			}
			if (find(open.begin(), open.end(), neighbour) == open.end() &&
				neighbour->value != 1)
			{
				double move_distance = current->gCost + GetDistance(*current, *neighbour);

				if (move_distance < neighbour->gCost)
				{
					neighbour->gCost = move_distance;
					neighbour->previous = current;
					open.push_back(neighbour);
				}
			}
		}

		if (allowDiagonal)
		{
			for (auto &neighbour : current->diagonally_adjacent)
			{
				if (neighbour->nodeIndex == finish->nodeIndex)
				{
					finish->previous = current;
					reconstruct_path(finish);
					return;
				}

				if (find(open.begin(), open.end(), neighbour) == open.end() &&
					neighbour->value != 1)
				{
					double move_distance = current->gCost + GetDistance(*current, *neighbour);

					if (move_distance < neighbour->gCost)
					{
						neighbour->gCost = move_distance;
						neighbour->previous = current;
						open.push_back(neighbour);
					}
				}
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
