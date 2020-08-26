#pragma once
#include <vector>
using namespace std;
#include "SFML/Graphics.hpp"
#include "Node.h"

class Grid
{
public:
	vector<Node> nodes;
	Grid();
	Grid(vector<vector<int>> &grid);
};

