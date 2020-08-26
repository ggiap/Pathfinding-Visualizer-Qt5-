#pragma once
#include <vector>
using namespace std;
#include "SFML/Graphics.hpp"

class Node
{
public:
	int value = 0;
	int nodeIndex;
	bool nodeValueChanged = false;
	vector<Node *> adjacent;
	vector<Node *> diagonally_adjacent;
	sf::RectangleShape cell;
	Node *previous;
	double hCost = 0;
	double gCost = 0;
	double fCost = 0;

	Node();
	Node(int newValue);

	bool operator==(Node &rhs);
	void Clear()
	{
		previous = nullptr;
	}
};

