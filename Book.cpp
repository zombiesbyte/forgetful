#include "Book.h"


Book::Book() {
    if (!initFont.loadFromFile("VT323-Regular.ttf")) {
        std::cout << "Error: VT323-Regular.ttf found not found. Please download it via Google Fonts" << std::endl;
    }
}

void Book::addPage(std::string id, float x, float y, std::string title) {
    
    sf::Text newPage;
    pages.insert({ id, newPage });
    pages[id].setFont(initFont);
    setCharSize(id, 22);
    setText(id, title);
    setPosition(id, x, y);
}

void Book::setText(std::string id, std::string title) {
    pages[id].setString(title);
}

void Book::setPosition(std::string id, float x, float y) {
    pages[id].setPosition(x, y);
}

void Book::setCharSize(std::string id, int pxSize) {
    pages[id].setCharacterSize(pxSize);
}

void Book::draw(sf::RenderWindow& window) {
    for (auto& page : pages) {
        //our second element is our sf::Text obj
        window.draw(page.second);
    }
}

Book::~Book() {}