#include <iostream>
#include <time.h>
#include <stdio.h>
#include "Game.h"

int main()
{
    srand((unsigned)time(NULL)); //initialize random seed
    
    auto game = Game{};
    
    game.run();

    return 0;

}



