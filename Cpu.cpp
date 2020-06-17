#include <iostream>
#include "Cpu.h"

void Cpu::create(std::string textureFN, sf::Vector2u imageCount, float switchTime)
{
	texture.loadFromFile(textureFN);
	row = 0;
	faceRight = true;

	body.setSize(sf::Vector2f(100, 80));
	body.setPosition(144.0f, 73.0f);
	body.setTexture(&texture);
	animation.create(&texture, imageCount, switchTime);
}

void Cpu::reset() {
	body.setPosition(144.0f, 73.0f);
}

sf::Vector2f Cpu::getBodyPosition() {
	return body.getPosition();
}

void Cpu::update(float deltaTime, sf::Vector2f movement, int tileID, bool grounded) {
	row = 0; //default state
	if (movement.y < 0.0f || movement.y > 0.0f)	row = 1;
	
	if (movement.x > 0.0f) faceRight = true;
	else faceRight = false;

	animation.update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(movement);
}

void Cpu::draw(sf::RenderWindow& window) {
	window.draw(body);
}
