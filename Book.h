#pragma once
#include <iostream>
#include <unordered_map>
#include <SFML\Graphics.hpp>

class Book
{
public:
	std::unordered_map<std::string, sf::Text> pages;
	sf::Font initFont;
public:
	Book();
	void addPage(std::string id, float x, float y, std::string title);
	void setText(std::string id, std::string title);
	void setPosition(std::string id, float x, float y);
	void setCharSize(std::string id, int pxSize);
	void draw(sf::RenderWindow& window);
	~Book();
};

