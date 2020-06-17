#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Cpu.h"

#include "Game.h"


Game::Game() : level(0) {
    init();
}

void Game::init() {
    Game::setPlayer1();
    Game::setNPC();
    deltaTime = 0.0f;
    hiScore = 10253;
    levelScore = 10000;
    playerScore = 0;
    currentLevel = 0;
}

void Game::setPlayer1() {
    Game::Piece p1;
    p1.movement = { 0.0f, 0.0f };
    p1.speed = 100.0f;
    p1.pos = { 0.0f, 0.0f };
    p1.tile = { 0, 0 };
    p1.lastTile = { 0, 0 };
    p1.faceRight = true;
    p1.faceUp = false;
    p1.action = 0;
    p1.grounded = true;
    p1.digging = 0;
    p1.npc = false;
    p1.lives = 3;
    p1.delay = 0;
    p1.resetLevel = false;
    p1.nextLevel = false;
    pieces.push_back(p1);
}

void Game::setNPC() {
    Game::Piece npc;
    npc.movement = { 0.0f, 0.0f };
    npc.speed = 0.6f;
    npc.pos = { 0.0f, 0.0f };
    npc.tile = { 0, 0 };
    npc.lastTile = { 0, 0 };
    npc.faceRight = true;
    npc.faceUp = false;
    npc.action = 0;
    npc.grounded = true;
    npc.digging = 0;
    npc.npc = true;
    npc.lives = 0;
    npc.delay = 0;
    npc.resetLevel = false;
    npc.nextLevel = false;
    pieces.push_back(npc);
}

