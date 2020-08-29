#include "Grid.h"

Grid::Grid()
{}

Grid::Grid(vector<vector<int>> &grid)
{
	/*
	* Grid that can accept a 2D array
	* representing a tilemap and convert
	* it into an array of nodes
	*/
	Node node;
	nodes.reserve(grid.size()*grid.at(0).size());
	for (size_t i = 0; i < grid.size(); ++i)
	{
		for (size_t j = 0; j < grid.at(i).size(); ++j)
		{
			node.value = grid.at(i).at(j);
			node.nodeIndex = grid.size() * i + j;
			node.cell.setSize(sf::Vector2f(650 / static_cast<float>(grid.at(0).size()),
										   650 / static_cast<float>(grid.size())));
			node.cell.setPosition(j*node.cell.getSize().y, i*node.cell.getSize().x);
			node.cell.setOutlineThickness(1.f);
			nodes.push_back(node);
		}
    }

	int rows = grid.at(0).size();
	int columns = grid.size();

	/*
	 * Find the adjacent nodes for
	 * every walkable node in the grid
	 */
	for (int r = 0; r < rows; ++r)
		for (int c = 0; c < columns; ++c)
		{
			// Test if it's an unwalkable node
			if (nodes.at(columns * r + c).value == 1)
				continue;

			// Up
			if (r > 0)
				nodes.at(columns * r + c).adjacent.push_back(&nodes.at(columns * (r - 1) + c));

			// Right
			if (c < columns - 1)
				nodes.at(columns * r + c).adjacent.push_back(&nodes.at(columns * r + c + 1));

			// Down
			if (r < rows - 1)
				nodes.at(columns * r + c).adjacent.push_back(&nodes.at(columns * (r + 1) + c));

			// Left
			if (c > 0)
				nodes.at(columns * r + c).adjacent.push_back(&nodes.at(columns * r + c - 1));

			// Connect diagonally
			// Up Left
			if (c > 0 && r > 0)
				nodes[columns * r + c].diagonally_adjacent.push_back(&nodes.at((r - 1) * columns + (c - 1)));
			
			// Up Right
			if (r < rows - 1 && c > 0)
				nodes[columns * r + c].diagonally_adjacent.push_back(&nodes[(r + 1) * columns + (c - 1)]);

			// Down Left
			if (r > 0 && c < columns - 1)
				nodes[columns * r + c].diagonally_adjacent.push_back(&nodes[(r - 1) * columns + (c + 1)]);

			// Down Right
			if (r < rows - 1 && c < columns - 1)
				nodes[columns * r + c].diagonally_adjacent.push_back(&nodes[(r + 1) * columns + (c + 1)]);
				
		}
}
