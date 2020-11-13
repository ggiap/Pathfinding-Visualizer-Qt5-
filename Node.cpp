#include "Node.h"


Node::Node() : nodeIndex(), previous(nullptr)
{
	cell.setSize(sf::Vector2f(30, 30));
	cell.setOutlineColor(sf::Color::Black);
	cell.setOutlineThickness(-2);
	previous = nullptr;
}

Node::Node(int newValue) : nodeIndex(), previous(nullptr)
{
	value = newValue;
}

bool Node::operator==(Node &rhs)
{
	if (cell.getPosition() == rhs.cell.getPosition())
		return true;
	return false;
}
