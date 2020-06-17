#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Level
{
public:
	std::vector<Tile> levelParts;
	std::vector<sf::Vector2i> bonePiles;
	std::vector<Tile> foreground;
	std::vector<std::vector<int>> data;
	sf::Texture levelTexture;
	int hideBoneInPile;

public:

	Level(int id);
	int checkTile(int x, int y);
	bool checkForBones(int m, int n);
	int getHiddenBoneID();
	bool canWalk(int x, int y);
	bool canClimbUp(int x, int y);
	bool canClimbDown(int x, int y, bool npc);
	bool checkForComplete(int x, int y, int currentAction);
	void setLevel(int id);
	std::vector<std::vector<int>> getLevel(int id);	
	void draw(sf::RenderWindow& window, int zIndex);
};

