#include "Search.h"
#include <algorithm>
#include <iterator>

Search::Search()
{}

Search::Search(Grid &grid)
{
	map = &grid;
	finished = false;
}

void Search::Start(Node &_start, Node &_finish)
{
	reachable.push_back(&_start);
	start = &_start;
	finish = &_finish;
	iterations = 0;

	/*
	 * Clear the search leftovers
	 * in case we have run the
	 * search previously
	 */
	for (size_t i = 0; i < map->nodes.size(); ++i)
	{
		map->nodes.at(i).Clear();
	}
}

void Search::Step(sf::RenderWindow &window, bool allowDiagonal)
{
	//Make window able to respond to events
	sf::Event event;
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			finished = true;
			iterations = 0;
			return;
		}
	}

	if (path.size() > 0)
		return;

	// Check if there is no path to the finish
	if (reachable.size() == 0)
	{
		finished = true;
		iterations = 0;
		return;
	}

	++iterations;

	Node *node = ChooseNode();

	if (*node == *finish)
	{
		node = node->previous;
		while (node->previous != nullptr)
		{
			path.push_back(node);
			node = node->previous;
		}
		finished = true;
		return;
	}

	auto it = find(reachable.begin(), reachable.end(), node);
	if (it != reachable.end())
		reachable.erase(it);
	explored.push_back(node);

	for (size_t i = 0; i < node->adjacent.size(); ++i)
	{
		AddAdjacent(*node, *(node->adjacent.at(i)));
	}
	if (allowDiagonal)
	{
		for (size_t i = 0; i < node->diagonally_adjacent.size(); ++i)
		{
			AddAdjacent(*node, *(node->diagonally_adjacent.at(i)));
		}
	}

	// =============Draw every step==================
	for (auto &node : reachable)
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

void Search::AddAdjacent(Node &node, Node &adjacent)
{
    /*
	 *	Add the adjacent node to reachable
	 *	only if it's a walkable node
	 */ 
	if (adjacent.value == 0)
	{
		if (find(explored.begin(), explored.end(), &adjacent) != explored.end())
		{
			return;
		}
		if (find(reachable.begin(), reachable.end(), &adjacent) != reachable.end())
		{
			return;
		}
		adjacent.previous = &node;
		reachable.push_back(&adjacent);
	}
}

Node* Search::ChooseNode()
{
	srand(static_cast<unsigned>(time(NULL)));
	return reachable.at(rand() % reachable.size());
}