#include "Button.h"
#include <iostream>
#include "Button.h"

Button::Button()
{
	buttonConfigured = false;
}

Button::Button(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &fillColor)
	:rect(size)
{
	rect.setPosition(position);
	rect.setFillColor(fillColor);
	buttonConfigured = false;
}

sf::RectangleShape& Button::getRect()
{
	return rect;
}

sf::Text& Button::getButtonText()
{
	return buttonText;
}

void Button::buttonConfig (	const sf::Vector2f &size = sf::Vector2f(0.f, 0.f),
							const sf::Vector2f &position = sf::Vector2f(0.f, 0.f),
							const sf::Color &fillColor = sf::Color::White,
							const string &name = "Button" )
{
	rect.setSize(size);
	rect.setPosition(position);
	rect.setFillColor(fillColor);
	if (!font.loadFromFile("../Font/ARJULIAN.ttf"))
		cerr << "Can't load font!" << endl;
	buttonText.setFont(font);
	buttonText.setCharacterSize(22);
	buttonText.setString(name);
	buttonText.setPosition(rect.getPosition().x + 3, rect.getPosition().y - 4);
	
}

void Button::setPosition(const sf::Vector2f &position)
{
	rect.setPosition(position);
	buttonText.setPosition(rect.getPosition().x + 3, rect.getPosition().y - 4);
}

void Button::setHoverColor(const sf::Color &color)
{
	rect.setFillColor(sf::Color(150, 150, 150, 155));
}

sf::Color Button::getHoverColor()
{
	return hoverColor;
}

void Button::draw(sf::RenderWindow &window)
{
	window.draw(rect);
	window.draw(buttonText);
}