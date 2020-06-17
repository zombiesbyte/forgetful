#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

class Player
{
public:
	sf::Texture texture;
	sf::RectangleShape body;
	Animation animation;
	unsigned int row;
	bool faceRight;

public:
	void create(std::string textureFN, sf::Vector2u imageCount, float switchTime);
	void reset();
	sf::Vector2f getBodyPosition();
	void update(float deltaTime, sf::Vector2f movement, int tileID, bool grounded, int action);
	void draw(sf::RenderWindow& window);
};

