#include <iostream>
#include <unordered_map>
#include <conio.h>
#include "Level.h"


Level::Level(int id) {
    Level::setLevel(id);
}

int Level::checkTile(int y, int x) {

    //std::cout << "data: data[" << data.size() << "][" << data[0].size() << "]" << std::endl;

    if (x < data.size()) {
        if (y < data[x].size()) {
            return data[x][y];
        }
        //else std::cout << "trying to access y Level.data[" << x << "][y] (" << y << ") outside of element size (" << data[x].size() << ")" << std::endl;
    }
    //else std::cout << "trying to access x Level.data[x] (" << x << ") outside of element size (" << data.size() << ")" << std::endl;
    return 0;
}

bool Level::checkForBones(int n, int m) {
    if (checkTile(n, m) == 5) {
        for (int i = 0; i < bonePiles.size(); i++) {
            if (bonePiles[i].x == n && bonePiles[i].y == m) {
                if (hideBoneInPile == i) return true;
            }
        }
    }
    return false;
}

int Level::getHiddenBoneID() {
    return hideBoneInPile;
}

bool Level::canWalk(int x, int y) {
    int tileID = checkTile(x, y);
    if (tileID == 1 || tileID == 2 ||
        tileID == 4 || tileID == 5 ||
        tileID == 6 || tileID == 8) {
        return true;
    }
    return false;
}

bool Level::canClimbUp(int x, int y) {
    int tileID = checkTile(x, y);
    if (tileID >= 2 && tileID <= 3) {
        return true;
    }
    return false;
}

bool Level::canClimbDown(int x, int y, bool npc) {
    int tileID = checkTile(x, y);
    if (tileID >= 3 && tileID <= 4 || tileID == 6){
        if (tileID == 6) {
            if (npc) return false;
            else return true;
        }
        return true;
    }
    return false;
}

bool Level::checkForComplete(int x, int y, int currentAction) {
    int tileID = checkTile(x, y);
    if (tileID == 8 && currentAction == 3) {
        return true;
    }
    else return false;
}

void Level::setLevel(int id) {
    levelTexture.loadFromFile("level-sprites.png");
    data = Level::getLevel(id);
    std::cout << id << std::endl;
    levelParts.clear();
    bonePiles.clear();

    for (int m = 0; m < data.size(); m++) {
        for (int n = 0; n < data[m].size(); n++) {
            if (data[m][n] == 1)
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 0, 0 }, sf::Vector2u{ 72, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
            if (data[m][n] == 2)
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 72, 0 }, sf::Vector2u{ 144, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
            if (data[m][n] == 3)
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 144, 0 }, sf::Vector2u{ 216, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
            if (data[m][n] == 4)
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 216, 0 }, sf::Vector2u{ 288, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
            if (data[m][n] == 5) {
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 288, 0 }, sf::Vector2u{ 360, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
                bonePiles.push_back({ n, m });
            }
            if (data[m][n] == 6)
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 216, 0 }, sf::Vector2u{ 288, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });

            if (data[m][n] == 8) {
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 0, 72 }, sf::Vector2u{ 144, 216 }, sf::Vector2i{ ((n) * 72) - 60 , ((m - 1) * 72) + 84 });
                levelParts.emplace_back(&levelTexture, sf::Vector2u{ 0, 0 }, sf::Vector2u{ 72, 72 }, sf::Vector2i{ (n * 72) - 60, (m * 72) + 84 });
                foreground.emplace_back(&levelTexture, sf::Vector2u{ 0, 72 }, sf::Vector2u{ 144, 216 }, sf::Vector2i{ ((n) * 72) - 60 , ((m - 1) * 72) + 84 });
            }

        }
    }

    hideBoneInPile = rand() % bonePiles.size(); // rand 0 - (bonePiles.size() - 1);
}

std::vector<std::vector<int>> Level::getLevel(int id) {

    //level 1 data (map this to an image in future)
    //0 = blank
    //1 = ground (cycles through 3 texture patterns)
    //2 = intersection between ground and ladder feet
    //3 = ladder
    //4 = intersection between ground and ladder top
    //5 = hotspot
    //6 = like 4 but restricts cpu from going down
    //8 = p1-start
    //9 = cpu-start (thunder / lightening vacuume / lawnmover)

    std::unordered_map<int, std::vector<std::vector<int>>> allLevels {
        {
            0, {
                //0  1  2  3  4  5  6  7  8  9  10
                { 0, 4, 1, 1, 5, 1, 1, 1, 4, 1, 0 },//0
                { 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0 },//1
                { 0, 2, 1, 4, 0, 0, 0, 4, 2, 4, 0 },//2
                { 0, 0, 0, 3, 0, 0, 0, 3, 0, 3, 0 },//3
                { 0, 0, 0, 3, 0, 5, 4, 2, 1, 2, 0 },//4
                { 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0 },//5
                { 0, 1, 5, 2, 1, 1, 2, 1, 1, 6, 0 },//6
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0 },//7
                { 0, 8, 1, 1, 1, 1, 1, 1, 1, 2, 0 },//8
            },
        }, {
            1, {
                //0  1  2  3  4  5  6  7  8  9  10
                { 0, 4, 5, 1, 1, 1, 4, 1, 1, 1, 0 },//0
                { 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0 },//1
                { 0, 2, 1, 4, 1, 1, 2, 1, 1, 4, 0 },//2
                { 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0 },//3
                { 0, 4, 1, 2, 1, 5, 1, 4, 1, 2, 0 },//4
                { 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0 },//5
                { 0, 2, 1, 1, 6, 1, 1, 2, 1, 4, 0 },//6
                { 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0 },//7
                { 0, 8, 1, 1, 2, 0, 5, 1, 1, 2, 0 },//8
            }
        }
    };
    return allLevels[id];
}

void Level::draw(sf::RenderWindow& window, int zIndex) {
    
    //could make levelParts another dimension/map so I don't need an extra var specifically for foreground
    if (zIndex == 0) {
        for (int n = 0; n < levelParts.size(); n++) {
            levelParts[n].draw(window);
        }
    }
    else if (zIndex == 1) {
        for (int n = 0; n < foreground.size(); n++) {
            foreground[n].draw(window);
        }
    }
}
