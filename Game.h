#pragma once
#include "Level.h"
#include "Book.h"

class Game
{
public:

	bool debugOn = false;
	bool debuggerArmed = false;

	float deltaTime;
	sf::Clock clock;
	unsigned int hiScore;
	unsigned int levelScore;
	unsigned int playerScore;

	struct Piece {
		sf::Vector2f movement;
		float speed;
		sf::Vector2f pos;
		sf::Vector2i tile;
		sf::Vector2i lastTile;
		bool faceRight;
		bool faceUp;
		int action;
		bool grounded;
		int digging;
		bool npc;
		int lives;
		int delay;
		bool resetLevel;
		bool nextLevel;
	};

	std::vector<Piece> pieces;
	Book book;
	Level level;
	int currentLevel;

public:
	Game();
	void init();
	void setPlayer1();
	void setNPC();
	void run();
	void pollWindow(sf::RenderWindow& window);
	void pollKeyboard();
	void calculatePositions();
	void movePieces();
	bool checkCollisions();
	void hud(sf::RenderWindow& window);
	void debug();
	std::string formatStr(std::string text, int align = 0, int stringWidth = 80, char fill = ' ');
};

