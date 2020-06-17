#include "Tile.h"

Tile::Tile(sf::Texture* texture, sf::Vector2u textureStart, sf::Vector2u textureEnd, sf::Vector2i pos) {
	bgRect.setSize(sf::Vector2f(textureEnd.x - textureStart.x, textureEnd.y - textureStart.y));
	bgRect.setPosition(pos.x, pos.y);
	bgRect.setTexture(texture);
	bgRect.setTextureRect(sf::IntRect(textureStart.x, textureStart.y, textureEnd.x - textureStart.x, textureEnd.y - textureStart.y));

}

void Tile::draw(sf::RenderWindow& window) {
	window.draw(bgRect);
}

Tile::~Tile() {};