void Game::run() {

    sf::RenderWindow window(sf::VideoMode(672, 768), "My Window", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(120);

    Player player;
    player.create("forgetful-spritesheet.png", sf::Vector2u(12, 12), 0.08f);

    Cpu lawnMower;
    lawnMower.create("lawn-mower-spritesheet.png", sf::Vector2u(12, 2), 0.08f);

    while (window.isOpen()) {

        if(levelScore > 0) levelScore--;

        if (pieces[0].delay == 0 && pieces[0].resetLevel) {
            level.setLevel(currentLevel);
            pieces[1].faceRight = true;
            pieces[1].faceUp = false;
            pieces[1].grounded = true;
            pieces[0].faceRight = true;
            pieces[0].faceUp = false;
            pieces[0].grounded = true;
            player.reset();
            lawnMower.reset();
            pieces[0].resetLevel = false;
            pieces[0].action = 0;
        }
        else if (pieces[0].delay == 0 && pieces[0].nextLevel) {
            currentLevel++;
            std::cout << "increased level" << std::endl;
            if (currentLevel > 1) currentLevel = 0; //I just need to add more levels :)
            level.setLevel(currentLevel);
            pieces[1].speed += 0.2f;
            pieces[1].faceRight = true;
            pieces[1].faceUp = false;
            pieces[1].grounded = true;
            player.reset();
            lawnMower.reset();
            pieces[0].nextLevel = false;
            pieces[0].action = 0;
            levelScore = 10000;
        }

        deltaTime = clock.restart().asSeconds();

        Game::pollWindow(window);

        pieces[0].pos = player.getBodyPosition();
        pieces[1].pos = lawnMower.getBodyPosition();

        Game::calculatePositions();
        Game::pollKeyboard();
        Game::movePieces();
        if (Game::checkCollisions()) {
            if (pieces[0].delay == 0) {
                pieces[0].action = 9;
                pieces[0].lives--;
                pieces[0].delay = 200;
                pieces[0].resetLevel = true;
                if (pieces[0].lives == 0) {
                    exit(1);
                }
            }
        }

        player.update(deltaTime, pieces[0].movement, level.checkTile(pieces[0].tile.x, pieces[0].tile.y), pieces[0].grounded, pieces[0].action);
        lawnMower.update(deltaTime, pieces[1].movement, level.checkTile(pieces[1].tile.y, pieces[1].tile.x), pieces[1].grounded);

        window.clear();
        Game::hud(window);
        Game::debug(); //update our debug info (if debugOn)
        if (pieces[0].action != 9) level.draw(window, 0); //bg
        book.draw(window);
        if(pieces[0].action != 9) lawnMower.draw(window);
        player.draw(window);        
        if (pieces[0].action != 9) level.draw(window, 1); //fg
        window.display();
    }


}

void Game::pollWindow(sf::RenderWindow& window) {
    sf::Event evnt;
    while (window.pollEvent(evnt)) {
        switch (evnt.type) {
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

void Game::calculatePositions() {
    for (auto& piece : pieces) {
        piece.movement.x = 0.0f;
        piece.movement.y = 0.0f;

        if (piece.faceRight) piece.tile.x = round((piece.pos.x + 60) / 72);
        else piece.tile.x = round((piece.pos.x + 60) / 72);

        if (piece.faceUp) piece.tile.y = round((piece.pos.y - 37) / 72);
        else piece.tile.y = round((piece.pos.y - 108) / 72);
    }
}

void Game::pollKeyboard() {
    pieces[0].movement.x = 0.0f;
    pieces[0].movement.y = 0.0f;

    if (pieces[0].faceRight) pieces[0].tile.x = round((pieces[0].pos.x + 60) / 72);
    else pieces[0].tile.x = round((pieces[0].pos.x + 60) / 72);

    if (pieces[0].faceUp) pieces[0].tile.y = round((pieces[0].pos.y - 37) / 72);
    else pieces[0].tile.y = round((pieces[0].pos.y - 108) / 72);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        pieces[0].faceRight = false;
        pieces[0].movement.x -= pieces[0].speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        pieces[0].faceRight = true;
        pieces[0].movement.x += pieces[0].speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        pieces[0].faceUp = true;
        pieces[0].grounded = false;
        pieces[0].movement.y -= pieces[0].speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        pieces[0].faceUp = false;
        pieces[0].grounded = false;
        pieces[0].movement.y += pieces[0].speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (pieces[0].action == 0) pieces[0].action = 1;
    }
}

void Game::movePieces() {

    for (auto& piece : pieces) {
        if (piece.npc) {
            if ((level.checkTile(piece.tile.x, piece.tile.y) == 4 ||
                level.checkTile(piece.tile.x, piece.tile.y) == 6) && ((int)piece.pos.x % 72) < 5) {
                if (rand() % 2) {

                    if (piece.lastTile.x != piece.tile.x) {
                        piece.faceUp = false;
                        piece.grounded = false;
                    }
                }
                piece.lastTile = { piece.tile.x, piece.tile.y };
            }
            else if (level.checkTile(piece.tile.x, piece.tile.y) == 2 && ((int)piece.pos.x % 72) < 5) {
                if (rand() % 3) {

                    if (piece.lastTile.x != piece.tile.x) {
                        piece.faceUp = true;
                        piece.grounded = false;

                    }
                }
                piece.lastTile = { piece.tile.x, piece.tile.y };
            }

            if (!piece.faceUp && !piece.grounded) {
                piece.movement.x = 0.0f;
                piece.movement.y = piece.speed;
            }
            else if (piece.faceUp && !piece.grounded) {
                piece.movement.x = 0.0f;
                piece.movement.y = -piece.speed;
            }
            else if (piece.faceRight && piece.grounded) {
                piece.movement.x = piece.speed;;
            }
            else if (!piece.faceRight && piece.grounded) {
                piece.movement.x = -piece.speed;
            }
        }

        //safeguards
        if (piece.tile.x > 10) piece.tile.x = 10;
        if (piece.tile.x < 0) piece.tile.x = 0;
        if (piece.tile.y > 8) piece.tile.y = 8;

        if (piece.movement.x > 0) {
            int checkX = round((piece.pos.x + piece.movement.x + 60) / 72);
            if (!level.canWalk(checkX, piece.tile.y) || !piece.grounded) {
                piece.movement.x = 0.0f;
                if (piece.npc) {
                    piece.faceRight = false;
                    piece.lastTile.x = 0;
                }
            }
        }
        if (piece.movement.x < 0) {
            int checkX = round((piece.pos.x + piece.movement.x + 60) / 72);
            if (!level.canWalk(checkX, piece.tile.y) || !piece.grounded) {
                piece.movement.x = 0.0f;
                if (piece.npc) {
                    piece.faceRight = true;
                }

            }
        }

        if (piece.movement.y < 0) {
            if (!level.canClimbUp(piece.tile.x, piece.tile.y) || ((int)(piece.pos.x + 60) % 72 < 40 || (int)(piece.pos.x + 60) % 72 > 70)) {
                piece.movement.y = 0.0f;
                piece.grounded = true;
            }
        }
        if (piece.movement.y > 0) {
            if (!level.canClimbDown(piece.tile.x, piece.tile.y, piece.npc) || (((int)(piece.pos.x + 60) % 72) < 40 || (int)(piece.pos.x + 60) % 72 > 70)) {
                piece.movement.y = 0.0f;
                piece.grounded = true;
            }
        }


        if(!piece.npc) {
            if (pieces[0].action == 1) {
                pieces[0].digging++;
                if (pieces[0].digging > 100) {
                    pieces[0].digging = 0;
                    pieces[0].action = 0;
                    //std::cout << "Checking" << std::endl;
                    if (level.checkForBones(pieces[0].tile.x, pieces[0].tile.y)) {
                        pieces[0].action = 2;
                        //std::cout << "found" << std::endl;
                    }
                }
            }

            if (pieces[0].action == 2) {
                pieces[0].digging++;
                if (pieces[0].digging > 200) {
                    pieces[0].digging = 0;
                    pieces[0].action = 3;
                }
            }

            if (level.checkForComplete(pieces[0].tile.x, pieces[0].tile.y, pieces[0].action)) {
                if (pieces[0].delay == 0) {
                    pieces[0].action = 4;
                    pieces[0].delay = 200;
                    pieces[0].nextLevel = true;
                    playerScore += levelScore;
                    if (playerScore > hiScore) hiScore = playerScore;
                }
            }

            if (pieces[0].delay > 0) {
                pieces[0].delay--;
                piece.movement.x = 0.0f;
                piece.movement.y = 0.0f;
            }
        }
    }
}

bool Game::checkCollisions() {
    for (auto& piece : pieces) {
        if (piece.npc) {
            if (pieces[0].tile.x == piece.tile.x) {
                if (pieces[0].tile.y == piece.tile.y) return true;                
            }
        }
    }
    return false;
}

void Game::hud(sf::RenderWindow& window) {
    book.addPage("hiScore", 12.0f, 0.0f, "HIGH SCORE");
    book.addPage("hiScoreInt", 12.0f, 25.0f, formatStr(std::to_string(hiScore), 2, 8, '0'));
    book.addPage("p1Score", 150.0f, 0.0f, "P1 SCORE");
    book.addPage("p1ScoreInt", 150.0f, 25.0f, formatStr(std::to_string(playerScore), 2, 8, '0'));
    book.addPage("lvlScoreInt", 150.0f, 50.0f, formatStr(std::to_string(levelScore), 2, 8, '0'));

    book.addPage("lives", 520.0f, 735.0f, "LIVES");

    sf::Texture lifeTexture;
    lifeTexture.loadFromFile("life.png");
    sf::RectangleShape lifeBox;
    lifeBox.setSize(sf::Vector2f(24, 24));
    
    if (pieces[0].lives > 0) {
        lifeBox.setPosition(580.0f, 740.0f);
        lifeBox.setTexture(&lifeTexture);
        window.draw(lifeBox);
        if (pieces[0].lives > 1) {
            lifeBox.setPosition(610.0f, 740.0f);
            window.draw(lifeBox);
            if (pieces[0].lives > 2) {
                lifeBox.setPosition(640.0f, 740.0f);
                window.draw(lifeBox);
            }
        }
    }
    Game::debug();
}

void Game::debug() {
    if (debugOn) {

        std::string moundSearch;

        if (!debuggerArmed) {
            book.addPage("chrFloatPos", 300.0f, 0.0f, "");
            book.addPage("chrTilePos", 300.0f, 25.0f, "");
            book.addPage("dataTilePos", 300.0f, 50.0f, "");
            book.addPage("bonePileInfo", 300.0f, 75.0f, "");
            debuggerArmed = true;
        }

        if (level.checkTile(pieces[0].tile.x, pieces[0].tile.y) == 5) {
            if (level.checkForBones(pieces[0].tile.x, pieces[0].tile.y)) moundSearch = "found";
            else moundSearch = "not found";
        }

        book.setText("chrFloatPos", "[Float] x:" + std::to_string(pieces[0].pos.x) + "px | y:" + std::to_string(pieces[0].pos.y) + "px");
        book.setText("chrTilePos", " [Tile] x:" + std::to_string(pieces[0].tile.x) + "t  | y:" + std::to_string(pieces[0].tile.y) + "t");
        book.setText("dataTilePos", " [Data]  :" + std::to_string(level.checkTile(pieces[0].tile.x, pieces[0].tile.y)));
        book.setText("bonePileInfo", "[BoneIn] :" + std::to_string(level.getHiddenBoneID()) + " [Mound Search]: " + std::to_string(pieces[0].delay));
    }
}

std::string Game::formatStr(std::string text, int align, int stringWidth, char fill) {
    if (align == 1) { //center align
        text.insert(text.begin(), (stringWidth / 2) - (text.length() / 2), fill);
        text.append(stringWidth - text.length(), fill);
    }
    else if (align == 2) { //right align
        text.insert(text.begin(), stringWidth - text.length(), fill);
    }
    else { //left align
        text.append(stringWidth - text.length(), fill);
    }
    return text;
}