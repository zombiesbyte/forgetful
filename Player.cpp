#include <iostream>
#include "Player.h"

void Player::create(std::string textureFN, sf::Vector2u imageCount, float switchTime)
{
	texture.loadFromFile(textureFN);
	row = 0;
	faceRight = true;
	
	body.setSize(sf::Vector2f(100, 80));
	body.setPosition(144.0f, 650.f);
	body.setTexture(&texture);
	animation.create(&texture, imageCount, switchTime);
}

void Player::reset() {
	body.setPosition(144.0f, 650.f);
}

sf::Vector2f Player::getBodyPosition() {
	return body.getPosition();
}

void Player::update(float deltaTime, sf::Vector2f movement, int tileID, bool grounded, int action) {
	
	int rowMod = 0;

	if (action == 3) rowMod = 6;
	
	if(tileID == 1 || tileID == 2 || tileID == 4 || tileID == 5 || tileID == 6 || tileID == 8) { //walk
		if (movement.x == 0.0f && movement.y == 0.0f) {
			if(grounded) row = 0 + rowMod;
			else row = 3 + rowMod;
		}
		else if (movement.x < 0.0f || movement.x > 0.0f) {
			row = 1 + rowMod;
			if (movement.x > 0.0f) faceRight = true;
			else faceRight = false;
		}
		else if (movement.y < 0.0f || movement.y > 0.0f) {
			row = 2 + rowMod;
			faceRight = true;
		}
	}

	if(tileID == 3) { //climb
		if (movement.y == 0.0f) row = 3 + rowMod;
		else if (movement.y < 0.0f || movement.y > 0.0f) {
			row = 2 + rowMod;
			faceRight = true;
		}		
	}

	if (action == 1) { //digging
		row = 4;
		movement.x = 0.0f;
		movement.y = 0.0f;
	}
	else if (action == 2) { //found bone
		row = 5;
		movement.x = 0.0f;
		movement.y = 0.0f;
	}
	else if (action == 4) { //level completed
		row = 10;
		movement.x = 0.0f;
		movement.y = 0.0f;
	}
	else if (action == 9) { //death
		row = 11;
		movement.x = 0.0f;
		movement.y = 0.0f;
	}

	animation.update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(movement);
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(body);
}
