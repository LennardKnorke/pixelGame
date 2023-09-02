#include "application.hpp"

GAME_STATE Application::gameLoop(void){
    std::cout << "Started the game loop!"<< std::endl;
    


    //at the end. dereference the current game, preparing for either ending game or going to the menu
    delete activeSave;
    return QUIT;
}
