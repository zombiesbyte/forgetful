#pragma once
#include <SFML/Graphics.hpp>

class Tile
{
public:
	sf::RectangleShape bgRect;
	sf::Font font;
public:
	Tile(sf::Texture* texture, sf::Vector2u textureStart, sf::Vector2u textureEnd, sf::Vector2i pos);
	void draw(sf::RenderWindow& window);
	~Tile();
};

